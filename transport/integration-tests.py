#!/usr/bin/env python

import os, sys

if __name__ == '__main__':
	os.system("cp bin/wifu-integration-test /tmp")
	os.system("cp bin/wifu-transport /tmp")
	os.system("cp test/integration-tests/*.conf /tmp")

	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	
	runCommand = "./wifu-integration-test"
	if len(sys.argv) > 1:
		runCommand = " ".join([runCommand, " ".join(sys.argv[1:])])

	os.system(runCommand)
