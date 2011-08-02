
import pxssh
import threading
from FileOps import *
import time
import sys

# To change this template, choose Tools | Templates
# and open the template in the editor.


# Runs one command on the remote machine specified
__author__ = "rbuck"
__date__ = "$Mar 11, 2010 2:38:37 PM$"

class Command(threading.Thread):
    """
    Run a single (or list of) command(s) on a remote host
    If you specify a list for the commands argument, after each command
    is run this script will wait for the prompt to come before executing the
    next command.  If you expect anything other than the default you will
    want to override the doCommand method
    """
    def __init__(self, host, username, commands):
        """
        * host: the host name of the remote machine
        * username: the username you use to login to the remote machine
        * commands: the command(s) to run
        """
        threading.Thread.__init__(self)
        self.host = host
        self.username = username
        self.commands = commands
        self.done = threading.Event()
        self.go = threading.Event()
        self.running = threading.Event()
        self.finished = threading.Event()
        self.child = False

    def login(self):
        """ Login to the remote host. """
        # login
        print "[%s] Connecting ..." % self.host
        self.child = pxssh.pxssh()
        OK = False
        tries = 0
        while not OK and tries < 10:
            try:
                tries += 1
                self.child.login(self.host, self.username, login_timeout=6000)
                OK = True
            except:
                print "Could not log in on attempt %d, t=%f" % (tries, time.time())
                print "Details of error: %s" % sys.exc_info()[0]
                self.child = pxssh.pxssh()
        if not OK:
            print "Can't login to %s" % (self.host)
            return

    def doCommand(self):
        """ Run the command(s).  Signal that
        this host is done and wait for a signal to continue. """

        # Checks and sets the appropriate type of self.commands
        # self.commands can either be a string or a list of string
        if isinstance(self.commands, str):
            temp = self.commands
            self.commands = []
            self.commands.append(temp)

        # run each command
        for command in self.commands:
            print "[%s] Running %s" % (self.host, command)
            self.child.sendline('%s' % command)
            self.running.set()
            self.child.prompt(timeout=None)

        # tell the caller we are done
        self.done.set()
        # wait for signal to finish
        self.go.wait()

        self.child.close()
        print '[%s] Done' % (self.host)
        self.finished.set()
        

    def run(self):
        """ Login, then execute the command(s) """
        self.login()
        self.doCommand()



if __name__ == "__main__":
    print "Hello World"

    # Example (runs a list of simple commands on a few mesh nodes):
    # You must have ssh keys set up for this to work
    # You must be in the sudoers list for this to work
    
    nodes = []
    username = 'rbuck'

    what = [1]

    for i in range(1,14):
        # This also works (either pass in a str or a list:
        # rc = Command('mesh%s' %(i), username, 'ls')
        destNode = 'mesh%s' %(i)

        pusher = FilePusher(destNode, '/tmp/', '/home/rbuck/Desktop/authorized_keys', 'rbuck')
        pusher.push()

        commands = [ 'mkdir /home/self',
                    'chown self /home/self',
                    'mkdir /home/self/.ssh',
                    'chown self /home/self/.ssh',
                    'cp /tmp/authorized_keys /home/self/.ssh/',
                    'chown self /home/self/.ssh/authorized_keys',
                    'rm -rf /tmp/authorized_keys']




        rc = Command(destNode, username, commands)
        nodes.append(rc)
        rc.start()

        # Not needed in for base class as we don't have to wait for certian processes
        # to finish before starting others; however, it may be necessary to wait
        # for a process to signal that he is done before proceeding
        # For example, making sure a server has started before starting the client

#        rc.done.wait()        

        rc.go.set()


    for node in nodes:
        node.join()

