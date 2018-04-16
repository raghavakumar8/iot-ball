#!/usr/bin/env python
# Reference: http://banjolanddesign.com/flask-google-charts.html

import sys
import time
import sqlite3

from datetime import datetime, timedelta
from flask import Flask, request, render_template, jsonify
from flask_sockets import Sockets

__author__ = "raghava@maidbot.co (Raghava Kumar)"

app = Flask(__name__, template_folder='templates')
sockets = Sockets(app)
database = ''

@app.route('/chart_data')
def get_data():
  """ Serve data to web front end """
  request_time = int(request.args['time'])

  conn = sqlite3.connect(database)
  c = conn.cursor()

  if request.args['type'] == 'week':
    (start, end) = get_week_timestamps(request_time)
    print 'Getting data for week: ' + `start` + ' - ' + `end`
    c.execute('SELECT * FROM scrum WHERE timestamp >= ? AND timestamp <= ?', (start, end))
    rows = c.fetchall()

    print rows
    data = [['Monday', 0, 0], ['Tuesday', 0, 0], ['Wednesday', 0, 0], 
            ['Thursday', 0, 0], ['Friday', 0, 0], ['Saturday', 0, 0],
            ['Sunday', 0, 0]]    

  elif request.args['type'] == 'meeting':
    print 'Getting data for meeting at: ' + `request_time`
    c.execute('SELECT * FROM scrum WHERE timestamp = ?', (request_time, ))
    rows = c.fetchall()

    if rows == []:
      print 'No meeting found for that timestamp. Returning latest meeting instead.'
      c.execute('SELECT * FROM scrum WHERE timestamp = (select max(timestamp) from scrum)')
      rows = c.fetchall()

    timestamp = int(rows[0][0])
    sections = rows[0][3].split(' ')

    section_data = []  
    for i in xrange(len(sections)-1):
      section_name = 'Section ' + `i+1`
      section_data.append([section_name, float(sections[i])])

    data = {'sections': section_data}
    data['meeting_info'] = datetime.fromtimestamp(timestamp).strftime('%A, %d %B at %X %p')

  elif request.args['type'] == 'trend':
    print 'Getting trends'

    # TODO: Actually implement this
    data = {'data': [['a', 1], ['b', 2], ['c', 3.5]]}

  sys.stdout.flush()
  conn.close()

  return jsonify(data)


def get_week_timestamps(timestamp):
  """ Get timestamps that demaracte the start and end of this week """
  dt = datetime.fromtimestamp(timestamp)
  time_since_week_start = timedelta(days=dt.weekday(),
                                    hours=dt.hour,
                                    minutes=dt.minute,
                                    seconds=dt.second)

  week_start = dt - time_since_week_start
  week_end = week_start + timedelta(days=7)

  epoch = datetime.fromtimestamp(0)

  start = int((week_start - epoch).total_seconds())
  end = int((week_end - epoch).total_seconds())
  return (start, end)

@app.route('/')
def load_chart():
  return render_template('index.html')


@sockets.route('/echo')
def echo_socket(ws):
  print "Client connected to: /echo"
  sys.stdout.flush()

  while not ws.closed:
    message = ws.receive()
    ws.send(message)

  print "Client disconnected"
  sys.stdout.flush()


def insert_into_table(c, data):
  # Process data semantics
  sections = data[0]
  duration = data[1]/1000.0
  timestamp = int(time.time() - duration)

  section_times = ''
  for i in xrange(2, len(data)):
    section_times += str(data[i]/1000.0) + ' '

  # Assemble and insert row
  row = (timestamp, duration, data[0], section_times)
  c.execute("INSERT INTO scrum VALUES (?, ?, ?, ?)", row)


@sockets.route('/scrum_data')
def data_socket(ws):
  print "Client connected to: /scrum_data"
  sys.stdout.flush()

  conn = sqlite3.connect(database)
  c = conn.cursor()

  while not ws.closed:
    message = ws.receive()
    data_str = message.split(", ")

    try:
      data = [int(datum) for datum in data_str]
      if len(data) < 3 or data[0] != len(data) - 2:
        raise ValueError()

      insert_into_table(c, data)

      conn.commit()
      ws.send("OK")

    except ValueError:
      ws.send("BAD")

  print "Client disconnected"
  sys.stdout.flush()
  conn.close()


if __name__ == '__main__':
    database = sys.argv[1]
    from gevent import pywsgi
    from geventwebsocket.handler import WebSocketHandler
    server = pywsgi.WSGIServer(('0.0.0.0', 80), app, handler_class=WebSocketHandler)
    server.serve_forever()
    