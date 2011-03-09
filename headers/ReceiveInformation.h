/* 
 * File:   ReceiveInformation.h
 * Author: rbuck
 *
 * Created on March 9, 2011, 11:39 AM
 */

#ifndef _RECEIVEINFORMATION_H
#define	_RECEIVEINFORMATION_H

#include <string>
#include "GarbageCollector.h"

using namespace std;

class ReceiveInformation : public gc {
public:
    ReceiveInformation(string& file, int receiving_buffer_size);

    string& get_file();
    void set_file(string& file);

    int get_receiving_buffer_size();
    void set_receiving_buffer_size(int size);

private:
    int receiving_buffer_size_;
    string file_;
    
};

#endif	/* _RECEIVEINFORMATION_H */

