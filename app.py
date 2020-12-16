#!/usr/bin/env python3
# -*- coding: utf-8 -*-

##################################################
# DEX-IC Hackathon APP
# Title: This is script which run on Kivy framework for visualization controlled aplication based on raspberry pi
# Author: Vojtěch Petrásek 
# Generated: 14/12/2020 20:55:01
##################################################

###
# imports section
###

from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.dropdown import DropDown 
from kivy.uix.widget import Widget
from kivy.uix.image import Image
from kivy.base import runTouchApp
from kivy.uix.floatlayout import FloatLayout
from kivy.clock import Clock
from kivy_garden.graph import Graph, MeshLinePlot
import os
import sys
import threading
import time
import kivy
import requests
import json
from math import sin
#import RPi.GPIO as GPIO


class MainScreen(FloatLayout):

    def __init__(self, **kwargs):
        super(MainScreen, self).__init__(**kwargs)
        self.thread = None
        self.started = False
        self.ie_val = 2
        self.av_val = 200
        self.rr_val = 20
        self.mainlabel = Label(text ="DEX-IC smart vent",font_size = 60, size_hint=(1, 0.2), pos_hint={'x':0, 'y':0.8})
        self.githublabel = Label(text ="github.com/VPetras/dexic-hackathon",font_size = 25, size_hint=(1.6, 0.1), pos_hint={'x':0.08, 'y':0})
        self.button = Button(text ='Start',font_size = 30, size_hint=(0.3, 0.1), pos_hint={'x':0.35, 'y':0.01})
        self.button.bind(on_press = self.stop_btn)
        self.add_widget(self.mainlabel)
        self.add_widget(self.githublabel)
        self.add_widget(self.button)
        #self.button.disabled=True

        self.dex = Image(source="dex-logo.jpg", size_hint=(0.15, 0.15), pos_hint={'x':0.16, 'y':0.05})
        self.add_widget(self.dex)
        self.eit = Image(source="EIT-Health.jpg", size_hint=(0.15, 0.15), pos_hint={'x':0.01, 'y':0.05})
        self.add_widget(self.eit)

        self.ier_button = Button(text ="IE / R = 1:{}".format(self.ie_val),font_size = 30, size_hint=(0.15, 0.1), pos_hint={'x':0.01, 'y':0.75})
        self.add_widget(self.ier_button)
        self.ier_p_button = Button(text ='+',font_size = 30, size_hint=(0.075, 0.05), pos_hint={'x':0.085, 'y':0.68})
        self.ier_p_button.bind(on_press = self.ie_p)
        self.add_widget(self.ier_p_button)
        self.ier_m_button = Button(text ='-',font_size = 30, size_hint=(0.075, 0.05), pos_hint={'x':0.01, 'y':0.68})
        self.ier_m_button.bind(on_press = self.ie_m)
        self.add_widget(self.ier_m_button)

        self.av_button = Button(text ="A V = {}".format(self.av_val),font_size = 30, size_hint=(0.15, 0.1), pos_hint={'x':0.01, 'y':0.55})
        self.add_widget(self.av_button)
        self.av_p_button = Button(text ='+',font_size = 30, size_hint=(0.075, 0.05), pos_hint={'x':0.085, 'y':0.48})
        self.av_p_button.bind(on_press = self.av_p)
        self.add_widget(self.av_p_button)
        self.av_m_button = Button(text ='-',font_size = 30, size_hint=(0.075, 0.05), pos_hint={'x':0.01, 'y':0.48})
        self.av_m_button.bind(on_press = self.av_m)
        self.add_widget(self.av_m_button)

        self.rr_button = Button(text ="R / R = {}".format(self.rr_val),font_size = 30, size_hint=(0.15, 0.1), pos_hint={'x':0.01, 'y':0.35})
        self.add_widget(self.rr_button)
        self.rr_p_button = Button(text ='+',font_size = 30, size_hint=(0.075, 0.05), pos_hint={'x':0.085, 'y':0.28})
        self.rr_p_button.bind(on_press = self.rr_p)
        self.add_widget(self.rr_p_button)
        self.rr_m_button = Button(text ='-',font_size = 30, size_hint=(0.075, 0.05), pos_hint={'x':0.01, 'y':0.28})
        self.rr_m_button.bind(on_press = self.rr_m)
        self.add_widget(self.rr_m_button)

        self.graph = Graph(
            xlabel='Time',
            ylabel='Pressure',
            x_ticks_minor=1,
            x_ticks_major=5,
            y_ticks_major=1,
            y_grid_label=True,
            x_grid_label=True,
            padding=10,
            xlog=False,
            ylog=False,
            x_grid=False,
            y_grid=False,
            ymin=0,
            ymax=200,
            size_hint=(0.7, 0.5),
            pos_hint={'x':0.25, 'y':0.3})
        self.plot = MeshLinePlot(mode='line_strip', color=[1, 0, 0, 1])
        values = [0]
        self.plot.points = [(x, values[x]) for x in range(-0, len(values))]
        self.graph.add_plot(self.plot)
        self.add_widget(self.graph)

    def stop_btn(self, x):
        if self.started:
            self.button.text="Start"
            self.started = False
        else:
            self.button.text="Stop"
            self.started = True

    def ie_p(self,x):
        if self.ie_val + 0.1 < 3.1:
            self.ie_val += 0.1
            self.ier_button.text="IE / R = 1:{:.1f}".format(self.ie_val)

    def ie_m(self,x):
        if self.ie_val - 0.1 > 0.9:
            self.ie_val -= 0.1
            self.ier_button.text="IE / R = 1:{:.1f}".format(self.ie_val)

    def av_p(self,x):
        if self.av_val + 50 < 850:
            self.av_val += 50
            self.av_button.text="A V = {}".format(self.av_val)

    def av_m(self,x):
        if self.av_val - 50 > 150:
            self.av_val -= 50
            self.av_button.text="A V = {}".format(self.av_val)

    def rr_p(self,x):
        if self.rr_val + 1 < 41:
            self.rr_val += 1
            self.rr_button.text="R / R = {}".format(self.rr_val)

    def rr_m(self,x):
        if self.rr_val - 1 > 5:
            self.rr_val -= 1
            self.rr_button.text="R / R = {}".format(self.rr_val)


    def update_graph(self, *args):
        print("update graph")
        #log = open('test.log','a')
        try:
            headers = {'content-type': 'application/json'}
            r = requests.get('http://0.0.0.0:8000/update')
            #log.write(str(r) + "\n")
            print(r.text)
            values = json.loads(r.text)
            print(values)
            self.graph.xmax = len(values)
            self.plot.points = [(x, values[x]) for x in range(-0, len(values))]
        except Exception as e:
            print(e)
            #log.write(str(e) + "\n")


class GuiApp(App):

    def build(self):
        self.title = 'DEX-IC smart vent'
        screen = MainScreen()
        Clock.schedule_interval(screen.update_graph, 0.1)
        return screen

if __name__ == '__main__':
    try:
        GuiApp().run()
    except KeyboardInterrupt:
        #GPIO.cleanup()
        sys.exit(1)
    except Exception as e:
        print('Exited with error: {}'.format(e))
        #GPIO.cleanup()
        sys.exit(1)