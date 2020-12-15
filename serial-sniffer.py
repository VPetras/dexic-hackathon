#!/usr/bin/env python3
# -*- coding: utf-8 -*-

##################################################
# DEX-IC Hackathon APP - Serial sniffer
# Title: This is sub script which is used to listen on serial port and translate instruction between arduino and GUI
# Author: Vojtěch Petrásek 
# Generated: 15/12/2020 10:59:33
##################################################

import serial
import time
import sys
import os
import requests


dim = 500
arduino = serial.Serial('/dev/ttyUSB0', 115200)
graph_data = [0 for x in range(dim)]

def update_graph_data(array,value):
    sec_array = [x for x in range(len(array))]
    sec_array[0] = value
    for i in range(len(array)-1):
        sec_array[i+1] = array[i]
    return sec_array

def main(graph_data):
    while True:
        data = ((arduino.readline()).decode('utf-8')).strip('\r\n')
        print(data)
        graph_data = update_graph_data(graph_data,float(data))
        print(graph_data)
        #log = open('request.log','a')
        try:
            headers = {'content-type': 'application/json'}
            r = requests.post('http://0.0.0.0:8000/upload', str(graph_data))
            #log.write(str(r) + "\n")
        except Exception as e:
            #log.write(str(e) + "\n")

if __name__ == '__main__':
    try:
        main(graph_data)
    except KeyboardInterrupt:
        pass
    except Exception as e:
        print('Exited with error: {}'.format(e))
        sys.exit(1)