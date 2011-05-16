/* 
 * File:   Math.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 4:17 PM
 */

#ifndef MATH_H
#define	MATH_H

#include <sys/types.h>

/**
 * Returns left <= number<= right.
 * This fuction handles curcular space.
 *
 * @param left LHS of the equation
 * @param number Number to compare
 * @param right RHS of the equation
 * @return True if left <= number <= right, false otherwise
 */
bool between_equal(u_int32_t left, u_int32_t number, u_int32_t right);

/**
 * Returns left < number<= right.
 * This fuction handles curcular space.
 *
 * @param left LHS of the equation
 * @param number Number to compare
 * @param right RHS of the equation
 * @return True if left < number <= right, false otherwise
 */
bool between_equal_right(u_int32_t left, u_int32_t number, u_int32_t right);

/**
 * Returns left <= number< right.
 * This fuction handles curcular space.
 *
 * @param left LHS of the equation
 * @param number Number to compare
 * @param right RHS of the equation
 * @return True if left <= number < right, false otherwise
 */
bool between_equal_left(u_int32_t left, u_int32_t number, u_int32_t right);

/**
 * Returns left < number< right.
 * This fuction handles curcular space.
 *
 * @param left LHS of the equation
 * @param number Number to compare
 * @param right RHS of the equation
 * @return True if left < number < right, false otherwise
 */
bool between(u_int32_t left, u_int32_t number, u_int32_t right);

/**
 * Returns lhs < rhs.
 * This fuction handles curcular space by determining if the two numbers are within half of the sequence space.
 * This should be more than ample for considering window sizes, even if the window scaling option is used (which allows for a window up to one gigabyte).
 * This allows a window of about two gigabytes.
 *
 * For example: lhs = 0 and rhs = UINT_MAX / 2 == true, but
 *              lsh = 0 and rhs = UINT_MAX / 2 + 1 == false
 *
 * @param lhs LHS of the equation
 * @param rhs RHS of the equation
 * @return True if lhs < rhs, false otherwise
 */
bool less_than(u_int32_t lhs, u_int32_t rhs);

/**
 * Returns lhs <= rhs
 * This function uses less_than()
 * @see less_than()
 * @param lhs LHS of the inequality
 * @param rhs RHS of the inequality
 * @return True of lhs <= rhs, false otherwise
 */
bool less_than_or_equal(u_int32_t lhs, u_int32_t rhs);


#endif	/* MATH_H */

