#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "rbuck"
__date__ = "$Aug 2, 2011 9:33:17 AM$"

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

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from pylab import *

class Configuration():
	def __init__(self, file, username):
		self.file = file
		self.username = username
		
		self.dictionary = {}
		self.parse()
		self.dir = "/tmp/"

		self.iterations = int(self.dictionary["iterations"])

	def parse(self):
		parser = FileParser()

		for line in parser.parse(self.file):
			if line.startswith('#'):
				continue

			if len(line) == 0:
				continue
			
			objs = line.split(' ')
			# must have only a key and a value
			assert len(objs) == 2
			self.dictionary[objs[0]] = objs[1]
			

	def get_sender_node(self):
		key = "senderNode"
		assert key in self.dictionary
		return self.dictionary[key]

	def get_receiver_node(self):
		key = "receiverNode"
		assert key in self.dictionary
		return self.dictionary[key]

	def equal_sender_receiver(self):
		return self.get_sender_node() == self.get_receiver_node()

	def get_interface(self):
		key = "interface"
		assert key in self.dictionary
		return self.dictionary[key]

	def get_rate(self):
		key = "rate"

		if key not in self.dictionary:
			return None
		

		rate = self.dictionary[key]
		if rate == "1000":
			return "0x1000"
		if rate == "100":
			return "0x008"
		if rate == "10":
			return "0x002"
		if rate == "auto":
			return "0x3F"

		return "0x3F"


class ExecutableCopier():
	def __init__(self, configuration):
		self.config = configuration
		self.wifu = "../bin/wifu-end"
		self.sender_application = "../bin/simple-tcp-sender"
		self.receiver_application = "../bin/simple-tcp-receiver"

		self.echo_client = "../bin/echo-client"
		self.echo_server = "../bin/echo-server"

		self.sysctl_wifu = "config/sysctl_wifu.conf"
		self.sysctl_default = "config/sysctl_default.conf"

	def copy_wifu(self):
		
		
		sender = self.config.get_sender_node()
		receiver = self.config.get_receiver_node()

		all = []
		all.append(sender)
		if not self.config.equal_sender_receiver():
			all.append(receiver)

		threads = []

		for node in all:
			pusher = FilePusher(node, self.config.dir, self.wifu, self.config.username)
			threads.append(pusher)
			pusher.start()

		return threads

	def copy_sender_application(self):
		sender = self.config.get_sender_node()

		pusher = FilePusher(sender, self.config.dir, self.sender_application, self.config.username)
		pusher.start()

		return pusher

	def copy_receiver_application(self):
		receiver = self.config.get_receiver_node()

		pusher = FilePusher(receiver, self.config.dir, self.receiver_application, self.config.username)
		pusher.start()

		return pusher

	def copy_echo_client(self):
		sender = self.config.get_sender_node()

		pusher = FilePusher(sender, self.config.dir, self.echo_client, self.config.username)
		pusher.start()

		return pusher

	def copy_echo_server(self):
		receiver = self.config.get_receiver_node()

		pusher = FilePusher(receiver, self.config.dir, self.echo_server, self.config.username)
		pusher.start()

		return pusher

	def copy_sysctl_config(self):
		sender = self.config.get_sender_node()
		receiver = self.config.get_receiver_node()

		all = []
		all.append(sender)
		if not self.config.equal_sender_receiver():
			all.append(receiver)

		threads = []

		for node in all:
			for file in [self.sysctl_wifu, self.sysctl_default]:
				pusher = FilePusher(node, self.config.dir, file, self.config.username)
				threads.append(pusher)
				pusher.start()

		return threads

	def copy_all(self):
		threads = []

		threads.extend(self.copy_wifu())
		threads.extend(self.copy_sysctl_config())
		threads.append(self.copy_sender_application())
		threads.append(self.copy_receiver_application())
		threads.append(self.copy_echo_client())
		threads.append(self.copy_echo_server())

		for thread in threads:
			thread.join()

