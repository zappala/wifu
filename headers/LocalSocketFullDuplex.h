/* 
 * File:   BaseModule.h
 * Author: rbuck
 *
 * Created on October 18, 2010, 1:48 PM
 */

#ifndef _LOCALSOCKETFULLDUPLEX_H
#define	_LOCALSOCKETFULLDUPLEX_H

#include <string>

#include "LocalSocketSender.h"
#include "LocalSocketReceiver.h"

using namespace std;

class LocalSocketFullDuplex : public LocalSocketReceiverCallback, public LocalSocketSender, public LocalSocketReceiver {
public:
    LocalSocketFullDuplex(string & file);
    virtual ~LocalSocketFullDuplex();

};

#endif	/* _LOCALSOCKETFULLDUPLEX_H */

