/* 
 * File:   LibraryEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 1:43 PM
 */

#ifndef LIBRARYEVENT_H
#define	LIBRARYEVENT_H

#include <string>
#include <map>

#include "defines.h"
#include "Event.h"
#include "QueryStringParser.h"

using namespace std;

class LibraryEvent : public Event {
public:

    LibraryEvent(string& message, string& file, int socket = 0) : Event(), file_(file) {
        QueryStringParser::parse(message, m_);
        name_ = m_[NAME_STRING];
        
        if (socket) {
            m_[SOCKET_STRING] = Utils::itoa(socket);
            set_socket(socket);
        } else {
            set_socket(atoi(m_[SOCKET_STRING].c_str()));
        }
    }

    virtual ~LibraryEvent() {

    }

    string& get_file() {
        return file_;
    }

    string& get_name() {
        return name_;
    }

    map<string, string>& get_map() {
        return m_;
    }

private:
    map<string, string> m_;
    string name_;
    string file_;
};

#endif	/* LIBRARYEVENT_H */

