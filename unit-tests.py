#!/usr/bin/env python

import os, sys

if __name__ == '__main__':
	os.system("cp bin/wifu-transport-unit-test /tmp")
	os.system("cp test/unit-tests/test_file /tmp")
	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	
	runCommand = "./wifu-transport-unit-test"
	if len(sys.argv) > 1:
		runCommand = " ".join([runCommand, " ".join(sys.argv[1:])])

	os.system(runCommand)
