# To change this template, choose Tools | Templates
# and open the template in the editor.

__author__ = "rbuck"
__date__ = "$Mar 12, 2010 11:04:06 AM$"


import subprocess
from RemoteCommand import *
import threading
import os
import sys
sys.path.append("/home/ilab/pylib")
from directory import *

class FileGrabber(threading.Thread):
	"""
	Grabs a file from a remote machine
	"""
	def __init__(self, srcNode, srcPath, destPath, username):
		threading.Thread.__init__(self)
		self.srcNode = srcNode
		self.srcPath = srcPath
		self.destPath = destPath
		self.username = username

	def grab(self):
		print "Grabbing file %s from [%s]" %(self.srcPath, self.srcNode)
		command = 'scp'
		src = '%s@%s:%s' % (self.username, self.srcNode, self.srcPath)
		dest = self.destPath
		#self.__mkdir(self.destPath)
		subprocess.call([command,src,dest])

	def run(self):
		self.grab()

	def __mkdir(self, path):
		maker = Directory(path)
		maker.make()

	

class FilePusher(threading.Thread):
    """
    Pushes a file to a remote machine
    """
    def __init__(self, destNode, destPath, file, username):
        threading.Thread.__init__(self)
        self.destNode = destNode
        self.file = file
        self.destPath = destPath
        self.username = username

    def push(self):
        dest = "%s:%s" %(self.destNode, self.destPath)
        print "Pushing file %s to [%s]" %(self.file, dest)
        command = 'scp'
        dest = '%s@%s' % (self.username, dest)
        subprocess.call([command,self.file,dest])

    def run(self):
        self.push()


class FileRemover:
    """
    Removes a file from a remote machine
    """
    def __init__(self, node, path, username):
        print "Init"
        self.node = node
        self.path = path
        self.username = username

    def remove(self):
        print "Removing file %s from [%s]" %(self.path, self.node)
        command = 'rm -rf %s' %(self.path)
        c = Command(self.node,self.username,command)
        c.start()
        c.go.set()
        c.join()


if __name__ == "__main__":
    print "Hello World"
    node = 'mesh10'
    path = '/home/rbuck/out.txt'
    username = 'rbuck'
    
    fg = FileGrabber(node,path,'/home/rbuck/Desktop/bobzilla/',username)
    fg.grab()

    fr = FileRemover(node,path,username)
    fr.remove()
    
