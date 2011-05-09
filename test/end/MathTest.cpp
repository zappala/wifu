#include "Math.h"

#include <iostream>
#include <limits.h>
#include "gtest/gtest.h"

using namespace std;

namespace {

    // Max: UINT_MAX

    TEST(Math, between) {
        // easy
        for (int i = 2; i < 9; i++) {
            ASSERT_TRUE(between(1, i, 10));
        }
        // low side
        ASSERT_FALSE(between(3, 2, 5));
        // high side
        ASSERT_FALSE(between(3, 7, 5));
        // low side equal
        ASSERT_FALSE(between(3, 3, 5));
        // high side equal
        ASSERT_FALSE(between(3, 5, 5));

        // check circular space
        // num and right wrap
        u_int32_t left = UINT_MAX - 2;
        u_int32_t num = left + 5;
        u_int32_t right = left + 6;
        ASSERT_TRUE(between(left, num, right));

        // only num wraps
        num = left + 1;
        ASSERT_TRUE(between(left, num, right));

        // equal on high side, with wrap
        num = left + 5;
        right = num;
        ASSERT_FALSE(between(left, num, right));

        // equal on low side, with wrap
        num = left;
        right = left + 6;
        ASSERT_FALSE(between(left, num, right));

        // not between low side with wrap
        num = left - 1;
        right = left + 5;
        ASSERT_FALSE(between(left, num, right));

        // not between high side with wrap
        num = left + 6;
        right = left + 5;
        ASSERT_FALSE(between(left, num, right));
    }

    TEST(Math, betweenEqual) {
        // easy
        for (int i = 1; i <= 10; i++) {
            ASSERT_TRUE(between_equal(1, i, 10));
        }
        // low side
        ASSERT_FALSE(between_equal(3, 2, 5));
        // high side
        ASSERT_FALSE(between_equal(3, 7, 5));
        // low side equal
        ASSERT_TRUE(between_equal(3, 3, 5));
        // high side equal
        ASSERT_TRUE(between_equal(3, 5, 5));

        // check circular space
        // num and right wrap
        u_int32_t left = UINT_MAX - 2;
        u_int32_t num = left + 5;
        u_int32_t right = left + 6;
        ASSERT_TRUE(between_equal(left, num, right));

        // only num wraps
        num = left + 1;
        ASSERT_TRUE(between_equal(left, num, right));

        // equal on high side, with wrap
        num = left + 5;
        right = num;
        ASSERT_TRUE(between_equal(left, num, right));

        // equal on low side, with wrap
        num = left;
        right = left + 6;
        ASSERT_TRUE(between_equal(left, num, right));

        // not between low side with wrap
        num = left - 1;
        right = left + 5;
        ASSERT_FALSE(between_equal(left, num, right));

        // not between high side with wrap
        num = left + 6;
        right = left + 5;
        ASSERT_FALSE(between_equal(left, num, right));
    }

    TEST(Math, betweenEualLeft) {
        // easy
        for (int i = 1; i < 10; i++) {
            ASSERT_TRUE(between_equal_left(1, i, 10));
        }
        // low side
        ASSERT_FALSE(between_equal_left(3, 2, 5));
        // high side
        ASSERT_FALSE(between_equal_left(3, 7, 5));
        // low side equal
        ASSERT_TRUE(between_equal_left(3, 3, 5));
        // high side equal
        ASSERT_FALSE(between_equal_left(3, 5, 5));

        // check circular space
        // num and right wrap
        u_int32_t left = UINT_MAX - 2;
        u_int32_t num = left + 5;
        u_int32_t right = left + 6;
        ASSERT_TRUE(between_equal_left(left, num, right));

        // only num wraps
        num = left + 1;
        ASSERT_TRUE(between_equal_left(left, num, right));

        // equal on high side, with wrap
        num = left + 5;
        right = num;
        ASSERT_FALSE(between_equal_left(left, num, right));

        // equal on low side, with wrap
        num = left;
        right = left + 6;
        ASSERT_TRUE(between_equal_left(left, num, right));

        // not between low side with wrap
        num = left - 1;
        right = left + 5;
        ASSERT_FALSE(between_equal_left(left, num, right));

        // not between high side with wrap
        num = left + 6;
        right = left + 5;
        ASSERT_FALSE(between_equal_left(left, num, right));
    }

    TEST(Math, betweenEqualRight) {
        // easy
        for (int i = 2; i <= 10; i++) {
            ASSERT_TRUE(between_equal_right(1, i, 10));
        }
        // low side
        ASSERT_FALSE(between_equal_right(3, 2, 5));
        // high side
        ASSERT_FALSE(between_equal_right(3, 7, 5));
        // low side equal
        ASSERT_FALSE(between_equal_right(3, 3, 5));
        // high side equal
        ASSERT_TRUE(between_equal_right(3, 5, 5));

        // check circular space
        // num and right wrap
        u_int32_t left = UINT_MAX - 2;
        u_int32_t num = left + 5;
        u_int32_t right = left + 6;
        ASSERT_TRUE(between_equal_right(left, num, right));

        // only num wraps
        num = left + 1;
        ASSERT_TRUE(between_equal_right(left, num, right));

        // equal on high side, with wrap
        num = left + 5;
        right = num;
        ASSERT_TRUE(between_equal_right(left, num, right));

        // equal on low side, with wrap
        num = left;
        right = left + 6;
        ASSERT_FALSE(between_equal_right(left, num, right));

        // not between low side with wrap
        num = left - 1;
        right = left + 5;
        ASSERT_FALSE(between_equal_right(left, num, right));

        // not between high side with wrap
        num = left + 6;
        right = left + 5;
        ASSERT_FALSE(between_equal_right(left, num, right));
    }
}
