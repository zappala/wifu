#!/usr/bin/env python

"""
Controls UDP blaster app
"""

import optparse
import socket
import threading
import time

class Blaster:
    """
    Tells the remote destination to listen, then tells the remote
    blaster to start sending.  Records the results and prints to
    standard output using the form:

      <packets sent> <packets received> <time to send> <time to receive>
    """
    def __init__(self,source,sourceport,dest,destport):
        """
        source = host name of the remote blaster (message socket)
        sourceport = port of the remote blaster (message socket)
        dest = host name of the remote listener (message socket)
        port = port of the remote listener (message socket)
        """
        self.source = source
	self.sourceport = sourceport
        self.dest = dest
	self.destport = destport
        # open sockets
	self.open_sockets()

    def open_sockets(self):
        """ Create message sockets to talk to blaster and listener """
        try:
            self.sourcesock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.destsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        except socket.error, (code,message):
            self.sourcesock = None
            self.destsock = None

    def blast(self,ip,port,size,rate,duration):
        """ start the blast and print the results """
        if not self.sourcesock or not self.destsock:
            return "Sockets not open"
        # tell listener to start
        # print "starting listener at %s %d" % (self.dest,self.destport)
        self.destsock.sendto("start",(self.dest,self.destport))
        time.sleep(1)
        # tell blaster to start
        # print "starting blaster at %s %d with %s:%d size %d rate %d duration %d" % (self.source,self.sourceport,ip,port,size,rate,duration)
        self.sourcesock.sendto("start %s %d %d %d %d" % (ip,port,
                                                         size,rate,duration),
                               (self.source,self.sourceport))
        # get results
        t1 = time.time()
        (b,address) = self.sourcesock.recvfrom(1000)
        t2 = time.time()
        (l,address) = self.destsock.recvfrom(1000)
        t3 = time.time()
        if b == "":
            return "empty response from source"
        if l == "":
            return "empty response from destination"
        # return results
        return "%s %s %f %f" % (b,l,t2-t1,t3-t1)

    def quit(self):
        """ tell blaster and listener to quit """
        self.sourcesock.sendto("quit",(self.dest,self.destport))
        self.destsock.sendto("quit",(self.source,self.sourceport))

class Main:
    """ Run a series of blasters """
    def __init__(self):
        self.parse_options()

    def parse_options(self):
        parser = optparse.OptionParser(usage = "%prog [options]",
                                       version = "%prog 0.1")


        parser.add_option("","--source",type="string",dest="source",
                          default='localhost',
                          help="host name of message socket on source host")
        parser.add_option("","--sport",type="int",dest="sport",
                          default=6000,
                          help="port of message socket on source host")
        parser.add_option("","--dest",type="string",dest="dest",
                          default='localhost',
                          help="host name of message socket on dest host")
        parser.add_option("","--dport",type="int",dest="dport",
                          default=6000,
                          help="port of message socket on dest host")
        parser.add_option("","--ip",type="string",dest="ip",
                          default="127.0.0.1",
                          help="IP address of destination to blast")
        parser.add_option("","--port",type="int",dest="port",
                          default=5000,
                          help="port of destination to blast")
        parser.add_option("","--size",type="int",dest="size",
                          default=64,
                          help="packet size")
        parser.add_option("","--rate",type="int",dest="rate",
                          default=1000,
                          help="sending rate in packets per second")
        parser.add_option("","--repeat",type="int",dest="repeat",
                          default=10,
                          help="number of times to repeat experiment")
        parser.add_option("","--duration",type="float",dest="duration",
                          default=60.0,
                          help="duration of experiments in seconds")
        parser.add_option("","--quit",action="store_true",default=False,
                          dest="quit")

        (self.options,self.args) = parser.parse_args()

    def run(self):
        """ Setup the blaster and run for each specified rate.  Rates go
        from rate-start to rate-end + rate-inc, at increments of rate-inc.
        """
        b = Blaster(self.options.source,self.options.sport,
                    self.options.dest,self.options.dport)
        if self.options.quit:
            b.quit()
        else:
            for i in range(0,self.options.repeat):
                results = b.blast(self.options.ip,self.options.port,
                                  self.options.size,self.options.rate,
                                  self.options.duration)
                print results

if __name__ == "__main__":
    m = Main()
    m.run()
