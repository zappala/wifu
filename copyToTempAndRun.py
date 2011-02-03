#!/usr/bin/env python

import os

if __name__ == '__main__':
	os.system("cp bin/wifu-end-test /tmp")
	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	os.system("./wifu-end-test")
