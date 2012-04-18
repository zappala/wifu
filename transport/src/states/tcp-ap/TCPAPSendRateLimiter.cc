/* 
 * File:   TCPAPSendRateLimiter.cc
 * Author: leer
 * 
 * Created on July 1, 2011, 5:05 PM
 */

#include "states/tcp-ap/TCPAPSendRateLimiter.h"

TCPAPSendRateLimiter::TCPAPSendRateLimiter() : SendRateLimiter(){
    //Start with no limitation?
    setRate(0,0);
}

TCPAPSendRateLimiter::~TCPAPSendRateLimiter() {
}
void TCPAPSendRateLimiter::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    //Just need to grab some info: packet seqnum and the data size.
    TCPAPRateLimiterContext* rlc = (TCPAPRateLimiterContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if(p->is_naked_ack() || p->is_tcp_syn())
    {
        //Just an ack, just send it.
        //cout << "TCPAPSendRateLimiter: Sending ACK (unpaced).\n";
        this->SendRateLimiter::dispatch_packet(c, e->get_socket(), p);
        return;
    }

    u_int32_t size = p->get_data_length_bytes() + p->get_ip_header_length_bytes() + p->get_tcp_header_length_bytes();
    map<u_int32_t, u_int32_t, std::less<u_int32_t>, gc_allocator<std::pair<u_int32_t, u_int32_t> > >* sendmap = rlc->get_packet_map();
    sendmap[p->get_tcp_sequence_number(), size];

    //cout << "TCPAPSendRateLimiter::state_send_packet(): passing to send rate limiter.\n";
    this->SendRateLimiter::state_send_packet(c, q, e);
}

void TCPAPSendRateLimiter::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPAPRateLimiterContext* rlc = (TCPAPRateLimiterContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if(rlc->get_fixed_rate())
    {
        //Use fixed rate instead of AP calculation
        return;
    }

    //just an ACK
    if(p->is_tcp_ack()) {

        //Don't forget that we need the latest echo_reply for the next packet to be sent out.
        TCPTimestampOption* ts = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
        if (ts) {
            rlc->set_echo_reply(ts->get_timestamp());
        }

        //FHD calculation
        double curtime = Utils::get_current_time_microseconds_32();
        double rtt = curtime - ts->get_echo_reply();
        //HACK: since about once an hour the 32-bit current time seems to
        //roll over, we need to catch and ignore negative RTTs

        if(rtt < 0)
        {
            return;
        }

        ofstream outstream;
        outstream.open("AP_send_rate_log.txt", ios::app);

        //cout << "TCPAPSendRateLimiter::state_receive_packet(): current time = " << curtime << endl;
        //cout << "TCPAPSendRateLimiter::state_receive_packet(): echo reply = " << ts->get_echo_reply() << endl;
        //cout << "TCPAPSendRateLimiter::state_receive_packet(): rtt = " << rtt << endl;
        outstream << "TCPAPSendRateLimiter::state_receive_packet(): rtt = " << rtt << endl;
        
        //don't forget to update the RTT history for covariance
        list<double, gc_allocator<double> >* history = rlc->get_history();
        double max_size = (double)rlc->get_history_size();
        history->push_back(rtt);
        if(history->size() > max_size) {
            //cout << "TCPAPSendRateLimiter::receive_packet(): history size too big, gotta take a pop...\n";
            double popped = history->front();
            history->pop_front();
            //fast avg update
            double avg = rlc->get_avg_rtt();
            avg -= popped / max_size;
            avg += rtt / max_size;
            rlc->set_avg_rtt(avg);
        }
        else {
            //update avg
            double total = 0.0;
            list<double, gc_allocator<double> >::iterator i;
            for(i = history->begin();i != history->end();i++) {
                total += *i;
            }
            rlc->set_avg_rtt(total / (double)history->size());
        }

        double hops = MAX_TTL - p->get_ip_ttl() + 1;

        //Hops = 0 if sending to self?
        if(!hops)
        {
            hops = 1;
        }

        //bandwidth in bits per second - how do we calculate this in the protocol?
        double bandwidth = (double)rlc->get_bandwidth() / 8.0; //don't forget to convert to bytes
        double ack_size = p->get_data_length_bytes() + p->get_ip_header_length_bytes() + p->get_tcp_header_length_bytes();
        double data_size = rlc->get_packet_map()->find(p->get_tcp_ack_number())->second;
        double n_hop_factor = (double)rlc->get_n_hop_factor() / 2.0;

        double fhd = n_hop_factor * ((rtt / hops) + (data_size - ack_size) / bandwidth);
        double old_hat = rlc->get_fhd_hat();
        double alpha = rlc->get_alpha();

        /*cout << "TCPAPSendRateLimiter::state_receive_packet(): n_hop_factor = " << n_hop_factor << endl;
        cout << "TCPAPSendRateLimiter::state_receive_packet(): rtt = " << rtt << endl;
        cout << "TCPAPSendRateLimiter::state_receive_packet(): hops = " << hops << endl;
        cout << "TCPAPSendRateLimiter::state_receive_packet(): bandwidth = " << bandwidth << endl;
        cout << "TCPAPSendRateLimiter::state_receive_packet(): ack_size = " << ack_size << endl;
        cout << "TCPAPSendRateLimiter::state_receive_packet(): data_size = " << data_size << endl;

        cout << "TCPAPSendRateLimiter::state_receive_packet(): fhd = " << fhd << endl;
        cout << "TCPAPSendRateLimiter::state_receive_packet(): old_hat = " << old_hat << endl;
        cout << "TCPAPSendRateLimiter::state_receive_packet(): alpha = " << alpha << endl;*/

        if(!old_hat){
            rlc->set_fhd_hat(fhd);
        }
        else{
            rlc->set_fhd_hat(alpha * old_hat + (1.0 - alpha) * fhd);
        }
        //cout << "TCPAPSendRateLimiter::state_receive_packet(): fhd_hat = " << rlc->get_fhd_hat() << endl;

        //Covariance calculation

        double cov = calc_covariance(rlc);
        //cout << "TCPAPSendRateLimiter::state_receive_packet(): covariance = " << cov << endl;

        //delay in microseconds
        double delay = rlc->get_fhd_hat() * (1.0 + 2.0*cov);

        //Update the delay
        //cout << "TCPAPSendRateLimiter::state_receive_packet(): setting delay to: " << delay / MICROSECONDS_IN_SECONDS << " seconds on sender at address, port: " << e->get_packet()->get_dest_address_port()->to_s() << endl;
        outstream << "TCPAPSendRateLimiter::state_receive_packet(): setting delay to: " << delay / MICROSECONDS_IN_SECONDS << " seconds on sender at address, port: " << e->get_packet()->get_dest_address_port()->to_s() << endl;
        outstream.close();
        setRate(0, delay * 1000);
    }
}

