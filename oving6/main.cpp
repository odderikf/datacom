#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <openssl/sha.h>
#include <bitset>

using boost::asio::ip::tcp;

#define MAGIC_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define FIN 0b10000000
#define OP_TEXTFRAME 0b0001
#define OP_CLOSE 0b1000


class WebServer{
    boost::asio::io_service io_service;
    tcp::endpoint endpoint;
    tcp::acceptor acceptor;

    std::vector<std::shared_ptr<tcp::socket>> active_connections;

    void accept_request(){
        auto socket = std::make_shared<tcp::socket>(io_service);
        acceptor.async_accept(*socket, [this, socket](const boost::system::error_code &ec){
            accept_request();

            if(!ec){
                handle_request(socket);
            }

        });
    }

    void listen_connection(const std::shared_ptr<tcp::socket> &socket){

        auto op = std::make_shared<std::vector<uint8_t>>(2);
        boost::asio::async_read(*socket, boost::asio::buffer(*op), boost::asio::transfer_exactly(2),
        [=](const boost::system::error_code &ec, size_t){
            if(op->at(0) & OP_CLOSE){
                auto write_buffer2 = std::make_shared<boost::asio::streambuf>();
                std::ostream write_stream2(write_buffer2.get());

                std::string message2 = "bye!";
                write_stream2 << (char) (FIN | OP_CLOSE);
                write_stream2 << message2.size();
                write_stream2 << message2;

                async_write(*socket, *write_buffer2, [=](const boost::system::error_code &ec, size_t) {
                    active_connections.erase(std::find(active_connections.begin(), active_connections.end(), socket));
                    return;
                });
            } else {
                uint64_t payload_length;
                uint8_t payload_byte = op->at(1) & uint8_t(0b01111111);
                if(payload_byte < 126) payload_length = payload_byte;
                else {
                    unsigned to_read = 2;
                    if (payload_byte == 127) to_read = 8;
                    auto pay = std::make_shared<std::vector<uint8_t>>(to_read);
                    boost::asio::read(*socket, boost::asio::buffer(*pay), boost::asio::transfer_exactly(to_read));
                    if(payload_byte == 126) payload_length = (pay->at(0) << 8) + pay->at(1);
                    else payload_length = (uint64_t(pay->at(0)) << 56) + (uint64_t(pay->at(1)) << 48)
                                        + (uint64_t(pay->at(2)) << 40) + (uint64_t(pay->at(3)) << 32)
                                        + (pay->at(4) << 24) + (pay->at(5) << 16) + (pay->at(6) << 8) + pay->at(7);

                }

                auto read_buffer = std::make_shared<boost::asio::streambuf>();
                boost::asio::async_read(*socket, *read_buffer, boost::asio::transfer_exactly(payload_length),
                [=](const boost::system::error_code &ec, size_t){
                    std::istream read_stream(read_buffer.get());
                    std::string message(std::istreambuf_iterator<char>(read_stream), {});

                    std::cout << message << std::endl;
                    for(const auto &i : active_connections){
                        auto write_buffer3 = std::make_shared<boost::asio::streambuf>();
                        std::ostream write_stream3(write_buffer3.get());

                        write_stream3 << (char) (FIN | OP_TEXTFRAME);
                        write_stream3 << message.size();
                        write_stream3 << message;
                        async_write(*i, *write_buffer3, [=](const boost::system::error_code &ec, size_t){
                            listen_connection(socket);
                        });
                    }
                });
            }

        });
    }
    void handle_connection(const std::shared_ptr<tcp::socket> &socket){

        active_connections.emplace_back(socket);
        auto write_buffer = std::make_shared<boost::asio::streambuf>();
        std::ostream write_stream(write_buffer.get());

        std::string message = "hello world";
        write_stream << (unsigned char) (FIN | OP_TEXTFRAME);
        write_stream << (unsigned char) message.size();
        write_stream << message;


        async_write(*socket, *write_buffer, [=](const boost::system::error_code &ec, size_t) {
            if (!ec) {
                std::cout << "server: sent hello world to client" << std::endl;
                listen_connection(socket);
            }
        });
    }
    void handle_request(const std::shared_ptr<tcp::socket> &socket){
        auto read_buffer = std::make_shared<boost::asio::streambuf>();
        boost::asio::async_read_until(*socket, *read_buffer, "\r\n\r\n", [=](const boost::system::error_code &ec, size_t){
            if(!ec){
                std::string message;
                std::string failmessage;
                std::istream read_stream(read_buffer.get());
                message.pop_back();
                if(message == "exit") return;
                read_stream >> message;
                if(message != "GET") return;
                read_stream >> message;
                if(message != "/hello") return;
                std::getline(read_stream, message); // get rid of rest of line

                std::map<std::string, std::string> headers;
                while(std::getline(read_stream, message) && message != "\r"){
                    auto split = message.find(": ");
                    std::string title(message.begin(), message.begin()+split);
                    std::string value(message.begin()+split+1, message.end());
                    boost::algorithm::trim(value);
                    headers[title] = value;
                    std::cout << title << ": " << value << std::endl;
                }

                std::cout << std::endl;
                if(headers.find("Host") == headers.end()) return;
                if(headers.find("Upgrade") == headers.end()) return;
                if(headers["Upgrade"] != "websocket") return;
                if(headers.find("Connection") == headers.end()) return;
                if(headers["Connection"] != "Upgrade") return;
                if(headers.find("Sec-WebSocket-Key") == headers.end()) return;
                if(headers.find("Sec-WebSocket-Version") == headers.end()) return;
                if(headers["Sec-WebSocket-Version"] != "13") return;

                std::string sec_websocket_key = headers["Sec-WebSocket-Key"];
                sec_websocket_key += MAGIC_GUID;
                std::cout << sec_websocket_key << std::endl;
                unsigned char digest[SHA_DIGEST_LENGTH] = {0};
                SHA1((unsigned char *)sec_websocket_key.data(), sec_websocket_key.size(), (unsigned char *)&digest);
                std::cout << digest << std::endl;

                std::string accept_key;

                char alphabet[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                                     'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                     'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                     'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                     '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

                int group24bit[7];
                for(int i = 0; i < 6; ++i){
                    group24bit[i] = digest[3*i];
                    group24bit[i] <<= 8;
                    group24bit[i] |= digest[3*i+1];
                    group24bit[i] <<= 8;
                    group24bit[i] |= digest[3*i+2];
                }
                group24bit[6] = digest[3*6];
                group24bit[6] <<= 8;
                group24bit[6] |= digest[3*6+1];
                group24bit[6] <<= 8;
                for(int i = 0; i < 6; ++i){
                    accept_key += alphabet[group24bit[i] >> 18];
                    accept_key += alphabet[group24bit[i] >> 12 & 0b00111111];
                    accept_key += alphabet[group24bit[i] >> 6 & 0b00111111];
                    accept_key += alphabet[group24bit[i] & 0b00111111];
                }

                accept_key += alphabet[group24bit[6] >> 18];
                accept_key += alphabet[group24bit[6] >> 12 & 0b00111111];
                accept_key += alphabet[group24bit[6] >> 6 & 0b00111111];
                accept_key += '='; //special case instead of the 6 bits that are padded zeroes



                accept_key += "\r\n"; //because 162+8 is divisible by 24, so we're on the 8 special case
                std::string handshake =
                        "HTTP/1.1 101 Switching Protocols\r\n"
                        "Upgrade: websocket\r\n"
                        "Connection: Upgrade\r\n"
                        "Sec-WebSocket-Accept: ";

                handshake += accept_key;
                handshake += "\r\n";

                std::cout << handshake;


                auto write_buffer = std::make_shared<boost::asio::streambuf>();
                std::ostream write_stream(write_buffer.get());
                write_stream << handshake;

                async_write(*socket, *write_buffer, [this, socket, write_buffer](const boost::system::error_code &ec, size_t) {
                    if (!ec) {
                        std::cout << "server: Switching Protocols with client" << std::endl;
                        std::string temp;
                        handle_connection(socket);
                    }
                });

            }
        });
    }

public:
    WebServer() : endpoint(tcp::v4(), 8080), acceptor(io_service, endpoint){}

    void start(){
        accept_request();
        io_service.run();
    }

};


int main() {

    WebServer server;
    server.start();

    return 0;
}