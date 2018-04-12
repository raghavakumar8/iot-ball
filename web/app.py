#!/usr/bin/env python
# Reference: http://banjolanddesign.com/flask-google-charts.html

from flask import Flask, render_template

__author__ = "raghava@maidbot.co (Raghava Kumar)"

app = Flask(__name__, template_folder='templates')

@app.route('/')
def load_chart():
    data = [['a', 1], ['b', 2], ['c', 1.5]]
    data = {'row_data': data}
    return render_template('chart.html', data=data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)