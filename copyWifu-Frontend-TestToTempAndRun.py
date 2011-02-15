#!/usr/bin/env python

import os

if __name__ == '__main__':
	os.system("cp bin/wifu-frontend-test /tmp")
	os.system("cp bin/wifu-end /tmp")
	os.chdir("/tmp")
	os.system("rm coverage.csexe")
	os.system("./wifu-frontend-test")
