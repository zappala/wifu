#!/usr/bin/env python

import os, sys

if __name__ == '__main__':
	os.system("cp bin/wifu-end-test /tmp")
	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	
	runCommand = "./wifu-end-test"
	if len(sys.argv) > 1:
		runCommand = " ".join([runCommand, " ".join(sys.argv[1:])])

	os.system(runCommand)
