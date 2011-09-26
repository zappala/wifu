#include "Dispatcher.h"


Dispatcher& Dispatcher::instance() {
    static Dispatcher instance_;
    return instance_;
}

Dispatcher::~Dispatcher() {
    reset();

    log_INFORMATIONAL("dispatcher_recv_events_size: ", pantheios::i(receive_events_.size()), " dispatcher_recv_response_events_size: ", pantheios::i(recv_response_events_.size()), " dispatcher_recv_response_sizes_size: ", pantheios::i(recv_response_sizes_.size()));
    log_INFORMATIONAL("dispatcher_send_events_size: ", pantheios::i(send_events_.size()), " dispatcher_send_response_events_size: ", pantheios::i(send_response_events_.size()), " dispatcher_send_response_sizes_size: ", pantheios::i(send_response_sizes_.size()));

    while (!recv_response_events_.empty()) {

        int size = 1000;
        char start[size];
        char end[size];
        char size_i[size];
        char protocol_queue_size[size];
        char library_queue_size[size];
        memset(start, 0, size);
        memset(end, 0, size);
        memset(size_i, 0, size);
        memset(protocol_queue_size, 0, size);
        memset(library_queue_size, 0, size);

        sprintf(start, "%llu", receive_events_.front());
        sprintf(end, "%llu", recv_response_events_.front());
        sprintf(size_i, "%u", recv_response_sizes_.front());
        sprintf(library_queue_size, "%d", receive_library_queue_sizes_.front());
        sprintf(protocol_queue_size, "%d", receive_protocol_queue_sizes_.front());

        basic_string<PAN_CHAR_T> start_s(start);
        basic_string<PAN_CHAR_T> end_s(end);
        basic_string<PAN_CHAR_T> size_s(size_i);
        basic_string<PAN_CHAR_T> lqs(library_queue_size);
        basic_string<PAN_CHAR_T> pqs(protocol_queue_size);

        log_INFORMATIONAL("recv_before_dispatcher ", start_s, " ", end_s, " ", size_s, " ", pqs, " ", lqs);


        receive_events_.pop_front();
        recv_response_events_.pop_front();

        end_recv_response_sizes_.push_back(recv_response_sizes_.front());
        recv_response_sizes_.pop_front();

        receive_library_queue_sizes_.pop_front();
        receive_protocol_queue_sizes_.pop_front();
    }

    while (!send_response_events_.empty()) {

        int size = 1000;
        char start[size];
        char end[size];
        char size_i[size];
        char protocol_queue_size[size];
        char library_queue_size[size];
        memset(start, 0, size);
        memset(end, 0, size);
        memset(size_i, 0, size);
        memset(protocol_queue_size, 0, size);
        memset(library_queue_size, 0, size);

        sprintf(start, "%llu", send_events_.front());
        sprintf(end, "%llu", send_response_events_.front());
        sprintf(size_i, "%u", send_response_sizes_.front());
        sprintf(library_queue_size, "%d", send_library_queue_sizes_.front());
        sprintf(protocol_queue_size, "%d", send_protocol_queue_sizes_.front());

        basic_string<PAN_CHAR_T> start_s(start);
        basic_string<PAN_CHAR_T> end_s(end);
        basic_string<PAN_CHAR_T> size_s(size_i);
        basic_string<PAN_CHAR_T> lqs(library_queue_size);
        basic_string<PAN_CHAR_T> pqs(protocol_queue_size);

        log_INFORMATIONAL("send_before_dispatcher ", start_s, " ", end_s, " ", size_s, " ", pqs, " ", lqs);

        send_events_.pop_front();
        send_response_events_.pop_front();

        end_send_response_sizes_.push_back(send_response_sizes_.front());
        send_response_sizes_.pop_front();

        send_library_queue_sizes_.pop_front();
        send_protocol_queue_sizes_.pop_front();
    }

    log_INFORMATIONAL("dispatcher_after_recv_events_size: ", pantheios::i(end_receive_events_.size()), " dispatcher_after_recv_response_events_size: ", pantheios::i(end_recv_response_events_.size()), " dispatcher_after_recv_response_sizes_size: ", pantheios::i(end_recv_response_sizes_.size()));
    log_INFORMATIONAL("dispatcher_after_send_events_size: ", pantheios::i(end_send_events_.size()), " dispatcher_after_send_response_events_size: ", pantheios::i(end_send_response_events_.size()), " dispatcher_after_send_response_sizes_size: ", pantheios::i(end_send_response_sizes_.size()));

    while (!end_recv_response_events_.empty()) {

        int size = 1000;
        char start[size];
        char end[size];
        char size_i[size];
        memset(start, 0, size);
        memset(end, 0, size);
        memset(size_i, 0, size);

        sprintf(start, "%llu", end_receive_events_.front());
        sprintf(end, "%llu", end_recv_response_events_.front());
        sprintf(size_i, "%u", end_recv_response_sizes_.front());

        basic_string<PAN_CHAR_T> start_s(start);
        basic_string<PAN_CHAR_T> end_s(end);
        basic_string<PAN_CHAR_T> size_s(size_i);

        log_INFORMATIONAL("recv_after_dispatcher ", start_s, " ", end_s, " ", size_s);


        end_receive_events_.pop_front();
        end_recv_response_events_.pop_front();
        end_recv_response_sizes_.pop_front();
    }

    while (!end_send_response_events_.empty()) {

        int size = 1000;
        char start[size];
        char end[size];
        char size_i[size];
        memset(start, 0, size);
        memset(end, 0, size);
        memset(size_i, 0, size);

        sprintf(start, "%llu", end_send_events_.front());
        sprintf(end, "%llu", end_send_response_events_.front());
        sprintf(size_i, "%u", end_send_response_sizes_.front());

        basic_string<PAN_CHAR_T> start_s(start);
        basic_string<PAN_CHAR_T> end_s(end);
        basic_string<PAN_CHAR_T> size_s(size_i);

        log_INFORMATIONAL("send_after_dispatcher ", start_s, " ", end_s, " ", size_s);

        end_send_events_.pop_front();
        end_send_response_events_.pop_front();
        end_send_response_sizes_.pop_front();
    }
}

