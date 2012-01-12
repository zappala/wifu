#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "rbuck"
__date__ = "$Jan 9, 2012 3:21:20 PM$"

import sys
import time

from FileOps import *
from RemoteCommand import Command
from fileparser import FileParser
import optparse
sys.path.append("/home/ilab/pylib")
from directory import *
from Stats import *
import os
from collections import deque

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from pylab import *

class InstantaneousGrapher:
	def __init__(self, file):
		self.file = file
		self.__parse()
		self.__create_data_arrays()

	def __parse(self):
		print "Parsing file..."
		parser = FileParser()
		self.lines = parser.parse(file)

		# contains distinct flows, opened with a SYN
		self.flows = []

		for line in self.lines:
			line = line.strip()
			tokens = line.split(' ')
			if '.' not in tokens[1]:
				continue

			if "SYN" in line:
				self.flows.append([])
				continue

			if "Len=0" in line:
				continue

			time = float(tokens[1])
			length = 0
			for token in tokens:
				if "Len=" in token:
					length = int(token.split('=')[1])
					break

			self.flows[len(self.flows) - 1].append((time, length))

	def __get_rate(self, begin_time, end_time, flow):
		bytes = 0
		for (time, length) in flow:
			if time >= begin_time and (time < end_time or time == self.__get_max_time(flow)):
				bytes += length

		bits = bytes * 8
		bits_per_sec = bits / (end_time - begin_time)
		Mbps = bits_per_sec / 1000000
		return Mbps

	def __get_max_time(self, flow):
		(time, length) = flow[len(flow) - 1]
		return time

	def __get_min_time(self, flow):
		(time, length) = flow[0]
		return time

	def __create_data_arrays(self):
		print "Creating data arrays..."
		self.data = []
		# window is in seconds
		self.window = 0.001
		self.interval = 0.0001
		self.window = 0.01
		self.interval = 0.001
		
		for flow in self.flows:
			x = []
			y = []
			max_time = self.__get_max_time(flow)
			begin = self.__get_min_time(flow)
			first_time = begin
			end = begin + self.window
			while end < max_time:
				x_val = begin - first_time
				y_val = self.__get_rate(begin, min([max_time, end]), flow)
				x.append(x_val)
				y.append(y_val)

				begin += self.interval
				end = begin + self.window

			self.data.append((x, y))
				
	def graph(self):
		print "Graphing..."
		fig = plt.figure()
		ax = fig.add_subplot(111)
		#ax.set_title("Smoothed rate over time")
		ax.set_xlabel("Time (seconds)")
		ax.set_ylabel("Smoothed Goodput (Mbps)")
		
		for (x, y) in self.data:
			ax.plot(x, y)

		savefig("temp.eps", format="eps")

if __name__ == "__main__":

	parser = optparse.OptionParser()
	parser.add_option("-f", "--file", dest="file", help="File to parse.  This file must have already been run through wireshark's export feature.")

	(options, args) = parser.parse_args()
	file = options.file

	if file is None:
		print "Usage: instantaneous.py [-f|--file] <filename>"
		exit()

	g = InstantaneousGrapher(file)
	g.graph()