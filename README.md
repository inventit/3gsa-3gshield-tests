3gsa-3gshield-tests
===================

Programs and tools for testing 3G Shield with IEM 2.0 by [3gsa.org](http://3gsa.org/)

# Python Server

## Supported Python Version

Python 2.7

## Depenencies Installation

    $ pip install twisted

## How to run

    $ cd python_server
    $ python tcp_server.py

Then the server is listening on the port 8001.

## Server behavior

The server will return a byte array data when a client is sending a text string of a numeric value.
The length of the byte array should be same as the sent numeric value.

A test client is able to specify the length of the receiving data.

# Arduino Client

## Installation

Replace all `"your-test-server-address"` text in the source codes of this project (`*.ino` and `*.py` files) with your public IP address or DNS name.

Copy all folders and files under `arduino_client` to your Arduino sketch folder.

Copy `SHA-1` folder under `arduino_lib` to your Arduino IDE `libraries` folder as well.
