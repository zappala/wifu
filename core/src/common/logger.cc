#include "logger.h"

Logger::Logger(const string& file_name){
    log_file_.open(file_name.c_str());
    sem_init(&sem_, 0,1);
    commented_ = false;
}

Logger::~Logger(){
    close();
}

void Logger::log(const string& message){
    sem_wait(&sem_);
    log_file_ << message << endl;
    sem_post(&sem_);
}

void Logger::log(Packet * p){
    sem_wait(&sem_);

    if (!commented_) {
        log_file_ << "# timestamp usec" << endl;
        log_file_ << p->to_s_format() << endl << endl;
        commented_ = true;
    }

    log_file_ << get_time() << endl;
    log_file_ << p->to_s() << endl << endl;

    sem_post(&sem_);
}

void Logger::close(){
    log_file_.flush();
    log_file_.close();
}
