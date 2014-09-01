#!/usr/bin/env python
# -*- coding: utf-8 -*-

import SocketServer

class MyTCPHandler(SocketServer.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(1024).strip()
        print "{} wrote:".format(self.client_address[0])
        print self.data
        
        self.request.sendall(self.createData(256))

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

if __name__ == "__main__":
    HOST, PORT = "0.0.0.0", 8001

    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
