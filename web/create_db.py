#!/usr/bin/env python
# Reference: http://sebastianraschka.com/Articles/2014_sqlite_in_python_tutorial.html

import sys
import sqlite3

__author__ = "raghava@maidbot.co (Raghava Kumar)"

if __name__ == '__main__':
  conn = sqlite3.connect(sys.argv[1])
  c = conn.cursor()

  c.execute("CREATE TABLE scrum (timestamp integer, duration real, sections integer, section_data text)")

  conn.commit()
  conn.close()