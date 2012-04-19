#include "test_handler.h"

TestHandler::TestHandler() {
    logging_ = false;
    insert_shim_ = false;
}

TestHandler::~TestHandler() {
    if (logging_) {
        delete logger_;
    }
}

void TestHandler::parse_options(int argc, char* const argv[]) {

    // setup options
    static struct option long_options[] = {
        {"test-log",        no_argument,       NULL, 0},
        {"test-shim",       no_argument,       NULL, 1},
        {0, 0, 0, 0}
    };

    // parse options
    OptionParser options;
    options.parse(argc, argv, long_options);
    if (options.present("test-log"))
        logging_ = true;
    if (options.present("test-shim"))
        insert_shim_ = true;

    // setup logger
    if (logging_) {
        ostringstream file_name;
        file_name << "test-" << get_asc_time() << ".log";
        logger_ = new Logger(file_name.str());
    }
}

void TestHandler::input(IPPacket& p) {
    if (verbose_)
        cout << "Processing input packet " << p.id() << endl;

    // handle packets that don't have a shim header
    if (p.get_ip_protocol() != WIFU_PROTOCOL) {
        p.accept();
        return;
    }

    // print out shim header
    struct test_shim* shim = (struct test_shim*)p.get_shim();
    cout << "shim header a: " << shim->a << endl;

    // get WiFu protocol
    u_int8_t protocol = p.get_wifu()->protocol;

    // UDP
    if (protocol == UDP) {
        UDPPacket packet(p);
        packet.remove_shim();
        if (logging_)
            logger_->log(&packet);
        packet.accept();
        return;
    }

    // TCP
    if (protocol == TCP) {
        TCPPacket packet(p);
        packet.remove_shim();
        if (logging_)
            logger_->log(&packet);
        packet.accept();
        return;
    }

    // all other protocols
    p.remove_shim();
    if (logging_)
        logger_->log(&p);
    p.accept();
}

// add shim header
void TestHandler::output(IPPacket& p) {
    if (verbose_)
        cout << "Processing output packet " << p.id() << endl;

    u_int8_t protocol = p.get_ip_protocol();
    u_int8_t length = sizeof(struct test_shim);

    // UDP
    if (protocol == UDP) {
        UDPPacket packet(p);
        if (insert_shim_) {
            struct test_shim shim;
            shim.a = 100;
            packet.insert_shim((unsigned char*)&shim, 0, length);
        }
        if (logging_)
            logger_->log(&packet);
        packet.accept();
        return;
    }

    // TCP
    if (protocol == TCP) {
        TCPPacket packet(p);
        if (insert_shim_) {
            struct test_shim shim;
            shim.a = 100;
            packet.insert_shim((unsigned char*)&shim, 0, length);
        }
        if (logging_)
            logger_->log(&packet);
        packet.accept();
    }

    // all other protocols
    if (insert_shim_) {
        struct test_shim shim;
        shim.a = 100;
        p.insert_shim((unsigned char*)&shim, 0, length);
    }
    if (logging_)
        logger_->log(&p);
    p.accept();
}

void TestHandler::print_packet(Packet* p){
    cout << p->to_s_format() << endl;
    cout << p->to_s() << endl << endl;
}

// log packet
void TestHandler::forward(IPPacket& p) {
    if (verbose_)
        cout << "Processing forward packet " << p.id() << endl;

    u_int8_t protocol = p.get_ip_protocol();

    // UDP
    if (protocol == UDP) {
        UDPPacket packet(p);
        if (logging_)
            logger_->log(&packet);
        packet.accept();
    }

    // TCP
    if (protocol == TCP) {
        TCPPacket packet(p);
        if (logging_)
            logger_->log(&packet);
        packet.accept();
    }

    // all other protocols
    if (logging_)
        logger_->log(&p);
    p.accept();
}
