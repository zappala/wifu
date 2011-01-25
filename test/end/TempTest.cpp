/* 
 * File:   TempTest.h
 * Author: rbuck
 *
 * Created on December 29, 2010, 2:43 PM
 */

#ifndef _TEMPTEST_H
#define	_TEMPTEST_H

#include "gtest/gtest.h"

#include "Module.h"

using namespace std;

class Temp : public Module {
public:

    Temp() : Module() {

    }

};

namespace {
	TEST(Temp) {
		Temp t;
		// TODO: This sleep a hack, we need to signal to the thread that it is okay to dequeue,
		// but at the same time we must wait for the thread to get to dequeue before we can
		// destroy this object.  It has only come up in test cases when we quickly create and
		// destroy objects of type QueueProcessor (QP).  We have not encountered this before due to
		// the fact that all objects we created of type QP were created in static space, not on the stack.
		// I have not tested what happens when we call new.
		//cout << "Starting processing." << endl;
		usleep(5000);
		ASSERT_TRUE(true);

	}
}

#endif	/* _TEMPTEST_H */