class ExecutableManager():
	def __init__(self, configuration):
		self.config = configuration

		self.senderAddress = "senderAddress"

		# wifu options
		self.loggerThreshold = "loggerThreshold"
		self.loggerTimeout = "loggerTimeout"
		self.network = "network"

		# common options between sender and receiver
		self.port = "port"
		self.wifu = "wifu"
		self.kernel = "kernel"
		self.threads = "threads"

		# sender options
		self.num = "num"
		self.sendingChunk = "sendingChunk"

		# receiver options
		self.receivingChunk = "receivingChunk"
		
	def get_wifu_command(self):
		command = "sudo nice -n -20 ./wifu-end"
		
		if self.loggerThreshold in self.config.dictionary:
			command += " --logger_threshold " + self.config.dictionary[self.loggerThreshold]
		if self.loggerTimeout in self.config.dictionary:
			command += " --logger_timeout " + self.config.dictionary[self.loggerTimeout]
		if self.network in self.config.dictionary:
			command += " --network " + self.config.dictionary[self.network]

		return command

	def start_wifu(self):
		chdir_command = "cd " + self.config.dir
		wifu_command = self.get_wifu_command()

		commands = []
		commands.append(chdir_command)
		commands.append(wifu_command)

		sender = self.config.get_sender_node()
		receiver = self.config.get_receiver_node()

		all = []
		all.append(sender)
		if not self.config.equal_sender_receiver():
			all.append(receiver)

		threads = []

		for node in all:
			c = Command(node, self.config.username, commands)
			threads.append(c)
			c.start()

		for c in threads:
			c.done.wait()
			c.go.set()
			c.finished.wait()
			c.join()

		# let wifu get going
		time.sleep(1)

	def kill_wifu(self):
		command = "sudo killall wifu-end"

		sender = self.config.get_sender_node()
		receiver = self.config.get_receiver_node()

		all = []
		all.append(sender)
		if not self.config.equal_sender_receiver():
			all.append(receiver)

		threads = []

		for node in all:
			c = Command(node, self.config.username, command)
			threads.append(c)
			c.start()

		for c in threads:
			c.done.wait()
			c.go.set()
			c.finished.wait()
			c.join()

	def setup_receiver(self, commands):
		receiver = self.config.get_receiver_node()

		all = []
		all.append(receiver)

		threads = []

		for node in all:
			c = Command(node, self.config.username, commands)
			threads.append(c)
			c.start()

		for c in threads:
			c.done.wait()
			c.go.set()
			c.finished.wait()
			c.join()


	def get_receiver_command(self, api, outfile=None, exe="simple-tcp-receiver"):
		receiver_command = "sudo nice -n -20 ./" + exe

		if self.senderAddress in self.config.dictionary:
			receiver_command += " --destination " + self.config.dictionary[self.senderAddress]
		if self.port in self.config.dictionary:
			receiver_command += " --port " + self.config.dictionary[self.port]
		if self.receivingChunk in self.config.dictionary:
			receiver_command += " --chunk " + self.config.dictionary[self.receivingChunk]
		if self.threads in self.config.dictionary:
			receiver_command += " --threads " + self.config.dictionary[self.threads]

		if api == self.kernel and self.kernel in self.config.dictionary:
			receiver_command += " --protocol " + self.config.dictionary[self.kernel]
		elif api == self.wifu and self.wifu in self.config.dictionary:
			receiver_command += " --protocol " + self.config.dictionary[self.wifu]

		receiver_command += " --api " + api

		if outfile:
			receiver_command += " > " + outfile
			
		return receiver_command

	def get_sender_command(self, api, outfile=None, exe="simple-tcp-sender"):
		sender_command = "sudo nice -n -20 ./" + exe

		if self.senderAddress in self.config.dictionary:
			sender_command += " --address " + self.config.dictionary[self.senderAddress]
		if self.port in self.config.dictionary:
			sender_command += " --port " + self.config.dictionary[self.port]
		if self.sendingChunk in self.config.dictionary:
			sender_command += " --chunk " + self.config.dictionary[self.sendingChunk]
		if self.num in self.config.dictionary:
			sender_command += " --num " + self.config.dictionary[self.num]
		if self.threads in self.config.dictionary:
			sender_command += " --threads " + self.config.dictionary[self.threads]

		if api == self.kernel and self.kernel in self.config.dictionary:
			sender_command += " --protocol " + self.config.dictionary[self.kernel]
		elif api == self.wifu and self.wifu in self.config.dictionary:
			sender_command += " --protocol " + self.config.dictionary[self.wifu]

		sender_command += " --api " + api

		if outfile:
			sender_command += " > " + outfile

		return sender_command
	
	

	def execute(self):

		current_time = str(time.ctime()).replace(' ', '_')
		base_path = "data/" + self.config.file + "_" + current_time + "/"

		maker = Directory(base_path)
		maker.make()

		os.system("cp " + self.config.file + " " + base_path)

		for i in range(0, self.config.iterations):
			time.sleep(30)

			for api in ['wifu', 'kernel']:

				data_path = base_path + str(i) + "/"

				# files will be put in a directory with the following format
				# data/[configfilename.conf]-[current_time]/[iteration]/
				maker = Directory(data_path)
				maker.make()

				if api == 'wifu':
					self.start_wifu()

				sender_node = self.config.get_sender_node()
				receiver_node = self.config.get_receiver_node()
				chdir_command = "cd " + self.config.dir

				nodes = []

				sysctl_command = "sudo sysctl -p /tmp/sysctl_wifu.conf"
				tso_command = "sudo ethtool -K " + self.config.get_interface() + " tso off"
				gso_command = "sudo ethtool -K " + self.config.get_interface() + " gso off"

				rate = self.config.get_rate()
				ethtool = "sudo ethtool " + self.config.get_interface()
				if rate is not None:
					rate_command = "sudo ethtool -s " + self.config.get_interface() + " advertise " + self.config.get_rate()

				# start up the sender
				sender_log = "sender_" + api + ".log"
				sender_command = self.get_sender_command(api, sender_log)
				sender_commands = []
				if rate is not None:
					sender_commands.append(rate_command)

				sender_commands.append(chdir_command)
				sender_commands.append(sysctl_command)

				if self.config.dictionary["tso"] is not None:
					sender_commands.append(tso_command)
				if self.config.dictionary["gso"] is not None:
					sender_commands.append(gso_command)

				sender_commands.append(ethtool)
				sender_commands.append(sender_command)

				sender = Command(sender_node, self.config.username, sender_commands)
				nodes.append(sender)
				sender.start()

				sender.running.wait()
				time.sleep(2)

				# start tcp dump
				

				tcpdump = None
				if self.config.dictionary.has_key("tcpdump"):
					tcpdump =  self.config.dictionary["tcpdump"]
					
				receiver_tcpdump = None
				sender_tcpdump = None
				if tcpdump is not None and tcpdump == "on":
					
					tcpdump_command = "sudo tcpdump -w /tmp/" + api + "_receiver_tcpdump.pcap -i " + self.config.get_interface() + " src " + self.config.dictionary[self.senderAddress]

					receiver_tcpdump = Command(receiver_node, self.config.username, tcpdump_command)
					receiver_tcpdump.start()
					receiver_tcpdump.running.wait()

					tcpdump_command = "sudo tcpdump -w /tmp/" + api + "_sender_tcpdump.pcap -i " + self.config.get_interface()
					sender_tcpdump = Command(sender_node, self.config.username, tcpdump_command)
					sender_tcpdump.start()
					sender_tcpdump.running.wait()

					

				# start up the receiver
				receiver_log = "receiver_" + api + ".log"
				
				receiver_commands = []
				if rate is not None:
					receiver_commands.append(rate_command)
									
				receiver_commands.append(sysctl_command)
				if self.config.dictionary["tso"] is not None:
					receiver_commands.append(tso_command)
				if self.config.dictionary["gso"] is not None:
					receiver_commands.append(gso_command)
				receiver_commands.append(ethtool)

				self.setup_receiver(receiver_commands)

				receiver_command = self.get_receiver_command(api, receiver_log)
				receiver_commands = []
				receiver_commands.append(chdir_command)
				receiver_commands.append(receiver_command)
				receiver = Command(receiver_node, self.config.username, receiver_commands)
				nodes.append(receiver)
				receiver.start()

				for node in nodes:
					node.done.wait()
					node.go.set()
					node.finished.wait()
					node.join()
