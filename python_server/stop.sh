#!/bin/bash

ps -ef | grep tcp_server | awk {'print $2'} | xargs kill -9
