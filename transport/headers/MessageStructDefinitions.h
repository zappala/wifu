/* 
 * File:   MessageStructDefinitions.h
 * Author: rbuck
 *
 * Created on August 8, 2011, 10:28 AM
 */

#ifndef MESSAGESTRUCTDEFINITIONS_H
#define	MESSAGESTRUCTDEFINITIONS_H

#include <sys/un.h>
#include <sys/socket.h>

struct GenericMessage {
    u_int32_t message_type;
    u_int32_t length;
    int fd;
};

struct FrontEndMessage : public GenericMessage {
    struct sockaddr_un source;
};

struct SocketMessage : public FrontEndMessage {
    int domain;
    int type;
    int protocol;
};

struct AddressMessage : public FrontEndMessage {
    struct sockaddr_in addr;
    socklen_t len;
};

struct BindMessage : public AddressMessage {
    
};

struct SockOptMessage : public FrontEndMessage {
    int level;
    int optname;
    socklen_t optlen;
};

struct GetSockOptMessage : public SockOptMessage {

};

struct SetSockOptMessage : public SockOptMessage {
    // Data of the optval goes after the header.
};

struct ListenMessage : public FrontEndMessage {
    int n;
};

struct AcceptMessage : public AddressMessage {
    // Has the same parameters as Bind
};

struct DataMessage : public AddressMessage {
    size_t buffer_length;
    int flags;
};

struct SendToMessage : public DataMessage {
    // Buffer data goes at the end
};

struct RecvFromMessage : public DataMessage {
    // same parameters as sendto, except we don't pass in buffer
};

struct ConnectMessage : public AddressMessage {
    // same parameters as bind
};

struct CloseMessage : public FrontEndMessage {
    // no extra params needed
};

// Response Messages

struct GenericResponseMessage : public GenericMessage {
    int return_value;
    // to set errno if needed
    int error;
};

struct SocketResponseMessage : public GenericResponseMessage {
    // return value is the new socket
};

struct BindResponseMessage : public GenericResponseMessage {
    // just need return value and errno
};

struct GetSockOptResponseMessage : public GenericResponseMessage {
    socklen_t optlen;
    // optval goes after this header
};

struct SetSockOptResponseMessage : public GenericResponseMessage {
    // just need return value (possibly errno?)
};

struct ListenResponseMessage : public GenericResponseMessage {
    // just need return value and errno
};

struct AddressResponseMessage : public GenericResponseMessage {
    struct sockaddr_in addr;
    socklen_t addr_len;
};

struct AcceptResponseMessage : public AddressResponseMessage {
    
};

struct SendToResponseMessage : public GenericResponseMessage {
    // just need return value (probably need errno too...)
};

struct RecvFromResponseMessage : public AddressResponseMessage {
    // size of buffer returned is in the return value
    // buffer of data goes after this header
};

struct ConnectResponseMessage : public GenericResponseMessage {
    // just need return value (probably need errno too...)
};

struct CloseResponseMessage : public GenericResponseMessage {
    // just need return value (probably need errno too...)
};

#endif	/* MESSAGESTRUCTDEFINITIONS_H */

