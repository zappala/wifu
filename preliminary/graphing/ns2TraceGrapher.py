#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__="rbuck"
__date__ ="$Nov 30, 2011 10:38:56 AM$"

import sys
sys.path.append("../")
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import optparse
from fileparser import FileParser
from pylab import *
import os

class NSTraceGrapher():
	def __init__(self, input, output):
		self.input = input
		self.output = output
		self.packets = []
		self.acks = []
		self.packet_drops = []
		self.ack_drops = []
		self.__parse__()

#		self.__print__(self.packets, "Packets")
#		self.__print__(self.acks, "Acks")
#		self.__print__(self.packet_drops, "Packet Drops")
#		self.__print__(self.ack_drops, "Ack Drops")

	def __print__(self, array, name):
		print name
		for item in array:
			print item


	def __parse__(self):
		parser = FileParser()
		packets = False
		acks = False
		drops = False
		do_nothing = False
		for line in parser.parse(self.input):
			if len(line) == 0:
				continue

			if "skip" in line:
				do_nothing = True
				
			if line.startswith("\"packets"):
				packets = True
				do_nothing = False
				continue
				
			if line.startswith("\"acks"):
				acks = True
				drops = False
				do_nothing = False
				continue

			if line.startswith("\"drops"):
				drops = True
				do_nothing = False
				continue

			if do_nothing:
				continue

			if packets and not acks:
				if drops:
					self.packet_drops.append(line)
				else:
					self.packets.append(line)
			
			if acks:
				if drops:
					self.ack_drops.append(line)
				else:
					self.acks.append(line)

	def __get_values(self, line):
		time, packet = line.split()
		time = float(time) - 1
		packet = (float(packet) - 1) * 100
		return time, packet

	def graph(self):
		fig = plt.figure(figsize=(20, 6.5))
		fig.canvas.set_window_title('A Boxplot Example')
		ax = fig.add_subplot(111)

		ax.set_xlabel('Time (seconds)')
		ax.set_ylabel('Packet Number (Mod 60)')
		

		for line in self.packets:
			time, packet = self.__get_values(line)
			ax.plot(time, packet, color='black', marker='s', markersize=4)

		for line in self.acks:
			time, packet = self.__get_values(line)
			ax.plot(time, packet, color='black', marker='.', markersize=2)

		for line in self.packet_drops:
			time, packet = self.__get_values(line)
			ax.plot(time, packet, color='black', marker='s', markersize=4)
			ax.plot(time, packet, color='black', marker='x', markersize=12)

		for line in self.ack_drops:
			time, packet = self.__get_values(line)
			ax.plot(time, packet, color='black', marker='.', markersize=2)
			ax.plot(time, packet, color='black', marker='x', markersize=6)
		
		xmin, xmax, ymin, ymax = ax.axis()
		
		scale = 0.05

		xoffset = (xmax - xmin) * scale
		yoffset = (ymax - ymin) * scale
		xmin -= xoffset
		xmax += xoffset
		ymin -= yoffset
		ymax += yoffset
		ax.axis([xmin, xmax, ymin, ymax])

		savefig(self.output, format="eps",bbox_inches='tight',pad_inches=0.05)
	