#					print node.return_values

				if api == 'wifu':
					self.kill_wifu()

				

				a = []

				finalizing_commands = []
				sysctl_command = "sudo sysctl -p /tmp/sysctl_default.conf"
				tso_command = "sudo ethtool -K " + self.config.get_interface() + " tso on"
				gso_command = "sudo ethtool -K " + self.config.get_interface() + " gso on"
				chmod_command = "sudo chmod o+r /tmp/wifu-end.log"
				# auto is 0x03F
				rate_command = "sudo ethtool -s " + self.config.get_interface() + " advertise 0x03F"
				if rate is not None:
					finalizing_commands.append(rate_command)

				finalizing_commands.append(sysctl_command)
				if self.config.dictionary["tso"] is not None:
					finalizing_commands.append(tso_command)

				if self.config.dictionary["gso"] is not None:
					finalizing_commands.append(gso_command)

				finalizing_commands.append(ethtool)
				finalizing_commands.append(chmod_command)

				if receiver_tcpdump is not None:
					finalizing_commands.append("sudo killall tcpdump")


				for node in [sender_node, receiver_node]:
					n = Command(node, self.config.username, finalizing_commands)
					a.append(n)
					n.start()

				for node in a:
					node.done.wait()
					node.go.set()
					node.finished.wait()
					node.join()

				if receiver_tcpdump is not None:
					receiver_tcpdump.done.wait()
					sender_tcpdump.done.wait()
					
					receiver_tcpdump.go.set()
					sender_tcpdump.go.set()

					receiver_tcpdump.finished.wait()
					sender_tcpdump.finished.wait()

					receiver_tcpdump.join()
					sender_tcpdump.join()

				# get data

				sender_grabber = FileGrabber(sender_node, self.config.dir + sender_log, data_path, self.config.username)
				sender_grabber.start()

				receiver_grabber = FileGrabber(receiver_node, self.config.dir + receiver_log, data_path, self.config.username)
				receiver_grabber.start()

				tcpdump_grabber = None
				if receiver_tcpdump is not None:
					tcpdump_grabber = FileGrabber(receiver_node, "/tmp/" + api + "_receiver_tcpdump.pcap", data_path, self.config.username)
					sender_tcpdump_grabber = FileGrabber(sender_node, "/tmp/" + api + "_sender_tcpdump.pcap", data_path, self.config.username)
					tcpdump_grabber.start()
					sender_tcpdump_grabber.start()
					tcpdump_grabber.join()
					sender_tcpdump_grabber.join()

				pcap_file = "wifu-log.pcap"
				wifu_log = "wifu-end.log"

				if api == "wifu":
					sender_pcap_grabber = FileGrabber(sender_node, self.config.dir + pcap_file, data_path + "sender_wifu_log.pcap", self.config.username)
					sender_pcap_grabber.start()

					receiver_pcap_grabber = FileGrabber(receiver_node, self.config.dir + pcap_file, data_path + "receiver_wifu_log.pcap", self.config.username)
					receiver_pcap_grabber.start()

					receiver_wifu_end_log_grabber = FileGrabber(receiver_node, self.config.dir + wifu_log, data_path + "receiver_wifu_end.log", self.config.username)
					receiver_wifu_end_log_grabber.start()

					sender_wifu_end_log_grabber = FileGrabber(sender_node, self.config.dir + wifu_log, data_path + "sender_wifu_end.log", self.config.username)
					sender_wifu_end_log_grabber.start()

					sender_pcap_grabber.join()
					receiver_pcap_grabber.join()

					receiver_wifu_end_log_grabber.join()
					sender_wifu_end_log_grabber.join()

				sender_grabber.join()
				receiver_grabber.join()
					
				
		return base_path

class PreliminaryGrapher:
	def __init__(self, data_path):
		self.data_path = data_path
		self.graph_path = self.data_path.replace("data", "graphs")
		self.configuration = self.__get_configuration()

		# these must be called after config is setup
		self.receive_files = self.__get_receive_files()
		self.send_files = self.__get_send_files()


	def __get_log_files(self, regex, special=None):
		files = []
		for i in range (0, self.configuration.iterations):
			s = self.data_path + str(i) + "/"
			if special is not None:
				s += special
			
			d = Directory(s)

			temp = d.get_files_re(regex)
			files.extend(temp)
		return files

	def __get_receive_files(self):
		return self.__get_log_files("receiver_(kernel|wifu)\.log")

	def __get_send_files(self):
		return self.__get_log_files("sender_(kernel|wifu)\.log")

	def __get_configuration(self):
		d = Directory(self.data_path)
		files = d.get_files()
		assert len(files) == 1
		return Configuration(files[0], "fake_username")

	def __graph_boxplot(self, data, title, filename):
		fig = plt.figure(figsize=(10, 6))
		fig.canvas.set_window_title('A Boxplot Example')
		ax1 = fig.add_subplot(111)
		plt.subplots_adjust(left=0.075, right=0.95, top=0.9, bottom=0.25)

		bp = plt.boxplot(data, notch=0, sym='+', vert=1, whis=1.5)
		plt.setp(bp['boxes'], color='black')
		plt.setp(bp['whiskers'], color='black')
		plt.setp(bp['fliers'], color='red', marker='+')

		# Add a horizontal grid to the plot, but make it very light in color
		# so we can use it for reading data values but not be distracting
		ax1.yaxis.grid(True, linestyle='-', which='major', color='lightgrey', alpha=0.5)

		# Hide these grid behind plot objects
		ax1.set_axisbelow(True)
		ax1.set_title(title)
		ax1.set_xlabel('Type')
		ax1.set_ylabel('Rate (Mbps)')

		# Set the axes ranges and axes labels
		ax1.set_xlim(0.5, len(data) + 0.5)
#		top = 2000
#		bottom = 0
#		ax1.set_ylim(bottom, top)
		ax1.set_ylim(xmin=0)

		xtickNames = plt.setp(ax1, xticklabels=["Kernel Send", "WiFu Send", "Kernel Receive", "WiFu Receive"])
		#plt.setp(xtickNames, rotation=45, fontsize=8)
		plt.setp(xtickNames, fontsize=10)

		d = Directory(self.graph_path)
		d.make()
		savefig(filename)

	def __graph_boxplot_all(self, data, title, filename):
		fig = plt.figure(figsize=(10, 6))
		fig.canvas.set_window_title('A Boxplot Example')
		ax1 = fig.add_subplot(111)
		plt.subplots_adjust(left=0.075, right=0.95, top=0.9, bottom=0.25)

		bp = plt.boxplot(data, notch=0, sym='+', vert=1, whis=1.5)
		plt.setp(bp['boxes'], color='black')
		plt.setp(bp['whiskers'], color='black')
		plt.setp(bp['fliers'], color='red', marker='+')

		# Add a horizontal grid to the plot, but make it very light in color
		# so we can use it for reading data values but not be distracting
		ax1.yaxis.grid(True, linestyle='-', which='major', color='lightgrey', alpha=0.5)

		# Hide these grid behind plot objects
		ax1.set_axisbelow(True)
		ax1.set_title(title)
		ax1.set_xlabel('Type')
		ax1.set_ylabel('Rate (Mbps)')

		# Set the axes ranges and axes labels
		ax1.set_xlim(0.5, len(data) + 0.5)
