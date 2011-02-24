#!/usr/bin/env python

import os, sys

if __name__ == '__main__':
	os.system("cp bin/wifu-end-test /tmp")
	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	if len(sys.argv) > 1:
		os.system(" ".join(["./wifu-end-test", " ".join(sys.argv[1:])]))
	else:
		os.system("./wifu-end-test")
