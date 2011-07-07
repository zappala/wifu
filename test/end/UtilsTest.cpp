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
#include <sstream>
#include <limits>

using namespace std;

namespace {

    class gettimespecfuturetimeFixture : public ::testing::Test {
    protected:

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }

        timespec time;
        int secondsStart;
        long nanosecondsStart;

        static const int nanoSecondThreshold = 10000; //10 microseconds
        static const int nanoSecondsInASecond = 1000000;

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

    TEST_F(gettimespecfuturetimeFixture, gettimespecfuturetimeSecondsTest) {
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

    TEST_F(gettimespecfuturetimeFixture, gettimespecfuturetimeNanosecondsTest) {
        secondsDifference = 0;

        //TODO: some of these may fail because the threshold isn't high enough. Put them in a separate precision test executable?
        //for now we will use EXPECT instead of ASSERT

        //nanoseconds < 1 second
        nanosecondsDifference = 0;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 100;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 500;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 1000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 10000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 100000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 1000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 10000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 100000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        //nanoseconds = 1 second
        nanosecondsDifference = 1000000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 1000000000 < nanosecondsDifference + nanoSecondThreshold);

        //nanoseconds > 1 second
        nanosecondsDifference = 10000000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 10000000000 < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 100000000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 100000000000 < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 1000000000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 1000000000000 < nanosecondsDifference + nanoSecondThreshold);

        nanosecondsDifference = 10000000000000;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(getNanosecondDifference(nanosecondsStart, time.tv_nsec) + 10000000000000 < nanosecondsDifference + nanoSecondThreshold);
    }

    TEST_F(gettimespecfuturetimeFixture, gettimespecfuturetimeBothTest) {
        secondsDifference = 1;
        nanosecondsDifference = nanoSecondsInASecond / 2;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(time.tv_sec - secondsStart == secondsDifference &&
                getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        secondsDifference = 5;
        nanosecondsDifference = nanoSecondsInASecond / 4;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(time.tv_sec - secondsStart == secondsDifference &&
                getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        secondsDifference = 180;
        nanosecondsDifference = nanoSecondsInASecond / 8;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(time.tv_sec - secondsStart == secondsDifference &&
                getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        secondsDifference = 360;
        nanosecondsDifference = nanoSecondsInASecond / 16;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(time.tv_sec - secondsStart == secondsDifference &&
                getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        secondsDifference = 86400; //one day
        nanosecondsDifference = nanoSecondsInASecond / 32;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(time.tv_sec - secondsStart == secondsDifference &&
                getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        secondsDifference = 604800; //one day
        nanosecondsDifference = nanoSecondsInASecond / 64;
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(time.tv_sec - secondsStart == secondsDifference &&
                getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);

        secondsDifference = 2629743; //one month    | found on
        nanosecondsDifference = 830000; //one month | Google
        setupTimespec();
        Utils::get_timespec_future_time(secondsDifference, nanosecondsDifference, &time);
        EXPECT_TRUE(time.tv_sec - secondsStart == secondsDifference &&
                getNanosecondDifference(nanosecondsStart, time.tv_nsec) < nanosecondsDifference + nanoSecondThreshold);
    }

    TEST(UtilsTest, itoa) {
        stringstream converter;
        gcstring result;
        int intMax = numeric_limits<int>::max();
        int intMin = numeric_limits<int>::min();
        int oneHundred = 100;
        int negativeOneHundred = -100;
        int zero = 0;

        converter << intMax;
        result = converter.str();
        ASSERT_EQ(result, Utils::itoa(intMax));

        converter.str("");

        converter << intMin;
        result = converter.str();
        ASSERT_EQ(result, Utils::itoa(intMin));

        converter.str("");

        converter << oneHundred;
        result = converter.str();
        ASSERT_EQ(result, Utils::itoa(oneHundred));

        converter.str("");

        converter << negativeOneHundred;
        result = converter.str();
        ASSERT_EQ(result, Utils::itoa(negativeOneHundred));

        converter.str("");

        converter << zero;
        result = converter.str();
        ASSERT_EQ(result, Utils::itoa(zero));
    }

    TEST(UtilsTest, read_file) {

        gcstring file = "test_file";
        vector<gcstring> results = Utils::read_file(file);

        vector<gcstring> expected;
        expected.push_back("line one");
        expected.push_back("line two");
        expected.push_back("this is a longer line");
        expected.push_back("line four");

        ASSERT_EQ(expected.size(), results.size());

        for (int i = 0; i < results.size(); i++) {
            gcstring exp = expected[i];
            gcstring actual = results[i];

            ASSERT_EQ(exp, actual);
        }
    }

    TEST(UtilsTest, tokenize) {
        vector<gcstring> expected;
        expected.push_back("This");
        expected.push_back("is");
        expected.push_back("a");
        expected.push_back("line");

        gcstring line = "This is a line";
        gcstring delims = " ";
        vector<gcstring> results = Utils::tokenize(line, delims);

        ASSERT_EQ(expected.size(), results.size());

        for (int i = 0; i < expected.size(); i++) {
            gcstring exp = expected[i];
            gcstring actual = results[i];

            ASSERT_EQ(exp, actual);
        }
    }

    TEST(UtilsTest, getTime64) {
        u_int64_t time = Utils::get_current_time_microseconds_64();
    }

    TEST(UtilsTest, getTime32) {
        u_int32_t time = Utils::get_current_time_microseconds_32();
    }
}

#endif /* UTILSTEST_H_ */
