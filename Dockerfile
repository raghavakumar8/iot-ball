# Based on: https://github.com/resin-io-projects/simple-server-python
FROM resin/raspberrypi3-python

WORKDIR /usr/src/app

COPY web/requirements.txt web/
RUN pip install -r web/requirements.txt

COPY web web

ENV INITSYSTEM on
CMD ["python","web/app.py"]
