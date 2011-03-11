#!/usr/bin/env python

import os, sys

if __name__ == '__main__':
	os.system("cp bin/wifu-frontend-test /tmp")
	os.system("cp bin/wifu-end /tmp")
	os.system("cp test/frontend/*.conf /tmp")

	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	
	runCommand = "./wifu-frontend-test"
	if len(sys.argv) > 1:
		runCommand = " ".join([runCommand, " ".join(sys.argv[1:])])

	os.system(runCommand)
