#!/usr/bin/env python
# -*- coding:utf-8 -*-

from tornado import ioloop
from tornado import iostream
import socket

def send_request():
    stream.write("GET /index.html HTTP/1.0\r\nHost: nginx.net\r\n\r\n")
    stream.read_until("\r\n\r\n", on_headers)

def on_headers(data):
    headers = {}
    for line in data.split("\r\n"):
       parts = line.split(":")
       if len(parts) == 2:
           headers[parts[0].strip()] = parts[1].strip()
    stream.read_bytes(int(headers["Content-Length"]), on_body)

def on_body(data):
    print data
    stream.close()
    ioloop.IOLoop.instance().stop()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
stream = iostream.IOStream(s)
stream.connect(("nginx.net", 80), send_request)
ioloop.IOLoop.instance().start()
