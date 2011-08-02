#! /usr/bin/python

# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "rbuck"
__date__ = "$Aug 2, 2011 9:33:17 AM$"

import time

from FileOps import *
from RemoteCommand import Command
from fileparser import FileParser
import optparse

class Configuration():
	def __init__(self, file, username):
		self.file = file
		self.username = username
		self.dictionary = {}
		self.parse()
		self.dir = "/tmp/"

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

		self.api = "api"

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

	def copy_all(self):
		threads = []

		if(self.config.dictionary[self.api] == "wifu"):
			threads.extend(self.copy_wifu())
			
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
		self.api = "api"
		self.protocol = "protocol"

		# sender options
		self.num = "num"
		self.sendingChunk = "sendingChunk"

		# receiver options
		self.receivingChunk = "receivingChunk"
		
	def get_wifu_command(self):
		command = "sudo ./wifu-end"
		
		if self.loggerThreshold in self.config.dictionary:
			command += " --logger_threshold " + self.config.dictionary[self.loggerThreshold]
		if self.loggerTimeout in self.config.dictionary:
			command += " --logger_timeout " + self.config.dictionary[self.loggerTimeout]
		if self.network in self.config.dictionary:
			command += " --network " + self.config.dictionary[self.network]

		return command

	def start_wifu(self):

		if(self.config.dictionary[self.api] != "wifu"):
			return;

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
		if(self.config.dictionary[self.api] != "wifu"):
			return;

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


	def get_receiver_command(self, outfile=None):
		receiver_command = "sudo ./simple-tcp-receiver"

		if self.receiverAddress in self.config.dictionary:
			receiver_command += " --address " + self.config.dictionary[self.receiverAddress]
		if self.port in self.config.dictionary:
			receiver_command += " --port " + self.config.dictionary[self.port]
		if self.api in self.config.dictionary:
			receiver_command += " --api " + self.config.dictionary[self.api]
		if self.protocol in self.config.dictionary:
			receiver_command += " --protocol " + self.config.dictionary[self.protocol]
		if self.receivingChunk in self.config.dictionary:
			receiver_command += " --chunk " + self.config.dictionary[self.receivingChunk]

		if outfile:
			receiver_command += " > " + outfile
			
		return receiver_command

	def get_sender_command(self, outfile=None):
		sender_command = "sudo ./simple-tcp-sender"

		if self.receiverAddress in self.config.dictionary:
			sender_command += " --destination " + self.config.dictionary[self.receiverAddress]
		if self.port in self.config.dictionary:
			sender_command += " --port " + self.config.dictionary[self.port]
		if self.api in self.config.dictionary:
			sender_command += " --api " + self.config.dictionary[self.api]
		if self.protocol in self.config.dictionary:
			sender_command += " --protocol " + self.config.dictionary[self.protocol]
		if self.sendingChunk in self.config.dictionary:
			sender_command += " --chunk " + self.config.dictionary[self.sendingChunk]
		if self.num in self.config.dictionary:
			sender_command += " --num " + self.config.dictionary[self.num]

		if outfile:
			sender_command += " > " + outfile

		return sender_command
	
	

	def execute(self):
		self.start_wifu()

		sender_node = self.config.get_sender_node()
		receiver_node = self.config.get_receiver_node()
		chdir_command = "cd " + self.config.dir

		nodes = []


		# start up the receiver
		receiver_log = "recever.log"
		receiver_command = self.get_receiver_command(receiver_log)
		receiver_commands = []
		receiver_commands.append(chdir_command)
		receiver_commands.append(receiver_command)



		receiver = Command(receiver_node, self.config.username, receiver_commands)
		nodes.append(receiver)
		receiver.start()
		receiver.running.wait()
		time.sleep(1)

		# start up the sender
		sender_log = "sender.log"
		sender_command = self.get_sender_command(sender_log)
		sender_commands = []
		sender_commands.append(chdir_command)
		sender_commands.append(sender_command)


		sender = Command(sender_node, self.config.username, sender_commands)
		nodes.append(sender)
		sender.start()

		for node in nodes:
			node.done.wait()
			node.go.set()
			node.finished.wait()
			node.join()

		self.kill_wifu()

		# get data
		data_path = "data/"
		sender_grabber = FileGrabber(sender_node, self.config.dir + sender_log, data_path, self.config.username)
		sender_grabber.start()

		receiver_grabber = FileGrabber(receiver_node, self.config.dir + receiver_log, data_path, self.config.username)
		receiver_grabber.start()

		sender_grabber.join()
		receiver_grabber.join()

		sender_remover = FileRemover(sender_node, self.config.dir + sender_log, self.config.username)
		sender_remover.remove()

		receiver_remover = FileRemover(receiver_node, self.config.dir + receiver_log, self.config.username)
		receiver_remover.remove()


if __name__ == "__main__":

	parser = optparse.OptionParser()
	parser.add_option("-u", "--username", dest="username", help="Username used to copy files to nodes.  Must also be in the sudoers file on the nodes.")
	parser.add_option("-c", "--config", dest="config", help="Configuration file containing necessary information to run preliminary executables.", metavar="FILE", default="config/ilab1.conf")

	(options, args) = parser.parse_args()
	username = options.username
	config = options.config

	c = Configuration(config, username)
	e = ExecutableCopier(c)
	e.copy_all()

	manager = ExecutableManager(c)
	manager.execute()