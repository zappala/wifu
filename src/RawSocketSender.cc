#include "RawSocketSender.h"

RawSocketSender::RawSocketSender() {
    create_socket();
    set_header_include();
}

ssize_t RawSocketSender::send(WiFuPacket* p) {
//    cout << "RawSocketSender::send(): sending packet:\n" << p->to_s() << endl;
    int ret = sendto(socket_,
            p->get_payload(),
            p->get_ip_tot_length(),
            0,
            (struct sockaddr*) p->get_dest_address_port()->get_network_struct_ptr(),
            (sizeof (struct sockaddr_in)));

    if (ret < 0) {
        perror("RawSocketSender: Error Sending Packet");
        // TODO: What should we do on a fail?
    }
    return ret;
}

void RawSocketSender::create_socket() {
    socket_ = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (socket_ < 0) {
        perror("NetworkInterface cannot create raw socket");
        exit(EXIT_FAILURE);
    }
}

void RawSocketSender::set_header_include() {
    int one = 1;
    const int *val = &one;
    if (setsockopt(socket_, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
        perror("NetworkInterface: cannot set HDRINCL");
        exit(EXIT_FAILURE);
    }
}
