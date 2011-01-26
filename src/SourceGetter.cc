#include "../headers/SourceGetter.h"

// public functions

SourceGetter::~SourceGetter() {
    close(netlink_socket_);
    close(udp_socket_);
}

SourceGetter& SourceGetter::instance() {
    static SourceGetter instance_;
    return instance_;
}

string SourceGetter::get_source_address(string& dest_address) {

    if (!dest_address.compare("127.0.0.1")) {
        return dest_address;
    }

    cache_itr_ = cache_.find(dest_address);
    if (cache_itr_ != cache_.end()) {
        return (*cache_itr_).second;
    }

    AddressPort dest(dest_address, 0);
    in_addr_t destination = dest.get_network_struct().sin_addr.s_addr;

    infos_.clear();
    if (get_address_info(infos_)) {
        return "";
    }

    string result;
    for (int i = 0; i < infos_.size(); ++i) {
        in_addr_t mask = get_bitmask((const char*) infos_.at(i).if_name);
        in_addr addr;
        addr.s_addr = infos_.at(i).dest_addr;

        if ((destination & mask) == addr.s_addr) {
            addr.s_addr = infos_.at(i).src_addr;
            result = inet_ntoa(addr);
            cache_[dest_address] = result;
            break;
        }
    }
    return result;
}

// private functions

SourceGetter::SourceGetter() {
    udp_socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket_ <= 0) {
        perror("SourceGetter: socket");
        assert(false);
    }

    netlink_socket_ = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (netlink_socket_ <= 0) {
        perror("SourceGetter: netlink socket ");
        assert(false);
    }
}

in_addr_t SourceGetter::get_bitmask(const char* interface) {
    struct ifreq ifr;
    strcpy(ifr.ifr_name, interface);

    if (!ioctl(udp_socket_, SIOCGIFNETMASK, &ifr)) {
        return ((struct sockaddr_in *) & ifr.ifr_netmask)->sin_addr.s_addr;
    }
    return -1;
}

int SourceGetter::get_address_info(vector<address_info>& infos) {
    struct nlmsghdr *nlMsg;
    struct rtmsg *rtMsg;
    char msgBuf[BUFSIZE];
    int len, msgSeq = 0;

    /* Initialize the buffer */
    memset(msgBuf, 0, BUFSIZE);

    /* point the header and the msg structure pointers into the buffer */
    nlMsg = (struct nlmsghdr *) msgBuf;
    rtMsg = (struct rtmsg *) NLMSG_DATA(nlMsg);

    /* Fill in the nlmsg header*/
    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof (struct rtmsg)); // Length of message.
    nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .
    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
    nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet.
    nlMsg->nlmsg_pid = getpid(); // PID of process sending the request.

    if (send(netlink_socket_, nlMsg, nlMsg->nlmsg_len, 0) < 0) {
        printf("Write To Socket Failed...\n");
        return -1;
    }

    if ((len = read_response(netlink_socket_, msgBuf, msgSeq)) < 0) {
        printf("Read From Socket Failed...\n");
        return -1;
    }

    for (; NLMSG_OK(nlMsg, len); nlMsg = NLMSG_NEXT(nlMsg, len)) {
        struct address_info info;
        parse_routes(nlMsg, &info);
        if (info.src_addr) {
            infos.push_back(info);
        }
    }
    return 0;
}

int SourceGetter::read_response(int sock, char* buffer, int seq) {
    struct nlmsghdr *nlHdr;
    int readLen = 0;
    int msgLen = 0;
    int pid = getpid();

    do {
        if ((readLen = recv(sock, buffer, BUFSIZE - msgLen, 0)) < 0) {
            perror("SOCK READ: ");
            return -1;
        }

        nlHdr = (struct nlmsghdr *) buffer;

        /* Check if the header is valid */
        if ((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR)) {
            perror("Error in recieved packet");
            return -1;
        }

        /* Check if the its the last message */
        if (nlHdr->nlmsg_type == NLMSG_DONE) {
            break;
        } else {
            /* Else move the pointer to buffer appropriately */
            buffer += readLen;
            msgLen += readLen;
        }

        /* Check if its a multi part message */
        if ((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) {
            /* return if its not */
            break;
        }
    } while ((nlHdr->nlmsg_seq != seq) || (nlHdr->nlmsg_pid != pid));
    return msgLen;
}

void SourceGetter::parse_routes(struct nlmsghdr* header, struct address_info* info) {
    struct rtmsg *rtMsg = (struct rtmsg *) NLMSG_DATA(header);

    /* If the route is not for AF_INET or does not belong to main routing table
    then return. */
    if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN)) {
        return;
    }

    /* get the rtattr field */
    struct rtattr* rtAttr = (struct rtattr *) RTM_RTA(rtMsg);
    int rtLen = RTM_PAYLOAD(header);

    for (; RTA_OK(rtAttr, rtLen); rtAttr = RTA_NEXT(rtAttr, rtLen)) {
        switch (rtAttr->rta_type) {
            case RTA_OIF:
                if_indextoname(*(int *) RTA_DATA(rtAttr), info->if_name);
                break;
            case RTA_PREFSRC:
                info->src_addr = *(u_int *) RTA_DATA(rtAttr);
                break;
            case RTA_DST:
                info->dest_addr = *(u_int *) RTA_DATA(rtAttr);
                break;
            default:
                break;
        }
    }
    return;
}