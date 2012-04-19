#include "wifu.h"

// global handler
Handler* handler_;

// global log
Logger* logger_;

// netfilter globals
struct nfq_handle* handle_;
struct nfq_q_handle* queue_handle_[5];
int enobufs_;

// option globals
bool verbose_;
bool flush_iptables_;
const char* interface_;
const char* config_file_;

// Returns the packet id netfilter assigned to this packet.
//
// nfa: pointer from which we gain access to the IP packet
//
// returns the packet id
static int get_packet_id(struct nfq_data* nfa) {
    struct nfqnl_msg_packet_hdr* ph;
    ph = nfq_get_msg_packet_hdr(nfa);
    return ntohl(ph->packet_id);
}

// Called for each packet passed from the INPUT netfilter queue to userspace
//
// Callback is defined here:
// http://www.nufw.org/doc/libnetfilter_queue/group__Queue.html#g138190b535fa79202b0bc4177edb7fa7
//
// qh: pointer to the queue handle managed by netfilter
// nfmsg: message object that contains the packet
// nfa: pointer from which we gain access to the IP packet
// data: the value passed to the data parameter of nfq_create_queue
//
// returns the packet id
static int input_callback(struct nfq_q_handle* qh, struct nfgenmsg* nfmsg,
                          struct nfq_data* nfa, void* data) {
    char* payload;

    int id = get_packet_id(nfa);
    int payload_length = nfq_get_payload(nfa, &payload);


    IPPacket p(qh, id, payload_length, (unsigned char*)payload, input, false);
    handler_->input(p);
    return id;
}

// Called for each packet passed from the OUTPUT netfilter queue to userspace
//
// qh: pointer to the queue handle managed by netfilter
// nfmsg: message object that contains the packet
// nfa: pointer from which we gain access to the IP packet
// data: the value passed to the data parameter of nfq_create_queue
//
// returns the packet id
static int output_callback(struct nfq_q_handle* qh, struct nfgenmsg* nfmsg,
                           struct nfq_data* nfa, void* data) {
    char* payload;

    int id = get_packet_id(nfa);
    int payload_length = nfq_get_payload(nfa, &payload);

    IPPacket p(qh, id, payload_length, (unsigned char*)payload, output, false);
    handler_->output(p);
    return id;
}


//
// Called for each packet passed from the FORWARD netfilter queue to userspace
//
// qh: pointer to the queue handle managed by netfilter
// nfmsg: message object that contains the packet
// nfa: pointer from which we gain access to the IP packet
// data: the value passed to the data parameter of nfq_create_queue
//
// returns the packet id
static int forward_callback(struct nfq_q_handle* qh, struct nfgenmsg* nfmsg,
                            struct nfq_data* nfa, void* data) {
    char*  payload;

    int id = get_packet_id(nfa);
    int payload_length = nfq_get_payload(nfa, &payload);

    IPPacket p(qh, id, payload_length, (unsigned char*)payload, forward, false);
    handler_->forward(p);
    return id;
}

// Called for each packet passed from the PREROUTING netfilter queue
// to userspace
//
// qh: pointer to the queue handle managed by netfilter
// nfmsg: message object that contains the packet
// nfa: pointer from which we gain access to the IP packet
// data: the value passed to the data parameter of nfq_create_queue
//
// returns the packet id
static int prerouting_callback(struct nfq_q_handle* qh, struct nfgenmsg* nfmsg,
                            struct nfq_data* nfa, void* data) {
    char*  payload;

    int id = get_packet_id(nfa);
    int payload_length = nfq_get_payload(nfa, &payload);

    IPPacket p(qh, id, payload_length, (unsigned char*)payload, prerouting, false);
    handler_->prerouting(p);
    return id;
}

