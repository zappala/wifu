#!/usr/bin/env python


import os

if __name__ == '__main__':
	output = ''
	runs = 5000
	broke = False
	for run in range(0, runs):
		print run + 1
		os.system("python copyWifu-End-TestToTempAndRun.py")
