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

def myComp(a, b):
	(a_x, a_y) = a
	(b_x, b_y) = b

	if a_x[0] < b_x[0]:
		return -1
	elif a_x[0] > b_x[0]:
		return 1
	else:
		return 0

class InstantaneousGrapher:
	def __init__(self, file, output):
		self.file = file
		self.output = output
		self.__parse()
		self.__create_data_arrays()
		self.__burstiness()


	def __burstiness(self):
		# Parse

		current_key = ""
		count = 0
		self.burstiness = {}
		
		for line in self.lines:
			line = line.strip()
			tokens = line.split(' ')
			tokens = self.__clean(tokens)
			if '.' not in tokens[1]:
				continue

			key = self.__get_key(tokens)

			if "SYN" in line:
				self.burstiness[key] = {}
				continue

# Including length == 0 packets because it is a packet that must be processed
#			if "Len=0" in line:
#				continue


			if current_key is not "" and current_key != key:
				if self.burstiness[current_key].has_key(count):
					num = self.burstiness[current_key][count]
					num += 1
					self.burstiness[current_key][count] = num
				else:
					self.burstiness[current_key][count] = 1
				
			if current_key == key:
				count += 1
			else:
				current_key = key
				count = 1

		if self.burstiness[current_key].has_key(count):
			num = self.burstiness[current_key][count]
			num += 1
			self.burstiness[current_key][count] = num
		else:
			self.burstiness[current_key][count] = 1

		s = Stats()
		for key in self.burstiness:
			print key
			array = []
			for lines in self.burstiness[key].keys():
				count = self.burstiness[key][lines]
				for i in range(0, count):
					array.append(lines)

			print "Min, 25th, 50th, 75th, and Max values:"
			print float(min(array)), "\t", s.get_25th_percentile(array), "\t", s.median(array), "\t", s.get_75th_percentile(array), "\t", float(max(array))
			
			
			

	def __get_key(self, tokens):
		return tokens[5] + " " + tokens[7]

	def __clean(self, tokens):
		a = []
		for token in tokens:
			if token is "":
				continue
			else:
				a.append(token)
		return a

	def __parse(self):
		print "Parsing file..."
		parser = FileParser()
		self.lines = parser.parse(file)

		# contains distinct flows, opened with a SYN
		self.flows = {}

		for line in self.lines:
			line = line.strip()
			tokens = line.split(' ')
			tokens = self.__clean(tokens)
			if '.' not in tokens[1]:
				continue

			key = self.__get_key(tokens)

			if "SYN" in line:
				self.flows[key] = []
				continue

			if "Len=0" in line:
				continue

			time = float(tokens[1])
			length = 0
			for token in tokens:
				if "Len=" in token:
					length = int(token.split('=')[1])
					break

			self.flows[key].append((time, length))
			

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

	def _get_flows_begin_time(self):
		min = 10000000000000000000000
		for key in self.flows.keys():
			begin = self.__get_min_time(self.flows[key])
			if begin < min:
				min = begin

		return min


	def __create_data_arrays(self):
		print "Creating data arrays..."
		self.data = []
		# window is in seconds
		self.window = 0.01
		self.interval = 0.001
		#self.window = 0.1
		#self.interval = 0.01
		self.window = 0.05
		self.interval = 0.002


		absolute_begin = self._get_flows_begin_time()


		for key in self.flows.keys():
			
			flow = self.flows[key]
			x = []
			y = []
			max_time = self.__get_max_time(flow)
			begin = self.__get_min_time(flow)
			diff = begin - absolute_begin
			first_time = begin
			end = begin + self.window
			while end < max_time:
				
				x_val = begin - first_time # + diff

				# put diff in when doing concurrent flows (multi-threaded)
				# leave out when not (but wish to compare multiple flows)
				x_val += diff
				y_val = self.__get_rate(begin, min([max_time, end]), flow)
				x.append(x_val)
				y.append(y_val)

				begin += self.interval
				end = begin + self.window
				
			self.data.append((x, y))

		self.data.sort(myComp)			
				
	def graph(self):
		print "Graphing..."
		fig = plt.figure()
		ax = fig.add_subplot(111)
		#ax.set_title("Smoothed rate over time")
		ax.set_xlabel("Time (seconds)")
		ax.set_ylabel("Smoothed Goodput (Mbps)")

		colors = ["blue", "green", "red", "yellow", "black", "cyan", "magenta", "burlywood", "chartreuse", "brown"]

		i = 0

		for (x, y) in self.data:
			ax.plot(x, y, color=colors[i % len(colors)])
			i += 1

		ax.set_ylim(0, 100)

		savefig(self.output, format="eps")

if __name__ == "__main__":

	parser = optparse.OptionParser()
	parser.add_option("-f", "--file", dest="file", help="File to parse.  This file must have already been run through wireshark's export feature.")
	parser.add_option("-o", "--output", dest="output", help="File to write.  Path and name of file to write (must be eps type)")

	(options, args) = parser.parse_args()
	file = options.file
	output = options.output

	if file is None or output is None:
		print "Usage: instantaneous.py [-f|--file] <filename> [-o|--output] <outfile>"
		exit()

	g = InstantaneousGrapher(file, output)
	g.graph()
