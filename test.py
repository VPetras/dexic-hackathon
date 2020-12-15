#!/usr/bin/env python3
# -*- coding: utf-8 -*-

##################################################
# google-home-test
# Title: 
# Author: Vojtěch Petrásek 
# Generated: 07/12/2020 09:59:10
##################################################

import requests

log = open('test.log','a')
try:
    headers = {'content-type': 'application/json'}
    r = requests.get('http://0.0.0.0:8000/update')
    log.write(str(r) + "\n")
    print(r.text)
except Exception as e:
    print(e)
    log.write(str(e) + "\n")