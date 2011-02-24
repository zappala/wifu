#!/usr/bin/env python

import os, sys

if __name__ == '__main__':
	os.system("cp bin/wifu-frontend-test /tmp")
	os.system("cp bin/wifu-end /tmp")
	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	if len(sys.argv) > 1:
		os.system(" ".join(["./wifu-frontend-test", " ".join(sys.argv[1:])]))
	else:
		os.system("./wifu-frontend-test")
