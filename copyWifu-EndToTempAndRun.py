#!/usr/bin/env python

import os

if __name__ == '__main__':
	os.system("cp -f bin/wifu-end /tmp")
	os.chdir("/tmp")
	os.system("./wifu-end")
