/* 
 * File:   SimpleTCP.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 3:13 PM
 */

#ifndef SIMPLETCP_H
#define	SIMPLETCP_H

#include "Protocol.h"
#include "defines.h"

#include "contexts/IContext.h"
#include "contexts/IContextContainer.h"
#include "contexts/ReliabilityContext.h"

class SimpleTCP : public Protocol {
private:

    SimpleTCP() : Protocol(SIMPLE_TCP) {

    }

    void save_socket(Socket* s) {
        map_[s] = new IContextContainer();
    }

    IContextContainer* get_context(Socket* s) {
        itr_ = map_.find(s);
        assert(itr_ != map_.end());
        return itr_->second;
    }


    tr1::unordered_map<Socket*, IContextContainer*> map_;
    tr1::unordered_map<Socket*, IContextContainer*>::iterator itr_;

public:

    static SimpleTCP& instance() {
        static SimpleTCP instance_;
        return instance_;
    }

    virtual ~SimpleTCP() {

    }

    void socket(Socket* s) {
        save_socket(s);
        IContextContainer* c = get_context(s);
        
        c->get_congestion_control()->socket(s);
        c->get_connection_manager()->socket(s);
        c->get_reliability()->socket(s);

    }

    void bind(Socket* s, AddressPort* ap) {
        IContextContainer* c = get_context(s);
        
        c->get_congestion_control()->bind(s, ap);
        c->get_connection_manager()->bind(s, ap);
        c->get_reliability()->bind(s, ap);
    }

    void listen(Socket* s, int back_log) {
        IContextContainer* c = get_context(s);

        c->get_congestion_control()->listen(s, back_log);
        c->get_connection_manager()->listen(s, back_log);
        c->get_reliability()->listen(s, back_log);
    }

    void receive_packet(Socket* s, WiFuPacket* p) {
        IContextContainer* c = get_context(s);

        c->get_congestion_control()->receive_packet(s, p);
        c->get_connection_manager()->receive_packet(s, p);
        c->get_reliability()->receive_packet(s, p);
    }

    void send_packet(Socket* s, WiFuPacket* p) {
        cout << "SimpleTCP::send_packet()" << endl;
        IContextContainer* c = get_context(s);
        
        c->get_congestion_control()->send_packet(s, p);
        c->get_connection_manager()->send_packet(s, p);
        c->get_reliability()->send_packet(s, p);

        NetworkSendPacketEvent* e = new NetworkSendPacketEvent(s, p);
        Dispatcher::instance().enqueue(e);
    }

    void connect(ConnectEvent* e) {
        IContextContainer* c = get_context(e->get_socket());

        c->get_congestion_control()->connect(e);
        c->get_connection_manager()->connect(e);
        c->get_reliability()->connect(e);
    }

    void accept(AcceptEvent* e) {
        IContextContainer* c = get_context(e->get_socket());

        c->get_congestion_control()->accept(e);
        c->get_connection_manager()->accept(e);
        c->get_reliability()->accept(e);
    }

    void new_connection_established(Socket* s) {
        save_socket(s);
        IContextContainer* c = get_context(s);
        
        c->get_congestion_control()->new_connection_established(s);
        c->get_connection_manager()->new_connection_established(s);
        c->get_reliability()->new_connection_established(s);

    }

    void close() {
    }

};

#endif	/* SIMPLETCP_H */

