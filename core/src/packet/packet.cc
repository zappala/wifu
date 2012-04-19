#include "packet.h"

// public functions

Packet::Packet(struct nfq_q_handle* qh, int id, int payload_length,
               unsigned char* payload, Filter filter, bool preserve_payload) :
        qh_(qh), id_(id), payload_length_(payload_length), payload_(payload),
        filter_(filter), preserve_payload_(preserve_payload) {

    copy_payload();
}

Packet::Packet(Packet& p, bool preserve_payload) :
        qh_(p.qh_), id_(p.id_), payload_length_(p.payload_length_),
        filter_(p.filter_), preserve_payload_(preserve_payload),
        payload_(p.payload_) {

    copy_payload();
}

Packet::~Packet() {
    if (preserve_payload_) {
        delete(payload_);
    }
}

int Packet::id() {
    return id_;
}

void Packet::accept() {
    set_verdict(NF_ACCEPT, true);
}

void Packet::accept(bool use_payload) {
    set_verdict(NF_ACCEPT, use_payload);
}

void Packet::drop() {
    set_verdict(NF_DROP, false);
}

int Packet::get_payload_length() {
    return payload_length_;
}

Filter Packet::get_filter() {
    return filter_;
}

string Packet::get_filter_s() {
    switch (filter_) {
        case input:
            return string("input");
        case output:
            return string("output");
        case forward:
            return string("forward");
        case prerouting:
            return string("prerouting");
        case postrouting:
            return string("postrouting");
        default:
            throw "Invalid Filter";
    }
}

bool Packet::is_output() {
    return filter_ == output;
}

bool Packet::is_input() {
    return filter_ == input;
}

bool Packet::is_forward() {
    return filter_ == forward;
}

bool Packet::is_prerouting() {
    return filter_ == prerouting;
}

bool Packet::is_postrouting() {
    return filter_ == postrouting;
}

string Packet::to_s() {
    stringstream s;
    s << "netfilter ";
    s << get_filter_s() << " ";
    s << get_payload_length();
    return s.str();
}

string Packet::to_s_format() {
    return string("# netfilter filter payload_length");
}


// private functions

void Packet::set_verdict(int verdict, bool use_payload) {
    if (use_payload) {
        nfq_set_verdict(qh_, id_, verdict, payload_length_, payload_);
    } else {
        nfq_set_verdict(qh_, id_, verdict, 0, NULL);
    }
}

void Packet::copy_payload() {
    if (preserve_payload_ && payload_length_ > 0 && payload_ != NULL) {
        unsigned char* buf = new unsigned char[BUF_SIZE];
        memcpy(buf, payload_, payload_length_);
        payload_ = buf;
    }
}
