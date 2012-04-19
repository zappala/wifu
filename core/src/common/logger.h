// Log

#ifndef _logger_h
#define	_logger_h

#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <sstream>

#include "common/utils.h"
#include "packet/packet.h"

// Logs messages and packets to a file
class Logger {
public:
    Logger(const string& file_name);
    ~Logger();

    // log a string to the file
    void log(const string&);
    // log a packet to the file
    void log(Packet * p);

private:
    void close();

     // Log file writer
    ofstream log_file_;

    // Marks whether this log file has logged the format strings as a comment
    bool commented_;

    // semaphore for log usage
    sem_t sem_;
};

#endif	// _logger_h