double TCPAPSendRateLimiter::calc_covariance(Context* c) {
    TCPAPRateLimiterContext* rlc = (TCPAPRateLimiterContext*) c;
    list<double, gc_allocator<double> >* history = rlc->get_history();
    list<double, gc_allocator<double> >::iterator i;

    if(history->size() <= 1) {
        return 0.0;
    }

    double avg_rtt = rlc->get_avg_rtt();
    double cov = 0.0;
    double total = 0.0;
    //double total = 0.0;
    for(i = history->begin(); i != history->end(); i++) {
        //cout << "AP RTT history contains: " << *i << endl;
        total += pow(*i - avg_rtt, 2);
    }

    total = total / (double)history->size();
    cov = sqrt(total) / avg_rtt;

    return cov;
}

//This will be called just before the packet is sent to the network layer.
void TCPAPSendRateLimiter::protocol_specific_stuff(Context* c, Socket* s, WiFuPacket* packet) {
    //Need to update the timestamp or else we'll get REALLY long (and inaccurate) RTTs
    TCPAPRateLimiterContext* rlc = (TCPAPRateLimiterContext*) c;
    TCPPacket* p = (TCPPacket*) packet;
    TCPTimestampOption* option = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    option->set_timestamp();
    //cout << "Dispatching packet with timestamp: " << option->to_s() << endl;

    if (rlc->get_echo_reply()) {
        option->set_echo_reply(rlc->get_echo_reply());
    }

    p->pack();
    p->calculate_and_set_tcp_checksum();
}

void TCPAPSendRateLimiter::state_set_socket_option(Context* c, QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {
    TCPAPRateLimiterContext* rlc = (TCPAPRateLimiterContext*) c;
    cout << "TCPAPSendRateLimiter::state_set_socket_option()" << endl;
    Socket* s = e->get_socket();

    if(e->get_level() == TCP_AP) {
        //alpha
        if(e->get_option_name() == 1) {
            pair<int,int>* option_name = new pair<int, int>(TCP_AP, 1);
            double a = *(double*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPAPSendRateLimiter::state_set_socket_option(): setting alpha to " << a << endl;
            rlc->set_alpha(a);
        }
        //history size
        else if(e->get_option_name() == 2) {
            pair<int,int>* option_name = new pair<int, int>(TCP_AP, 2);
            u_int16_t size = *(u_int16_t*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPAPSendRateLimiter::state_set_socket_option(): setting histsize to " << size << endl;
            rlc->set_history_size(size);
        }
        //bandwidth
        else if(e->get_option_name() == 3) {
            pair<int,int>* option_name = new pair<int, int>(TCP_AP, 3);
            u_int32_t bw = *(u_int32_t*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPAPSendRateLimiter::state_set_socket_option(): setting bandwidth to " << bw << endl;
            rlc->set_bandwidth(bw);
        }
        //n-hop delay (default 4)
        else if(e->get_option_name() == 4) {
            pair<int,int>* option_name = new pair<int, int>(TCP_AP, 4);
            u_int16_t hops = *(u_int16_t*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPAPSendRateLimiter::state_set_socket_option(): setting n-hops to " << hops << endl;
            rlc->set_n_hop_factor(hops);
        }
        //fixed send rate in bits per second (default 0 = use AP)
        else if(e->get_option_name() == 5) {
            pair<int,int>* option_name = new pair<int, int>(TCP_AP, 5);
            u_int32_t rate = *(u_int32_t*) (s->get_socket_options().get(*option_name).first.data());

            cout << "TCPAPSendRateLimiter::state_set_socket_option(): setting permanent rate to " << rate << endl;
            rlc->set_fixed_rate(rate);

            //Convert to seconds and milliseconds for the rate limiter
            double bytes_per_sec = (double)rate / 8.0;
            double newrate = bytes_per_sec / (double)MTU;
            newrate = 1.0 / newrate;
            setRate(0, newrate * NANOSECONDS_IN_SECONDS);
        }
    }
}