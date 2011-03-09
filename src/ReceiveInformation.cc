#include "ReceiveInformation.h"

ReceiveInformation::ReceiveInformation(string& file, int receiving_buffer_size) :
    file_(file), receiving_buffer_size_(receiving_buffer_size) {

}

string& ReceiveInformation::get_file() {
    return file_;
}

void ReceiveInformation::set_file(string& file) {
    file_ = file;
}

int ReceiveInformation::get_receiving_buffer_size() {
    return receiving_buffer_size_;
}

void ReceiveInformation::set_receiving_buffer_size(int size) {
    receiving_buffer_size_ = size;
}
