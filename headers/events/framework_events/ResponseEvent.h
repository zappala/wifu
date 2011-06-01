/* 
 * File:   ResponseEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:36 PM
 */

#ifndef RESPONSEEVENT_H
#define	RESPONSEEVENT_H

#include <string>
#include <map>

#include "events/Event.h"
#include "QueryStringParser.h"
#include "Socket.h"
#include "Utils.h"

using namespace std;

class ResponseEvent : public Event {
public:
    ResponseEvent(Socket* socket, string& name, string& file);

    virtual ~ResponseEvent();

    string get_response();

    void put(string key, string value);

    void put(const char* key, string value);

    void execute(IModule* m);

    string get_write_file();

    string& get_value(string key);

private:
    string name_;
    string file_;
    map<string, string> m_;
            
};

#endif	/* RESPONSEEVENT_H */