#		top = 2000
#		bottom = 0
#		ax1.set_ylim(bottom, top)
		ax1.set_ylim(xmin=0)

		xtickNames = plt.setp(ax1, xticklabels=["Kernel/Loop", "WiFu/Tahoe", "WiFu/Dispatcher After", "WiFu/Dispatcher Before", "WiFu/Inside Unix Socket", "WiFu/Outside Unix Socket", "WiFu/Function", "WiFu/Loop"])
		#plt.setp(xtickNames, rotation=45, fontsize=8)
		plt.setp(xtickNames, fontsize=10)

		d = Directory(self.graph_path)
		d.make()
		savefig(filename)

	def graph_loop_goodputs_scalability(self):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = []
		kernel_send = []
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		#create vectors of goodput
		for file in self.receive_files:
			lines = parser.parse(file)
			wifu = "receiver_wifu.log" in file
			for line in lines:
				if line.startswith("Duration"):
					values = line.split(' ')
					assert len(values) == 10
					duration = float(values[9])
					# assuming one kilo == 1000
					rate = (num_bytes * 8 / 1000000) / (duration / 1000000)
					if wifu:
						wifu_receive.append(rate)
					else:
						kernel_receive.append(rate)

		for file in self.send_files:
			lines = parser.parse(file)
			wifu = "sender_wifu.log" in file
			for line in lines:
				if line.startswith("Duration"):
					values = line.split(' ')
					assert len(values) == 10
					duration = float(values[9])
					# assuming one kilo == 1000
					rate = (num_bytes * 8 / 1000000) / (duration / 1000000)
					if wifu:
						wifu_send.append(rate)
					else:
						kernel_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
	#		print "Looping data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Entire Loop)'
		filename = self.graph_path + 'send_receive_rate_boxplot_loop_scalability.png'
		self.__graph_boxplot(data, title, filename)
		return data

	def graph_loop_goodputs(self):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = []
		kernel_send = []
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		#create vectors of goodput
		for file in self.receive_files:
			lines = parser.parse(file)
			wifu = "receiver_wifu.log" in file
			for line in lines:
				if line.startswith("Duration"):
					values = line.split(' ')
					assert len(values) == 10
					duration = float(values[9])
					# assuming one kilo == 1000
					rate = (num_bytes * 8 / 1000000) / (duration / 1000000)
					if wifu:
						wifu_receive.append(rate)
					else:
						kernel_receive.append(rate)

		for file in self.send_files:
			lines = parser.parse(file)
			wifu = "sender_wifu.log" in file
			for line in lines:
				if line.startswith("Duration"):
					values = line.split(' ')
					assert len(values) == 10
					duration = float(values[9])
					# assuming one kilo == 1000
					rate = (num_bytes * 8 / 1000000) / (duration / 1000000)
					if wifu:
						wifu_send.append(rate)
					else:
						kernel_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
	#		print "Looping data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Entire Loop)'
		filename = self.graph_path + 'send_receive_rate_boxplot_loop.png'
		self.__graph_boxplot(data, title, filename)
		return data

	def graph_function_goodputs(self):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = []
		kernel_send = []
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		#create vectors of goodput
		for file in self.receive_files:
			lines = parser.parse(file)
			wifu = "receiver_wifu.log" in file
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if line.startswith("recv "):
					values = line.split(' ')
					assert len(values) == 4
					start = int(values[1])
					end = int(values[2])
					bytes = int(values[3])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8.0 / 1000000.0) / (total_time / 1000000.0)
			if wifu:
				wifu_receive.append(rate)
			else:
				kernel_receive.append(rate)

		for file in self.send_files:
			lines = parser.parse(file)
			wifu = "sender_wifu.log" in file
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if line.startswith("send "):
					values = line.split(' ')
					assert len(values) == 4
					start = int(values[1])
					end = int(values[2])
					bytes = int(values[3])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes


			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8 / 1000000) / (total_time / 1000000)
			if wifu:
				wifu_send.append(rate)
			else:
				kernel_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
	#		print "Function data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Function Call)'
		filename = self.graph_path + 'send_receive_rate_boxplot_function.png'
		self.__graph_boxplot(data, title, filename)
		return data

	def graph_inside_unix_socket_goodputs(self, function_goodputs_data):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = function_goodputs_data[2]
		kernel_send = function_goodputs_data[0]
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		files = self.__get_log_files("receiver_wifu_end\.log")

		#create vectors of goodput
		for file in files:
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "recv_backend" in line:
					values = line.split(' ')
					assert len(values) == 9
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8.0 / 1000000.0) / (total_time / 1000000.0)
			wifu_receive.append(rate)

		files = self.__get_log_files("sender_wifu_end\.log")

		for file in files:
#			print file
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "send_backend" in line:
					values = line.split(' ')
					assert len(values) == 9
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

#			print total_bytes, " ", num_bytes
			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8 / 1000000) / (total_time / 1000000)
			wifu_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
	#		print "Inside unix socket data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Inside Unix Socket)'
		filename = self.graph_path + 'send_receive_rate_boxplot_inside_unix_socket.png'
		self.__graph_boxplot(data, title, filename)
		return data

	def graph_inside_tahoe_goodputs(self, function_goodputs_data):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = function_goodputs_data[2]
		kernel_send = function_goodputs_data[0]
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		files = self.__get_log_files("receiver_wifu_end\.log")

		#create vectors of goodput
		for file in files:
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "recv_tahoe" in line:
					values = line.split(' ')
					assert len(values) == 9
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8.0 / 1000000.0) / (total_time / 1000000.0)
			wifu_receive.append(rate)

		files = self.__get_log_files("sender_wifu_end\.log")

		for file in files:
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "send_tahoe" in line:
					values = line.split(' ')
					assert len(values) == 9
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

