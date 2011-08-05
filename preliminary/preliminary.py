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

	

class ExecutableCopier():
	def __init__(self, configuration):
		self.config = configuration
		self.wifu = "../bin/wifu-end"
		self.sender_application = "../bin/simple-tcp-sender"
		self.receiver_application = "../bin/simple-tcp-receiver"

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

		for thread in threads:
			thread.join()

class ExecutableManager():
	def __init__(self, configuration):
		self.config = configuration

		self.receiverAddress = "receiverAddress"

		# wifu options
		self.loggerThreshold = "loggerThreshold"
		self.loggerTimeout = "loggerTimeout"
		self.network = "network"

		# common options between sender and receiver
		self.port = "port"
		self.wifu = "wifu"
		self.kernel = "kernel"

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


	def get_receiver_command(self, api, outfile=None):
		receiver_command = "sudo nice -n -20 ./simple-tcp-receiver"

		if self.receiverAddress in self.config.dictionary:
			receiver_command += " --address " + self.config.dictionary[self.receiverAddress]
		if self.port in self.config.dictionary:
			receiver_command += " --port " + self.config.dictionary[self.port]
		if self.receivingChunk in self.config.dictionary:
			receiver_command += " --chunk " + self.config.dictionary[self.receivingChunk]

		if api == self.kernel and self.kernel in self.config.dictionary:
			receiver_command += " --protocol " + self.config.dictionary[self.kernel]
		elif api == self.wifu and self.wifu in self.config.dictionary:
			receiver_command += " --protocol " + self.config.dictionary[self.wifu]

		receiver_command += " --api " + api

		if outfile:
			receiver_command += " > " + outfile
			
		return receiver_command

	def get_sender_command(self, api, outfile=None):
		sender_command = "sudo nice -n -20 ./simple-tcp-sender"

		if self.receiverAddress in self.config.dictionary:
			sender_command += " --destination " + self.config.dictionary[self.receiverAddress]
		if self.port in self.config.dictionary:
			sender_command += " --port " + self.config.dictionary[self.port]
		if self.sendingChunk in self.config.dictionary:
			sender_command += " --chunk " + self.config.dictionary[self.sendingChunk]
		if self.num in self.config.dictionary:
			sender_command += " --num " + self.config.dictionary[self.num]

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

				# start up the receiver
				receiver_log = "receiver_" + api +".log"
				receiver_command = self.get_receiver_command(api, receiver_log)
				receiver_commands = []
				receiver_commands.append(chdir_command)
				receiver_commands.append(sysctl_command)
				receiver_commands.append(receiver_command)



				receiver = Command(receiver_node, self.config.username, receiver_commands)
				nodes.append(receiver)
				receiver.start()
				receiver.running.wait()
				time.sleep(1)

				# start up the sender
				sender_log = "sender_" + api + ".log"
				sender_command = self.get_sender_command(api, sender_log)
				sender_commands = []
				sender_commands.append(chdir_command)
				sender_commands.append(sysctl_command)
				sender_commands.append(sender_command)


				sender = Command(sender_node, self.config.username, sender_commands)
				nodes.append(sender)
				sender.start()

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
				chmod_command = "sudo chmod o+r /tmp/wifu-end.log"
				finalizing_commands.append(sysctl_command)
				finalizing_commands.append(chmod_command)

				for node in [sender_node, receiver_node]:
					n = Command(node, self.config.username, finalizing_commands)
					a.append(n)
					n.start()

				for node in a:
					node.done.wait()
					node.go.set()
					node.finished.wait()
					node.join()

				# get data

				sender_grabber = FileGrabber(sender_node, self.config.dir + sender_log, data_path, self.config.username)
				sender_grabber.start()

				receiver_grabber = FileGrabber(receiver_node, self.config.dir + receiver_log, data_path, self.config.username)
				receiver_grabber.start()

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


	def __get_log_files(self, regex):
		files =[]
		for i in range (0, self.configuration.iterations):
			d = Directory(self.data_path + str(i) + "/")
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
		fig = plt.figure(figsize=(10,6))
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
		print "Looping data: ", data
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
		print "Function data: ", data
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


			assert total_bytes == num_bytes
			# assuming one kilo == 1000
			rate = (num_bytes * 8 / 1000000) / (total_time / 1000000)
			wifu_send.append(rate)

		data = [kernel_send, wifu_send, kernel_receive, wifu_receive]
		print "Inside unix socket data: ", data
		title = 'Comparison WiFu and Kernel Sending and Receiving Rates (Inside Unix Socket)'
		filename = self.graph_path + 'send_receive_rate_boxplot_inside_unix_socket.png'
		self.__graph_boxplot(data, title, filename)

	def graph(self):
		self.graph_loop_goodputs()
		data = self.graph_function_goodputs()
		self.graph_inside_unix_socket_goodputs(data)


if __name__ == "__main__":

	start = time.time()

	parser = optparse.OptionParser()
	parser.add_option("-u", "--username", dest="username", help="Username used to copy files to nodes.  Must also be in the sudoers file on the nodes.")
	parser.add_option("-c", "--config", dest="config", help="Configuration file containing necessary information to run preliminary executables.")

	#TODO: use argparse instead (need python 2.7) as it will allow for variable length args with nargs='?' (nargs='*')

	(options, args) = parser.parse_args()
	username = options.username
	config = options.config

	c = Configuration(config, username)
	e = ExecutableCopier(c)
	e.copy_all()

	manager = ExecutableManager(c)
	path = manager.execute()

	end = time.time()

	print "Duration (seconds) to do " + str(c.iterations) + " run(s): " + str(end - start)

	print "Graphing..."
	grapher = PreliminaryGrapher(path)
	grapher.graph()
	
	print "All done."