// Called for each packet passed from the POSTROUTING netfilter queue
// to userspace
//
// qh: pointer to the queue handle managed by netfilter
// nfmsg: message object that contains the packet
// nfa: pointer from which we gain access to the IP packet
// data: the value passed to the data parameter of nfq_create_queue
//
// returns the packet id
static int postrouting_callback(struct nfq_q_handle* qh, struct nfgenmsg* nfmsg,
                            struct nfq_data* nfa, void* data) {
    char*  payload;

    int id = get_packet_id(nfa);
    int payload_length = nfq_get_payload(nfa, &payload);

    IPPacket p(qh, id, payload_length, (unsigned char*)payload, postrouting, false);
    handler_->postrouting(p);
    return id;
}

// Opens a library handle, binds it to AF_INET, then sets up callback
// functions for the INPUT, OUTPUT, FORWARD, PREROUTING, and
// POSTROUTING queues.
void setup_nfq(){
    struct nfq_q_handle* qh;
    
    handle_ = nfq_open();
    if (!handle_) {
        perror("Can't get netfilter handle");
        exit(1);
    }

    nfq_unbind_pf(handle_, AF_INET);

    if (nfq_bind_pf(handle_, AF_INET) < 0) {
        perror("Can't bind netfilter handle");
        exit(1);
    }

    // INPUT queue (0)
    qh = nfq_create_queue(handle_,  0, &input_callback, NULL);
    if (!qh) {
        perror("Can't create netfilter input queue");
        exit(1);
    }
    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        perror("Can't set netfilter copy mode for input queue");
        exit(1);
    }
    queue_handle_[0] = qh;

    // OUTPUT queue (1)
    qh = nfq_create_queue(handle_,  1, &output_callback, NULL);
    if (!qh) {
        perror("Can't create netfilter output queue");
        exit(1);
    }
    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        perror("Can't set netfilter copy mode for output queue");
        exit(1);
    }
    queue_handle_[1] = qh;

    // FORWARD queue (2)
    qh = nfq_create_queue(handle_,  2, &forward_callback, NULL);
    if (!qh) {
        perror("Can't create netfilter forward queue");
        exit(1);
    }
    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        perror("Can't set netfilter copy mode for forward queue");
        exit(1);
    }
    queue_handle_[2] = qh;

    // PREROUTING queue (3)
    qh = nfq_create_queue(handle_,  3, &prerouting_callback, NULL);
    if (!qh) {
        perror("Can't create netfilter prerouting queue");
        exit(1);
    }
    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        perror("Can't set netfilter copy mode for prerouting queue");
        exit(1);
    }
    queue_handle_[3] = qh;

    // POSTROUTING queue (4)
    qh = nfq_create_queue(handle_,  4, &postrouting_callback, NULL);
    if (!qh) {
        perror("Can't create netfilter postrouting queue");
        exit(1);
    }
    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        perror("Can't set netfilter copy mode for postrouting queue");
        exit(1);
    }
    queue_handle_[4] = qh;
}

void usage() {
    cout << "wifu options (with defaults)" << endl;
    cout << "\t--verbose               : turn on debugging" << endl;
    cout << "\t--flush-iptables        : flush the mangle iptable before configuring" << endl;
    cout << "\t--interface [name]      : interface to run on (wlan0)" << endl;
    cout << "\t--config [file]         : iptables configuration file (wifu.conf)" << endl;
    cout << "\t--handler [name]        : packet handler (default)" << endl;
    cout << "\t--handler-config [file] : handler configuration file" << endl;

    exit(-1);
}

// Parses command line options and sets the associated global variables
// appropriately