#			print total_bytes, " ", num_bytes
			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8 / 1000000) / (total_time / 1000000)
			wifu_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
	#		print "Inside unix socket data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Inside Tahoe)'
		filename = self.graph_path + 'send_receive_rate_boxplot_inside_tahoe.png'
		self.__graph_boxplot(data, title, filename)
		return data

	def graph_outside_unix_socket_goodputs(self, function_goodputs_data):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = function_goodputs_data[2]
		kernel_send = function_goodputs_data[0]
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		files = self.__get_log_files("receiver_wifu\.log")

		#create vectors of goodput
		for file in files:
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if line.startswith("recv_unix_socket"):
					values = line.split(' ')
					assert len(values) == 4
					start = int(values[1])
					end = int(values[2])
					bytes = int(values[3])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

#			print total_bytes, " ", num_bytes
			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8.0 / 1000000.0) / (total_time / 1000000.0)
			wifu_receive.append(rate)

		files = self.__get_log_files("sender_wifu\.log")

		for file in files:
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if line.startswith("send_unix_socket"):
					values = line.split(' ')
					assert len(values) == 4
					start = int(values[1])
					end = int(values[2])
					bytes = int(values[3])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8 / 1000000) / (total_time / 1000000)
			wifu_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
		#print "Outside Socket Data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Outside Socket Call)'
		filename = self.graph_path + 'send_receive_rate_boxplot_outside_unix_socket.png'
		self.__graph_boxplot(data, title, filename)
		return data

	def graph_before_dispatcher_goodputs(self, function_goodputs_data):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = function_goodputs_data[2]
		kernel_send = function_goodputs_data[0]
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		files = self.__get_log_files("receiver_wifu_end\.log")

		#create vectors of goodput
		for file in files:
#			print file
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "recv_before_dispatcher" in line:
					values = line.split(' ')
					assert len(values) == 9 or len(values) == 11
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

#			print total_bytes, " ", num_bytes
			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8.0 / 1000000.0) / (total_time / 1000000.0)
			wifu_receive.append(rate)

		files = self.__get_log_files("sender_wifu_end\.log")

		for file in files:
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "send_before_dispatcher" in line:
					values = line.split(' ')
					assert len(values) == 9 or len(values) == 11
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

#			print total_bytes, " ", num_bytes
			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8 / 1000000) / (total_time / 1000000)
			wifu_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
	#		print "Inside unix socket data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Dispatcher Before Enqueue)'
		filename = self.graph_path + 'send_receive_rate_boxplot_dispatcher_before_enqueue.png'
		self.__graph_boxplot(data, title, filename)
		return data

	def graph_after_dispatcher_goodputs(self, function_goodputs_data):
		num_bytes = float(self.configuration.dictionary["num"])

		kernel_receive = function_goodputs_data[2]
		kernel_send = function_goodputs_data[0]
		wifu_receive = []
		wifu_send = []

		parser = FileParser()

		files = self.__get_log_files("receiver_wifu_end\.log")

		#create vectors of goodput
		for file in files:
#			print file
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "recv_after_dispatcher" in line:
					values = line.split(' ')
					assert len(values) == 9
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8.0 / 1000000.0) / (total_time / 1000000.0)
			wifu_receive.append(rate)

		files = self.__get_log_files("sender_wifu_end\.log")

		for file in files:
			lines = parser.parse(file)
			total_time = 0.0
			total_bytes = 0
			for line in lines:
				if "send_after_dispatcher" in line:
					values = line.split(' ')
					assert len(values) == 9
					start = int(values[6])
					end = int(values[7])
					bytes = int(values[8])
					duration = end - start
					assert duration > 0
					total_time += duration
					total_bytes += bytes

