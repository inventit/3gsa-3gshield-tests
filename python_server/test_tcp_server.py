#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import binascii

TCP_IP = "your-test-server-address"
# TCP_IP = 'localhost'
TCP_PORT = 8001
SIZE = 256
BUFFER_SIZE = SIZE

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print">> connecting"
s.connect((TCP_IP, TCP_PORT))
print">> sending"
print b"%d" % SIZE
s.send(b"%d" % SIZE)

# s.send(str(SIZE))

# a='000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff'
# s.send(a.decode('hex'))

print">> waiting"
data = s.recv(BUFFER_SIZE)
print "size:", len(data)
print "received data(hex):", binascii.hexlify(data)

print"2>> sending"
print b"%d" % SIZE
s.send(b"%d" % SIZE)
print"2>> waiting"
data = s.recv(BUFFER_SIZE)
print "2size:", len(data)
print "2received data(hex):", binascii.hexlify(data)


s.close()
assert len(bytearray(data)) == SIZE
