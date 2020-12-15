#!/usr/bin/env python3
# -*- coding: utf-8 -*-

##################################################
# DEX-IC Hackathon APP - API
# Title: This API is for data transfer between arduino and GUI
# Author: Vojtěch Petrásek 
# Generated: 07/12/2020 09:59:10
##################################################

from flask import Flask, request, jsonify

app = Flask(__name__)

data = [(0,0.0)]
@app.route('/upload', methods=['POST'])
def upload():
    #print(request.data)
    global data
    data = request.data
    return request.data

@app.route('/update', methods=['GET'])
def update():
    global data
    return data

@app.route('/')
def main():
    return "hello word"


if __name__ == '__main__':
    app.run(host= '0.0.0.0',debug=True,port=8000)