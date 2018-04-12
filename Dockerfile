# Based on: https://github.com/resin-io-projects/simple-server-python
FROM resin/raspberrypi3-python

WORKDIR /usr/src/app

COPY web web
RUN pip install -r web/requirements.txt

ENV INITSYSTEM on
CMD ["python","web/main.py"]
