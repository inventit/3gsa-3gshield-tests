#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Packetdump for wireshark
# sudo tcpdump -nnXSs 0 -i eth0 -s 65535 -w bs_dump 'port 8001'
PORT=8001

import logging
import datetime
import binascii
import hashlib
from twisted.internet import reactor, protocol, endpoints
from twisted.protocols import basic

header = False
logger = logging.getLogger('tcp_server')
hdlr = logging.FileHandler('tcp_server.log')
formatter = logging.Formatter('%(message)s')
hdlr.setFormatter(formatter)
logger.addHandler(hdlr) 
logger.setLevel(logging.INFO)

class PubProtocol(basic.LineReceiver):
    
    def __init__(self, factory):
        self.factory = factory
        global logger
        self.logger = logger
        basic.LineReceiver.setRawMode(self)

    def connectionMade(self):
        self.factory.clients.add(self)

    def connectionLost(self, reason):
        self.factory.clients.remove(self)

    def lineReceived(self, line):
        """
        Not used for now as `basic.LineReceiver.setRawMode(self)` is invoked.
        """
        for c in self.factory.clients:
            c.sendLine("<{}> {}".format(self.transport.getHost(), line))

    def rawDataReceived(self, data):
        """
        Waiting a second, then sending back the given size of binary data to clients.
        The data payload is handled as a text of an int value.
        """
        try:
            if data[0]=='\x01':
                # data[1] = char
                # data[2:] = str expression of payload length
                size = int(data[2:])
                payload = self.createData(size, ord(data[1]))
                self.logger.info("--------------------------------------------------- %s" % datetime.datetime.now())
                self.logger.info("Generated Data Size => %s" % size)
                self.logger.info("Byte Value => %d" % ord(data[1]))
                self.logger.info("Payload (sha-1) => %s" % hashlib.sha1(payload).hexdigest().upper())
                for c in self.factory.clients:
                    c.send(payload)
            
            elif data[0]=='\x02':
                # data[1:301] = input, 300 bytes
                # data[301:321] = sha-1, 20 bytes
                inp = data[1:301]
                inpInDigest = data[301:321]
                if inpInDigest == hashlib.sha1(inp).digest():
                    inpOk = "Good"
                else:
                    inpOk = "Bad"

                payload = self.createData(300)
                payloadHash = hashlib.sha1(payload)
                out = payload + payloadHash.digest()

                global header
                if header == False:
                    self.logger.info("datetime, in size, in sha-1 hex, in ok?, out size, out sha-1")
                    header = True

                # datetime, in size, in sha-1 hex, in ok?, out size, out sha-1
                self.logger.info("%s,%d,%s,%s,%d,%s" % (datetime.datetime.now(),
                    len(data), inpInDigest.encode("hex").upper(), inpOk,
                    len(out), payloadHash.hexdigest().upper()))
                for c in self.factory.clients:
                    c.send(out)
            else:
                # broadcast to clients
                size = int(data)
                payload = self.createData(size)
                self.logger.info("--------------------------------------------------- %s" % datetime.datetime.now())
                self.logger.info("Generated Data Size => %s" % size)
                self.logger.info("Payload (sha-1) => %s" % hashlib.sha1(payload).hexdigest().upper())
                for c in self.factory.clients:
                    c.send(payload)
        except ValueError:
            # binary data, one way
            self.logger.info("Data Received => %d" % len(data))
            self.logger.info("Payload (sha-1) => %s" % hashlib.sha1(data).hexdigest().upper())

    def send(self, data):
        self.transport.write(data)

    def createData(self, size, v=None):
        """
        Creates a byte string of the given length of byte array.
        Each value in the array is a byte-size int value.
        """
        result = bytes()
        for i in range(0, size):
            if v is None:
                c = i % 256
            else:
                c = v
            result = result + ("%0.2X" % c)
        return result.decode('hex')

class PubFactory(protocol.Factory):
    def __init__(self):
        self.clients = set()

    def buildProtocol(self, addr):
        return PubProtocol(self)

if __name__ == '__main__':
    print "Starting TCP server on the port : " + str(PORT)
    endpoints.serverFromString(reactor, "tcp:" + str(PORT)).listen(PubFactory())
    reactor.run()