#			print total_bytes, " ", num_bytes
			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8 / 1000000) / (total_time / 1000000)
			wifu_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
	#		print "Inside unix socket data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Dispatcher After Enqueue)'
		filename = self.graph_path + 'send_receive_rate_boxplot_dispatcher_after_enqueue.png'
		self.__graph_boxplot(data, title, filename)
		return data



	def graph_all_sends(self, loop, function, outside_socket, inside_socket, tahoe, dispatcher, dispatcher_after):
		"""
		Each argument is a 2D array with each element being an array of actual values of goodputs
		Each array within is arranged as follows
		Kernel Send, WiFu Send, Kernel Receive, WiFu Receive
		"""

		# get all of the wifu sends
		index = 1
		data = []

		# We will use the kernel loop results to compare against
		data.append(loop[0])

		# go from innermost to outermost
		data.append(tahoe[index])
		data.append(dispatcher_after[index])
		data.append(dispatcher[index])
		data.append(inside_socket[index])
		data.append(outside_socket[index])
		data.append(function[index])
		data.append(loop[index])

		title = 'Comparison of WiFu and Kernel Sending Rates'
		filename = self.graph_path + 'send_rates_boxplot.png'
		self.__graph_boxplot_all(data, title, filename)
		return data

		

	def graph_all_receives(self, loop, function, outside_socket, inside_socket, tahoe, dispatcher, dispatcher_after):
		"""
		Each argument is a 2D array with each element being an array of actual values of goodputs
		Each array within is arranged as follows
		Kernel Send, WiFu Send, Kernel Receive, WiFu Receive
		"""

		# get all of the wifu sends
		index = 3
		data = []

		# We will use the kernel loop results to compare against
		data.append(loop[2])

		# go from innermost to outermost
		data.append(tahoe[index])
		data.append(dispatcher_after[index])
		data.append(dispatcher[index])
		data.append(inside_socket[index])
		data.append(outside_socket[index])
		data.append(function[index])
		data.append(loop[index])

		title = 'Comparison of WiFu and Kernel Receive Rates'
		filename = self.graph_path + 'receive_rates_boxplot.png'
		self.__graph_boxplot_all(data, title, filename)
		return data

	def graph_all_echos(self):
		# sender is the client
		# for each iteration: open up sender_kernel.log and get out the function duration (should be about the same)
		# I go ahead and record the loop data, but we have removed the loop so it the loop timestamps are taken on the outside of the function ts
		# for each type: for each iteration: open up sender_wifu_end.log and sender_wifu.log and extract out each value
		# we will need to take the send start time and match it with the receive finish time at each level of measurement

		kernel_app = []
		wifu_app = []
		wifu_outside_unix_socket = []

		parser = FileParser()

		for file in self.__get_log_files("sender_(kernel|wifu)\.log", "echo"):
			#print file
			lines = parser.parse(file)
			wifu = "sender_wifu.log" in file
			outside_unix_start = 0
			outside_unix_end = 0
			for line in lines:
				values = line.split(' ')
				# get the function time
				if line.startswith("send "):
					assert len(values) == 4
					start = int(values[1])
					end = int(values[2])
					bytes = int(values[3])
					duration = end - start
					assert duration > 0
					assert bytes == 1000
					if wifu:
						wifu_app.append(duration)
					else:
						kernel_app.append(duration)

				# get the outside socket time
				if line.startswith("send_unix_socket"):
					assert len(values) == 4
					start = int(values[1])
					end = int(values[2])
					bytes = int(values[3])
					duration = end - start
					assert duration > 0
					assert bytes == 1000
					outside_unix_start = start
				if line.startswith("recv_unix_socket"):
					assert len(values) == 4
					start = int(values[1])
					end = int(values[2])
					bytes = int(values[3])
					duration = end - start
					assert duration > 0
					assert bytes == 1000
					outside_unix_end = end
			duration = outside_unix_end - outside_unix_start
			if wifu:
				wifu_outside_unix_socket.append(duration)


		# This whole for loop assumes that the receive is logged first!
		inside_socket_arr = []
		dispatcher_before_arr = []
		dispatcher_after_arr = []
		tahoe_arr =[]

		for file in self.__get_log_files("sender_wifu_end.log", "echo"):
			print file
			lines = parser.parse(file)
			inside_socket = 0
			dispatcher_before = 0
			dispatcher_after = 0
			tahoe = 0
			for line in lines:
				values = line.split(' ')
				if len(values) == 8:
					assert int(values[8]) == 1000

				if "recv_tahoe" in line:
					tahoe = int(values[7])
				elif "send_tahoe" in line:
					tahoe -= int(values[6])

				elif "recv_backend" in line:
					inside_socket = int(values[7])
				elif "send_backend" in line:
					inside_socket -= int(values[6])

				elif "recv_before_dispatcher" in line:
					dispatcher_before = int(values[7])
				elif "send_before_dispatcher" in line:
					dispatcher_before -= int(values[6])

				elif "recv_after_dispatcher" in line:
					dispatcher_after = int(values[7])
				elif "send_after_dispatcher" in line:
					dispatcher_after -= int(values[6])
					
			inside_socket_arr.append(inside_socket)
			dispatcher_before_arr.append(dispatcher_before)
			dispatcher_after_arr.append(dispatcher_after)
			tahoe_arr.append(tahoe)
			
		data = [kernel_app, tahoe_arr, dispatcher_after_arr, dispatcher_before_arr, inside_socket_arr, wifu_outside_unix_socket, wifu_app, wifu_app]
		print "Function data: ", data
		s = Stats()
		print s.median(kernel_app)
		print s.median(tahoe_arr)
		print s.median(dispatcher_after_arr)
		print s.median(dispatcher_before_arr)
		print s.median(inside_socket_arr)
		print s.median(wifu_outside_unix_socket)
		print s.median(wifu_app)
		title = 'Echo Server Latency'
		filename = self.graph_path + 'echo_server_latency.png'
		self.__graph_boxplot_all(data, title, filename)
		return data

	def jain(self, array):
		numerator = 0
		denominator = 0
		for i in range(0, len(array)):
			numerator += array[i]
			denominator += math.pow(array[i], 2)

		numerator = math.pow(numerator, 2)
		denominator = denominator * len(array)
		return numerator / denominator

	def graph_instantaneous(self):
		if not self.configuration.dictionary.has_key("tcpdump"):
			return

		if self.configuration.dictionary["tcpdump"] != "on":
			return

		# convert pcap file to something instantaneous.py can use
		files = self.__get_log_files("(kernel|wifu)_receiver_tcpdump\.pcap")
		print files

		files_string = ""
		for file in files:
			files_string = file + " " + files_string

		command = "/home/rbuck/NetBeansProjects/wireshark/wireshark-1.2.7/mergecap -a " + files_string + "-w merged.pcap"
		os.system(command)

		files.append("merged.pcap")

		for file in files:
			print file
			# Must use our modified version of tshark
			command = "/home/rbuck/NetBeansProjects/wireshark/wireshark-1.2.7/tshark -r " + file + " -R \"tcp.port == 5000\" > temp.txt"
			os.system(command)
			command = "sed -i 's/ ->//g' temp.txt"
			os.system(command)

			d = Directory(self.graph_path)
			d.make()
			
			api = "merged"
			if "kernel" in file:
				api = "kernel"
			elif "wifu" in file:
				api = "wifu"
			
			outfile = self.graph_path + "instantaneous_" + api + ".eps"
			command = "python intantaneous.py -f temp.txt -o" + outfile
			os.system(command)
			os.system("rm temp.txt")

		# graph all files together
		

			


	def graph(self):
		loop_data = self.graph_loop_goodputs()
		scale_data = self.graph_loop_goodputs_scalability()
#		function_data = self.graph_function_goodputs()
#		outside_unix_socket_data = self.graph_outside_unix_socket_goodputs(function_data)
#		inside_unix_socket_data = self.graph_inside_unix_socket_goodputs(function_data)
#		inside_tahoe_data = self.graph_inside_tahoe_goodputs(function_data)
#		dispatcher_data = self.graph_before_dispatcher_goodputs(function_data)
#		dispatcher_data_after = self.graph_after_dispatcher_goodputs(function_data)
#
#		self.graph_all_sends(loop_data, function_data, outside_unix_socket_data, inside_unix_socket_data, inside_tahoe_data, dispatcher_data, dispatcher_data_after)
#		self.graph_all_receives(loop_data, function_data, outside_unix_socket_data, inside_unix_socket_data, inside_tahoe_data, dispatcher_data, dispatcher_data_after)

