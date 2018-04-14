#!/usr/bin/env python
# Reference: http://banjolanddesign.com/flask-google-charts.html

import sys
import time
import sqlite3

from flask import Flask, render_template
from flask_sockets import Sockets

__author__ = "raghava@maidbot.co (Raghava Kumar)"

app = Flask(__name__, template_folder='templates')
sockets = Sockets(app)
database = ''

@app.route('/')
def load_chart():
  data = [['a', 1], ['b', 2], ['c', 1.5]]
  data = {'row_data': data}
  return render_template('chart.html', data=data)


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
    