void Dispatcher::map_event(event_name name, EventQPPointer q) {
    mutex_.wait();
    if (map_.find(name) == map_.end()) {
        map_[name] = new vector<EventQPPointer, gc_allocator<EventQPPointer> >;
    }

    vector<EventQPPointer, gc_allocator<EventQPPointer> >::iterator itr = find(map_[name]->begin(), map_[name]->end(), q);
    if (itr == map_[name]->end()) {
        map_[name]->push_back(q);
    }
    mutex_.post();
}

void Dispatcher::reset() {
    mutex_.wait();
    itr_ = map_.begin();
    while (itr_ != map_.end()) {
        QPVectorPointer v = itr_->second;
        delete v;
        ++itr_;
    }
    map_.clear();
    mutex_.post();
}

void Dispatcher::process(Event* e) {
    mutex_.wait();
    assert(e);
    itr_ = map_.find(type_name(*e));

    if (itr_ != map_.end()) {

        QPVectorPointer queue_processors = itr_->second;

        for (int i = 0; i < queue_processors->size(); i++) {
//            cout << "Processing: " << type_name(*e) << endl;
            queue_size_ = queue_processors->at(i)->size();
            start_ = Utils::get_current_time_microseconds_64();
            queue_processors->at(i)->enqueue(e);
            end_ = Utils::get_current_time_microseconds_64();

            if (typeid(*e) == typeid(SendEvent)) {
                // incoming from front end
                send_events_.push_back(start_);
                end_send_events_.push_back(end_);
                send_protocol_queue_sizes_.push_back(queue_size_);

            }
            else if(typeid(*e) == typeid(ReceiveEvent)) {
                // incoming from front end
                receive_events_.push_back(start_);
                end_receive_events_.push_back(end_);
                receive_protocol_queue_sizes_.push_back(queue_size_);
            }
            else if(typeid(*e) == typeid(ResponseEvent)) {
                // coming from protocol
                struct GenericResponseMessage* grm = ((ResponseEvent*) e)->get_response();
                switch(grm->message_type) {
                    case WIFU_SENDTO:
                        send_response_events_.push_back(start_);
                        end_send_response_events_.push_back(end_);
                        send_response_sizes_.push_back(grm->return_value);
                        send_library_queue_sizes_.push_back(queue_size_);
                        break;
                    case WIFU_RECVFROM:
                    case WIFU_PRECLOSE:
                        recv_response_events_.push_back(start_);
                        end_recv_response_events_.push_back(end_);
                        recv_response_sizes_.push_back(grm->return_value);
                        receive_library_queue_sizes_.push_back(queue_size_);
                        break;
                }
            }
        }
    }
    mutex_.post();
}


Dispatcher::Dispatcher() : QueueProcessor<Event*>() {
    mutex_.init(1);
}
