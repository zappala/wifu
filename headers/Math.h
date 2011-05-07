/* 
 * File:   Math.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 4:17 PM
 */

#ifndef MATH_H
#define	MATH_H

#include "GarbageCollector.h"

class Math : public gc {
public:

    static bool between_equal_right(u_int32_t left, u_int32_t number, u_int32_t right);
};

#endif	/* MATH_H */

