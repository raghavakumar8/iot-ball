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
    print 'Fetching data for week: ' + `start` + ' - ' + `end`

    # Query database
    c.execute('SELECT * FROM scrum WHERE timestamp >= ? AND timestamp <= ?', (start, end))
    rows = c.fetchall()

    # Add default data so all days show up
    zero = {'h': 0, 'm': 0, 's': 0, 't': 0}
    meetings = [['Monday', zero, zero], ['Tuesday', zero, zero], ['Wednesday', zero, zero], 
                ['Thursday', zero, zero], ['Friday', zero, zero], ['Saturday', zero, zero],
                ['Sunday', zero, zero]]

    # Convert database rows to processable data
    for row in rows:
      start = datetime.fromtimestamp(int(row[0]))
      end = start + timedelta(seconds=row[1])
      meetings.append([start.strftime('%A'), 
                      {'h': start.hour, 'm': start.minute, 's': start.second, 't': int(row[0])}, 
                      {'h': end.hour, 'm': end.minute, 's': end.second, 't': int(row[0])}])

    data = {'meetings': meetings}

  elif request.args['type'] == 'meeting':
    print 'Fetching data for meeting at: ' + `request_time`

    # Query database
    c.execute('SELECT * FROM scrum WHERE timestamp = ?', (request_time, ))
    rows = c.fetchall()

    if rows == []:
      # Fallback query
      print 'No meeting found for that timestamp. Returning latest meeting instead.'
      c.execute('SELECT * FROM scrum WHERE timestamp = (select max(timestamp) from scrum)')
      rows = c.fetchall()

    # Convert row to processable data
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

    # Query database
    c.execute('SELECT * FROM scrum')
    rows = c.fetchall()
    
    # Convert database rows to processable data
    trends = []
    for row in rows:
      start = datetime.fromtimestamp(int(row[0]))
      date = start.strftime('%d %B %X %p')
      total_time = float(row[1])

      num_sections = len(row[3].split(' ')) - 1
      avg_time = total_time/num_sections

      trends.append([date, total_time, num_sections, avg_time])

    data = {'trends': trends}

  sys.stdout.flush()
  conn.close()

  return jsonify(data)


def get_week_timestamps(timestamp):
  """ Get timestamps that demarcate the start and end of this week """
  dt = datetime.fromtimestamp(timestamp)
  time_since_week_start = timedelta(days=dt.weekday(),
                                    hours=dt.hour,
                                    minutes=dt.minute,
                                    seconds=dt.second)

  week_start = dt - time_since_week_start
  week_end = week_start + timedelta(days=7)

  utc_difference = datetime.utcfromtimestamp(timestamp) - dt
  utc_week_start = week_start + utc_difference
  utc_week_end = week_end + utc_difference

  return (datetime_to_epoch(utc_week_start), datetime_to_epoch(utc_week_end))


def datetime_to_epoch(date):
  """ Helper to convert a UTC datetime object into seconds since epoch. """
  epoch = datetime.utcfromtimestamp(0)
  return int((date - epoch).total_seconds())

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
    