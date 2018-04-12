#!/usr/bin/env python
# Reference: http://banjolanddesign.com/flask-google-charts.html

from flask import Flask, render_template
from flask_socketio import SocketIO, emit

__author__ = "raghava@maidbot.co (Raghava Kumar)"

app = Flask(__name__, template_folder='templates')
socketio = SocketIO(app)

@app.route('/')
def load_chart():
    data = [['a', 1], ['b', 2], ['c', 1.5]]
    data = {'row_data': data}
    return render_template('chart.html', data=data)

@socketio.on('message', namespace='/data')
def test_message(message):
    send(message)

@socketio.on('connect', namespace='/data')
def test_connect():
    print 'Client connected'

@socketio.on('disconnect', namespace='/data')
def test_disconnect():
    print 'Client disconnected'

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=80)
    