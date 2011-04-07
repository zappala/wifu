#!/usr/bin/env python


import os

if __name__ == '__main__':
	runs = 10000
	broke = False
	for run in range(0, runs):
		print run + 1
		os.system("./copyWifu-End-TestToTempAndRun.py")
