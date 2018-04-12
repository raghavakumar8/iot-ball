#!/usr/bin/env python
# Reference: http://banjolanddesign.com/flask-google-charts.html

import sys
from flask import Flask, render_template
from flask_sockets import Sockets

__author__ = "raghava@maidbot.co (Raghava Kumar)"

app = Flask(__name__, template_folder='templates')
sockets = Sockets(app)

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

if __name__ == '__main__':
    from gevent import pywsgi
    from geventwebsocket.handler import WebSocketHandler
    server = pywsgi.WSGIServer(('0.0.0.0', 80), app, handler_class=WebSocketHandler)
    server.serve_forever()
    