class PcapConverter():
	def __init__(self, input):
		self.input = input

		self.packets = []
		self.acks = []
		self.packet_drops = []
		
		self.__parse()
		self.tempfilename = "converted.tmp"
		self.__write()

	def get_file(self):
		return self.tempfilename

	def remove_file(self):
		os.remove(self.tempfilename)

	def __write(self):
		f = open(self.tempfilename, 'w')

		f.write("\"packets\n")
		for line in self.packets:
			f.write(line + "\n")

		f.write("\"drops\n")
		for line in self.packet_drops:
			print line
			f.write(line + "\n")
			
		f.write("\"packets\n")
		f.write("\"acks\n")
		for line in self.acks:
			f.write(line + "\n")

		f.close()

	def __get_packet_number(self, sequence_number):
		packet_number = int(sequence_number / 1448)
		packet_number %= 60
		return packet_number

	def __parse(self):
		parser = FileParser()

		s = set()

		packet_numbers = {}
		sequence_numbers = {}

		start = False;
		
		for line in parser.parse(self.input):
			array = line.split()
			if len(array) == 6:
				# header
				continue

			if "Len=0" not in line:
				start = True

			if not start:
				continue

			if "FIN" in line:
				break;

			is_ack = ("ACK" in line) and ("Len=0" in line)
			is_data = ("Len=0" not in line) or ("SYN" in line) or ("FIN" in line)

			packet_number = -1
			time = float(array[1]) + 1 # add one to be like ns-2

			sequence_number = -1
			length = -1
			ack_number = -1

			for item in array:
				if is_data and "Seq=" in item:
					sequence_number = int(item.split("=")[1])
					packet_number = self.__get_packet_number(sequence_number)
				elif is_ack and "Ack=" in item:
					ack_number = int(item.split("=")[1])
				elif "Len=" in item:
					length = int(item.split("=")[1])

			if "SYN" in line or "FIN" in line:
				length = 1
				
					
			temp = ""
			

			if packet_number < 10:
				temp = "0"

			tsvl = array[-2]
			tser = array[-1]
			ts = tsvl + tser

			if is_data:
				data = str(time) + " " + "1." + temp + str(packet_number)
				self.packets.append(data)
				packet_key = sequence_number + length
#				print "Packet key:    ", packet_key
#				print "Packet number: ", packet_number
				if packet_key not in packet_numbers.keys():
					packet_numbers[sequence_number + length] = packet_number

				
					
			else:
				assert is_ack
				if ts in s:
#					print "Ack number: ", ack_number
#
#					print "Ack packet number: ", packet_numbers[ack_number]

					temp = ""
					if packet_numbers[ack_number] < 10:
						temp = "0"
					
					data = str(time) + " " + "1." + temp + str(packet_numbers[ack_number])
#					print data
					self.acks.append(data)


			


			if length > 1 and sequence_numbers.has_key(sequence_number) and sequence_numbers[sequence_number] != ts:
				# duplicate
				for item in self.packets:					
					num = item.split()[1]
					num = int(num.split(".")[1])
					if num == self.__get_packet_number(sequence_number):
						self.packets.remove(item)
						self.packet_drops.append(item)
						
						break
				

			s.add(ts)


			if length > 1 and sequence_numbers.has_key(sequence_number) and sequence_numbers[sequence_number] == ts:
				del sequence_numbers[sequence_number]
			else:
				sequence_numbers[sequence_number] = ts

		

if __name__ == "__main__":
	"""
	You can use the proper typesetting unicode minus (see
	http://en.wikipedia.org/wiki/Plus_sign#Plus_sign) or the ASCII hypen
	for minus, which some people prefer.  The matplotlibrc param
	axes.unicode_minus controls the default behavior.

	The default is to use the unicode minus
	"""

	parser = optparse.OptionParser()
	parser.add_option("-n", "--ns", dest="ns", help="Graph NS-2 type input.")
	parser.add_option("-p", "--pcap", dest="pcap", help="Graph pcap type input already converted to text via wireshark.")
	parser.add_option("-o", "--output", dest="filename", help="Filename of the resulting graph")

	(options, args) = parser.parse_args()

	pcap = options.pcap
	ns = options.ns
	filename = options.filename

	if pcap and ns:
		print "Cannot define both pcap and ns type files.  Choose one or the other."
		exit()

	if not pcap and not ns:
		print "Need at least one file."
		exit()

	if not filename:
		print "Need output file."
		exit()

	if pcap:
		"""
		Scrub create temp file similar to what ns does and then process as ns.
		"""
		p = PcapConverter(pcap)
		g = NSTraceGrapher(p.get_file(), filename)
		g.graph()
		p.remove_file()
	else:
		g = NSTraceGrapher(ns, filename)
		g.graph()

#	
