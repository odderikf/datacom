#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

class MathClient {
    boost::asio::io_service io_service;
    udp::resolver resolver;

    void ask(udp::socket &socket, udp::endpoint &remote_endpoint){
        std::string message_out;
        while( std::cin >> message_out and message_out != "exit"){
            socket.send_to(boost::asio::buffer(message_out), remote_endpoint);
            boost::array<char, 256> message_in;
            socket.receive_from(boost::asio::buffer(message_in), remote_endpoint);
            std::cout << std::string(message_in.data()) << std::endl;
            ask(socket, remote_endpoint);
        }

    }

public:
    MathClient(const std::string &host, unsigned short port) : resolver(io_service) {
        // Create query from host and port
        auto query = udp::resolver::query(host, std::to_string(port));
        udp::endpoint remote_base(*resolver.resolve(query));

        udp::socket socket(io_service);
        socket.open(udp::v4());

        boost::array<char, 1> buf0 = {'h'};
        boost::array<char, 1> buf1;
        boost::system::error_code ec;

        socket.send_to(boost::asio::buffer(buf0), remote_base);
        udp::endpoint remote_endpoint;
        socket.receive_from(boost::asio::buffer(buf1), remote_endpoint);
        if(buf1[0] == 'h'){
            std::cout << remote_endpoint.address() << ':' << remote_endpoint.port() << std::endl;
            std::string message_out;
            ask(socket, remote_endpoint);
            io_service.run();
        }
    }
};

int main() {
    MathClient("127.0.0.1", 8081);

}