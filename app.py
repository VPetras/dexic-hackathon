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
from kivy.base import runTouchApp
from kivy.uix.floatlayout import FloatLayout
from kivy.clock import Clock
from kivy.core.window import Window
from kivy_garden.graph import Graph, MeshLinePlot
import os
import sys
import threading
import time
import kivy
from math import sin
#import kivy_garden
#import RPi.GPIO as GPIO


class MainScreen(FloatLayout):

    def __init__(self, **kwargs):
        super(MainScreen, self).__init__(**kwargs)
        self.thread = None
        self.started = False
        self.mainlabel = Label(text ="DEX-IC smart vent",font_size = 60, size_hint=(1, 0.2), pos_hint={'x':0, 'y':0.8})
        self.githublabel = Label(text ="github.com/VPetras/dexic-hackathon",font_size = 25, size_hint=(1.6, 0.1), pos_hint={'x':0.08, 'y':0})
        self.button = Button(text ='STOP',font_size = 30, size_hint=(0.3, 0.1), pos_hint={'x':0.35, 'y':0.01})
        self.add_widget(self.mainlabel)
        self.add_widget(self.githublabel)
        self.add_widget(self.button)
        #self.button.disabled=True

        self.ier_button = Button(text ='IE / R',font_size = 30, size_hint=(0.1, 0.05), pos_hint={'x':0.01, 'y':0.75})
        self.add_widget(self.ier_button)
        self.av_button = Button(text ='A / V',font_size = 30, size_hint=(0.1, 0.05), pos_hint={'x':0.01, 'y':0.65})
        self.add_widget(self.av_button)
        self.rr_button = Button(text ='R / R',font_size = 30, size_hint=(0.1, 0.05), pos_hint={'x':0.01, 'y':0.55})
        self.add_widget(self.rr_button)

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
            x_grid=True,
            y_grid=True,
            ymin=-5,
            ymax=5,
            size_hint=(0.7, 0.5),
            pos_hint={'x':0.25, 'y':0.3})
        self.plot = MeshLinePlot(mode='line_strip', color=[1, 1, 0, 1])
        self.plot.points = [(x / 10., sin(x / 10.)*4) for x in range(-0, 5000)]
        self.graph.add_plot(self.plot)
        self.add_widget(self.graph)


class GuiApp(App):

    def build(self):
        self.title = 'DEX-IC smart vent'
        return MainScreen()

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