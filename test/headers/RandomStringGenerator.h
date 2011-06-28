/* 
 * File:   RandomStringGenerator.h
 * Author: rbuck
 *
 * Created on January 31, 2011, 9:08 AM
 */

#ifndef _RANDOMSTRINGGENERATOR_H
#define	_RANDOMSTRINGGENERATOR_H

#include <iostream>

using namespace std;

class RandomStringGenerator {
public:
    static string get_data(int packet_size) {
        string data = "";
        //We shouldn't need to do this...but just in case of fragmented memory
        data.reserve(packet_size);
        for (int i = 0; i < packet_size; i++) {
            // Thanks to: http://www.geekstogo.com/forum/topic/94906-random-ints-and-chars-in-c/
            timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            srand(ts.tv_nsec);
            char c = (rand() % 26) + 'a';
            string s(1, c);
            data.append(s);
            //cout << "Data length: " << data.length() << endl;
            //cout << "String capacity: " << data.capacity() << endl;

        }
        cout << "Random string successfully generated.\n";
        return data;
    }
};

#define random_string(x) RandomStringGenerator::get_data(x)

#endif	/* _RANDOMSTRINGGENERATOR_H */