#		self.graph_all_echos()


		self.graph_instantaneous()
		
		

		s = Stats()
		print "Stats of 25th, 50th, and 75th percentiles in Mbps...\n"
		print "Entire loop:"
		print "Kernel Send:\t", s.get_25th_percentile(loop_data[0]), "\t", s.median(loop_data[0]), "\t", s.get_75th_percentile(loop_data[0])
		print "WiFu Send:\t", s.get_25th_percentile(loop_data[1]), "\t", s.median(loop_data[1]), "\t", s.get_75th_percentile(loop_data[1])
		print "WiFu / Kernel Send Ratio for median value:\t", s.median(loop_data[1]) / s.median(loop_data[0])
		print "Kernel Receive:\t", s.get_25th_percentile(loop_data[2]), "\t", s.median(loop_data[2]), "\t", s.get_75th_percentile(loop_data[2])
		print "WiFu Receive:\t", s.get_25th_percentile(loop_data[3]), "\t", s.median(loop_data[3]), "\t", s.get_75th_percentile(loop_data[3])
		print "WiFu / Kernel Receive Ratio for median value:\t", s.median(loop_data[3]) / s.median(loop_data[2])

		loop_data[3].sort()
		l = len(loop_data[3])
		print "WiFu Receive Max: ", loop_data[3][l-1]


		wifu_receiver_scalability = scale_data[3]
		kernel_receiver_scalability = scale_data[2]
		print len(wifu_receiver_scalability)
		print len(kernel_receiver_scalability)
		# compute jain's fairness
		print "Jain's Fairness (WiFu)  : ", self.jain(wifu_receiver_scalability)
		print "Jain's Fairness (Kernel): ", self.jain(kernel_receiver_scalability)
		

		

#		print ""
#		print "Function Call:"
#		print "Kernel Send:\t", s.get_25th_percentile(function_data[0]), "\t", s.median(function_data[0]), "\t", s.get_75th_percentile(function_data[0])
#		print "WiFu Send:\t", s.get_25th_percentile(function_data[1]), "\t", s.median(function_data[1]), "\t", s.get_75th_percentile(function_data[1])
#		print "WiFu / Kernel Send Ratio for median value:\t", s.median(function_data[1]) / s.median(function_data[0])
#		print "Kernel Receive:\t", s.get_25th_percentile(function_data[2]), "\t", s.median(function_data[2]), "\t", s.get_75th_percentile(function_data[2])
#		print "WiFu Receive:\t", s.get_25th_percentile(function_data[3]), "\t", s.median(function_data[3]), "\t", s.get_75th_percentile(function_data[3])
#		print "WiFu / Kernel Receive Ratio for median value:\t", s.median(function_data[3]) / s.median(function_data[2])
#
#		print ""
#		print "Outside Unix Socket Call:"
#		print "Kernel Send:\t", s.get_25th_percentile(outside_unix_socket_data[0]), "\t", s.median(outside_unix_socket_data[0]), "\t", s.get_75th_percentile(outside_unix_socket_data[0])
#		print "WiFu Send:\t", s.get_25th_percentile(outside_unix_socket_data[1]), "\t", s.median(outside_unix_socket_data[1]), "\t", s.get_75th_percentile(outside_unix_socket_data[1])
#		print "WiFu / Kernel Send Ratio for median value:\t", s.median(outside_unix_socket_data[1]) / s.median(outside_unix_socket_data[0])
#		print "Kernel Receive:\t", s.get_25th_percentile(outside_unix_socket_data[2]), "\t", s.median(outside_unix_socket_data[2]), "\t", s.get_75th_percentile(outside_unix_socket_data[2])
#		print "WiFu Receive:\t", s.get_25th_percentile(outside_unix_socket_data[3]), "\t", s.median(outside_unix_socket_data[3]), "\t", s.get_75th_percentile(outside_unix_socket_data[3])
#		print "WiFu / Kernel Receive Ratio for median value:\t", s.median(outside_unix_socket_data[3]) / s.median(outside_unix_socket_data[2])
#
#		print ""
#		print "Inside Unix Socket Call:"
#		print "Kernel Send:\t", s.get_25th_percentile(inside_unix_socket_data[0]), "\t", s.median(inside_unix_socket_data[0]), "\t", s.get_75th_percentile(inside_unix_socket_data[0])
#		print "WiFu Send:\t", s.get_25th_percentile(inside_unix_socket_data[1]), "\t", s.median(inside_unix_socket_data[1]), "\t", s.get_75th_percentile(inside_unix_socket_data[1])
#		print "WiFu / Kernel Send Ratio for median value:\t", s.median(inside_unix_socket_data[1]) / s.median(inside_unix_socket_data[0])
#		print "Kernel Receive:\t", s.get_25th_percentile(inside_unix_socket_data[2]), "\t", s.median(inside_unix_socket_data[2]), "\t", s.get_75th_percentile(inside_unix_socket_data[2])
#		print "WiFu Receive:\t", s.get_25th_percentile(inside_unix_socket_data[3]), "\t", s.median(inside_unix_socket_data[3]), "\t", s.get_75th_percentile(inside_unix_socket_data[3])
#		print "WiFu / Kernel Receive Ratio for median value:\t", s.median(inside_unix_socket_data[3]) / s.median(inside_unix_socket_data[2])
#
#		print ""
#		print "Dispatcher Before Enqueue:"
#		print "Kernel Send:\t", s.get_25th_percentile(dispatcher_data[0]), "\t", s.median(dispatcher_data[0]), "\t", s.get_75th_percentile(dispatcher_data[0])
#		print "WiFu Send:\t", s.get_25th_percentile(dispatcher_data[1]), "\t", s.median(dispatcher_data[1]), "\t", s.get_75th_percentile(dispatcher_data[1])
#		print "WiFu / Kernel Send Ratio for median value:\t", s.median(dispatcher_data[1]) / s.median(dispatcher_data[0])
#		print "Kernel Receive:\t", s.get_25th_percentile(dispatcher_data[2]), "\t", s.median(dispatcher_data[2]), "\t", s.get_75th_percentile(dispatcher_data[2])
#		print "WiFu Receive:\t", s.get_25th_percentile(dispatcher_data[3]), "\t", s.median(dispatcher_data[3]), "\t", s.get_75th_percentile(dispatcher_data[3])
#		print "WiFu / Kernel Receive Ratio for median value:\t", s.median(dispatcher_data[3]) / s.median(dispatcher_data[2])
#
#		print ""
#		print "Dispatcher After Enqueue:"
#		print "Kernel Send:\t", s.get_25th_percentile(dispatcher_data_after[0]), "\t", s.median(dispatcher_data_after[0]), "\t", s.get_75th_percentile(dispatcher_data_after[0])
#		print "WiFu Send:\t", s.get_25th_percentile(dispatcher_data_after[1]), "\t", s.median(dispatcher_data_after[1]), "\t", s.get_75th_percentile(dispatcher_data_after[1])
#		print "WiFu / Kernel Send Ratio for median value:\t", s.median(dispatcher_data_after[1]) / s.median(dispatcher_data_after[0])
#		print "Kernel Receive:\t", s.get_25th_percentile(dispatcher_data_after[2]), "\t", s.median(dispatcher_data_after[2]), "\t", s.get_75th_percentile(dispatcher_data_after[2])
#		print "WiFu Receive:\t", s.get_25th_percentile(dispatcher_data_after[3]), "\t", s.median(dispatcher_data_after[3]), "\t", s.get_75th_percentile(dispatcher_data_after[3])
#		print "WiFu / Kernel Receive Ratio for median value:\t", s.median(dispatcher_data_after[3]) / s.median(dispatcher_data_after[2])
#
#		print ""
#		print "Inside Tahoe:"
#		print "Kernel Send:\t", s.get_25th_percentile(inside_tahoe_data[0]), "\t", s.median(inside_tahoe_data[0]), "\t", s.get_75th_percentile(inside_tahoe_data[0])
#		print "WiFu Send:\t", s.get_25th_percentile(inside_tahoe_data[1]), "\t", s.median(inside_tahoe_data[1]), "\t", s.get_75th_percentile(inside_tahoe_data[1])
#		print "WiFu / Kernel Send Ratio for median value:\t", s.median(inside_tahoe_data[1]) / s.median(inside_tahoe_data[0])
#		print "Kernel Receive:\t", s.get_25th_percentile(inside_tahoe_data[2]), "\t", s.median(inside_tahoe_data[2]), "\t", s.get_75th_percentile(inside_tahoe_data[2])
#		print "WiFu Receive:\t", s.get_25th_percentile(inside_tahoe_data[3]), "\t", s.median(inside_tahoe_data[3]), "\t", s.get_75th_percentile(inside_tahoe_data[3])
#		print "WiFu / Kernel Receive Ratio for median value:\t", s.median(inside_tahoe_data[3]) / s.median(inside_tahoe_data[2])

		return loop_data



