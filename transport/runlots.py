#!/usr/bin/env python


import os
import time

if __name__ == '__main__':
	runs = 10000
	broke = False
	for run in range(0, runs):
		print run + 1
		os.system("python integration-tests.py --gtest_filter=WiFuTransportMockTestDropRandom1Percent.tahoeSendReceiveTestActiveBigChunks1000000")
		time.sleep(1)
		os.system("ps axu | grep wifu")
