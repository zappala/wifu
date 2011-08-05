
#include "gtest/gtest.h"
#include "LocalSocketFullDuplex.h"
#include "../headers/RandomStringGenerator.h"

using namespace std;

namespace {

    class UnixSocketSender : public LocalSocketSender {
    public:
        UnixSocketSender() : LocalSocketSender() {

        }

        virtual ~UnixSocketSender() {
            
        }
    };

    class UnixSocketReceiver : public LocalSocketReceiver, public LocalSocketReceiverCallback {
    public:
        UnixSocketReceiver(gcstring& file) : LocalSocketReceiver(file, this), LocalSocketReceiverCallback() {

        }

        virtual ~UnixSocketReceiver() {
            
        }

        virtual void receive(gcstring& message, u_int64_t& receive_time) {
//            cout << message << endl;
        }
    };

    TEST(UnixSocketSpeedTest, A) {
        gcstring file = "/tmp/speed_test";
        UnixSocketSender sender;
        

        int size = 100000;
        gcstring message = RandomStringGenerator::get_data(size);
        UnixSocketReceiver receiver(file);
        for(int i = 0; i < size; i++) {
            ssize_t sent = sender.send_to(file, message);
        }
        cout << "Done sending" << endl;
        

    }
}
