/*
 * UtilsTest.h
 *
 *  Created on: Dec 20, 2010
 *      Author: erickson
 */

#ifndef UTILSTEST_H_
#define UTILSTEST_H_

#include "gtest/gtest.h"
#include "Utils.h"

using namespace std;

namespace {

	class UtilsTest : public ::testing::Test {
	protected:
		virtual void SetUp() {}
		virtual void TearDown() {}

		timespec time;
		int secondsStart;
		long nanosecondsStart;

		//these are only here for convenience and must be set in the tests before using them
		int secondsDifference;
		long nanosecondsDifference;

		virtual void setupTimespec() {
			clock_gettime(CLOCK_REALTIME, &time);
			secondsStart = time.tv_sec;
			nanosecondsStart = time.tv_nsec;
		}

		//this is needed because timespec nanoseconds reset once they hit 1000000000 (one second) to 0
		virtual long getNanosecondDifference(long start, long end) {
			if (end >= start)
				return end - start;
			else
				return 1000000000 - start + end;
		}
	};

	TEST_F(UtilsTest, gettimespecfuturetimeSecondsTest) {
		nanosecondsDifference = 0;

		secondsDifference = 0;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_EQ(secondsDifference, time.tv_sec - secondsStart);

		secondsDifference = 1;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_EQ(secondsDifference, time.tv_sec - secondsStart);

		secondsDifference = 2;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_EQ(secondsDifference, time.tv_sec - secondsStart);

		secondsDifference = 180;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_EQ(secondsDifference, time.tv_sec - secondsStart);
	}

	TEST_F(UtilsTest, gettimespecfuturetimeNanosecondsTest) {
		secondsDifference = 0;
		int threshold = 1000; //a microsecond

		//nanoseconds < 1 second
		nanosecondsDifference = 0;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		nanosecondsDifference = 100;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		nanosecondsDifference = 500;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		nanosecondsDifference = 1000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		nanosecondsDifference = 10000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		nanosecondsDifference = 100000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		nanosecondsDifference = 1000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		nanosecondsDifference = 10000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		//nanoseconds = 1 second
		nanosecondsDifference = 100000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + threshold);

		//nanoseconds > 1 second
		nanosecondsDifference = 1000000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 1000000000 < nanosecondsDifference + threshold);

		nanosecondsDifference = 10000000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 10000000000 < nanosecondsDifference + threshold);

		nanosecondsDifference = 100000000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 100000000000 < nanosecondsDifference + threshold);

		nanosecondsDifference = 1000000000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 1000000000000 < nanosecondsDifference + threshold);

		nanosecondsDifference = 10000000000000;
		setupTimespec();
		Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
		ASSERT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 10000000000000 < nanosecondsDifference + threshold);
	}

}

#endif /* UTILSTEST_H_ */