// argc: argc passed into main
// argv: argv passed into main
void parse_options(int argc, char* const argv[]) {

    // The command line options should limited to those that are
    // necessary for basic processing by ALL handlers.
    // Handler-specific options MUST be formatted as
    // [handler]-[option] and are processed by the handler.
    static struct option long_options[] = {
        {"verbose",         no_argument,       NULL, 0},
        {"flush-iptables",  no_argument,       NULL, 1},
        {"interface",       required_argument, NULL, 2},
        {"config",          required_argument, NULL, 3},
        {"handler",         required_argument, NULL, 4},
        {0, 0, 0, 0}
    };

    // default options
    verbose_ = false;
    flush_iptables_ = false;
    interface_ = "wlan0";
    config_file_ = "wifu.conf";

    // default handler
    handler_ = NULL;
    
    // parse options
    OptionParser options;
    options.parse(argc, argv, long_options);
    if (options.present("verbose"))
        verbose_ = true;
    if (options.present("flush-iptables"))
        flush_iptables_ = true;
    if (options.present("interface"))
        interface_ = options.argument("interface");
    if (options.present("config"))
        config_file_ = options.argument("config");
    if (options.present("handler"))
        handler_ = create_handler(options.argument("handler"));

    // setup handler
    if (handler_ == NULL) {
        handler_ = new Handler();
    }
    handler_->parse_options(argc, argv);
    handler_->start();
}

// Reads the configuration file to set IPtables rules.
//
// add: true = add rules, false = delete rules
void setup_iptables(bool add) {
    string line;
    string config;

    // flush tables if requested
    if (add && flush_iptables_) {
        system("iptables -t mangle -F");
    }

    // open file
    ifstream file(config_file_);
    if (!file.is_open()) {
        cerr << "Can't open " << config_file_ << endl;
        return;
    }

    // parse file
    while (!file.eof()) {
        getline(file,line);
        if ((line == "") || (line[0] == '#')) {
            continue;
        }
        if (add) {
            config = "iptables -t mangle -A " + line;
        }
        else {
            config = "iptables -t mangle -D " + line;
        }
        system(config.c_str());
    }

    // close file
    file.close();
}

// Sets up the logger to record wifu type information (enobufs)
void setup_logger(){
    ostringstream file_name;
    file_name << "wifu-" <<  get_asc_time() << ".log";
    logger_ = new Logger(file_name.str());
}

// Performs a nice cleanup of the queue and handle and calls undo_tables.
void quit() {
    nfq_destroy_queue(queue_handle_[0]);
    nfq_destroy_queue(queue_handle_[1]);
    nfq_destroy_queue(queue_handle_[2]);
    nfq_destroy_queue(queue_handle_[3]);
    nfq_destroy_queue(queue_handle_[4]);
    nfq_close(handle_);
    setup_iptables(false);
    ostringstream message;
    message << "enobufs " << enobufs_;
    logger_->log(message.str());
    delete handler_;
    delete logger_;
}

// Manage system signals registered in register_signals. Calls quit
// when a terminating singal is received by the system.
//
// signal: the active system signal
void signal_manager(int signal) {
    switch(signal) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            quit();
            exit(0);
    }
}

// Register system signals to be handled by the application.
void register_signals(){
    signal(SIGINT, signal_manager);
    signal(SIGQUIT, signal_manager);
    signal(SIGTERM, signal_manager);
}

int main(int argc, char **argv) {
    int fd;
    int count;
    // does the following need to be aligned?
    // __attribute__ ((aligned));
    char buf[BUF_SIZE];

    register_signals();
    parse_options(argc, argv);
    setup_logger();
    setup_iptables(true);
    setup_nfq();
    fd = nfq_fd(handle_);

    enobufs_ = 0;
    while (1) {
        count = recv(fd, buf, sizeof(buf), 0);

        if (count < 0) {
            // The kernel uses a socket to send packets to the
            // application layer.  This socket buffer may overflow,
            // causing an ENOBUFS error.  This means the kernel has
            // dropped some packets, rather than sending them up to
            // the application.  We ignore the error, because there is
            // nothing else we can do.  Be sure to write fast handlers
            // so that this error does not happen very often.
            if(errno == ENOBUFS) {
                enobufs_++;
                continue;
            }
            break;
        }
        nfq_handle_packet(handle_, buf, count);
    }

    // clean up in case we jump out of the loop (on a recv error)
    quit();

    return(EXIT_FAILURE);
}