class MultipleGrapher:
	def __init__(self, dirs, data):
		self.dirs = []

		for dir in dirs:
			self.dirs.append(dir.replace("data", "graphs"))

		self.data = data

	def graph(self):
		print "MultipleGrapher::graph()"
		# data is a 3-d array
		assert len(self.dirs) == 3

		data = []
		for item in self.data:
			# item represents one of the different types (e.g. 10, 100, 1000 Mbps)
			# kernel is in index 2, wifu in 2
			data.append(item[2])
			data.append(item[3])

		assert len(data) == 6

		fig = plt.figure(figsize=(10, 6))
		fig.canvas.set_window_title('A Boxplot Example')
		ax1 = fig.add_subplot(111)
		ax1.set_yscale('log')
		#plt.subplots_adjust(left=0.075, right=0.95, top=0.9, bottom=0.25)

		bp = plt.boxplot(data, notch=0, sym='+', vert=1, whis=1.5)
		plt.setp(bp['boxes'], color='black')
		plt.setp(bp['whiskers'], color='black')
		plt.setp(bp['medians'], color='black')
		plt.setp(bp['fliers'], color='black', marker='+')

		# Add a horizontal grid to the plot, but make it very light in color
		# so we can use it for reading data values but not be distracting
		ax1.yaxis.grid(True, linestyle='-', which='major', color='lightgrey', alpha=0.5)

		# Hide these grid behind plot objects
		ax1.set_axisbelow(True)
		#title = 'Comparison WiFu and Kernel Receiving Rates'
		#ax1.set_title(title)
		#ax1.set_xlabel('Type')
		ax1.set_ylabel('Goodput (Mbps)')

		# Set the axes ranges and axes labels
		ax1.set_xlim(0.5, len(data) + 0.5)
#		top = 2000
#		bottom = 0
#		ax1.set_ylim(bottom, top)
		ax1.set_ylim(xmin=0)

		xtickNames = plt.setp(ax1, xticklabels=["Kernel 10 Mbps", "WiFu 10 Mbps", "Kernel 100 Mbps", "WiFu 100 Mbps", "Kernel 1000 Mbps", "WiFu 1000 Mbps"])
#		xtickNames = plt.setp(ax1, xticklabels=["Kernel 1-hop", "WiFu 1-hop", "Kernel 2-hops", "WiFu 2-hops", "Kernel 3-hops", "WiFu 3-hops"])
		#plt.setp(xtickNames, rotation=45, fontsize=8)
		plt.setp(xtickNames, fontsize=10)

		print "Saving graph"
		# save in all directories

		for dir in self.dirs:
			d = Directory(dir)
			d.make()
			filename = dir + 'multiple_compare_wired_.eps'
#			filename = dir + 'multiple_compare_wireless.eps'
			savefig(filename, format="eps")
		

if __name__ == "__main__":

	start = time.time()

	parser = optparse.OptionParser()
	parser.add_option("-u", "--username", dest="username", help="Username used to copy files to nodes.  Must also be in the sudoers file on the nodes.")
	parser.add_option("-c", "--config", dest="config", help="Configuration file containing necessary information to run preliminary executables.")
	parser.add_option("-g", "--graph", dest="graph", help="Only do graphing of the specified path.")
	

	#TODO: use argparse instead (need python 2.7) as it will allow for variable length args with nargs='?' (nargs='*')

	(options, args) = parser.parse_args()
	username = options.username
	config = options.config
	graph = options.graph

	graph_path = graph

	if graph is None:
		c = Configuration(config, username)
		e = ExecutableCopier(c)
		e.copy_all()

		manager = ExecutableManager(c)
		graph_path = manager.execute()
#		manager.execute_echo()

		end = time.time()
		print "Duration (seconds) to do " + str(c.iterations) + " run(s): " + str(end - start)

	print "Graphing..."

	# check for multiple directories
	dirs = graph_path.split(" ")

	loop_datas = []
	for dir in dirs:
		grapher = PreliminaryGrapher(dir)
		data = grapher.graph()
		loop_datas.append(data)

	if len(dirs) == 3:
		mg = MultipleGrapher(dirs, loop_datas)
		mg.graph()
	
	
	print "All done."
