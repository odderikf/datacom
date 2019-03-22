#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

class WebServer{
    boost::asio::io_service io_service;
    udp::endpoint endpoint;
    udp::socket base_socket;


    void accept_request(){
        auto remote_endpoint = std::make_shared<udp::endpoint>();
        auto buf = std::make_shared<boost::array<unsigned char, 1>>();
        base_socket.async_receive_from(boost::asio::buffer(*buf), *remote_endpoint, 0, [=](boost::system::error_code ec, std::size_t s){
            accept_request();
            if(!ec && (*buf)[0] == 'h'){
                std::cout << remote_endpoint->address() << ':' << remote_endpoint->port() << std::endl;
                auto socket = std::make_shared<udp::socket>(io_service);
                socket->open(udp::v4());
                socket->send_to(boost::asio::buffer(*buf), *remote_endpoint, 0);
                handle_request(socket, remote_endpoint);
            }
        });
    }

    void handle_request(std::shared_ptr<udp::socket> const& socket, std::shared_ptr<udp::endpoint> const& remote_endpoint){
        auto buf = std::make_shared<boost::array<char, 256>>();
        socket->async_receive_from(boost::asio::buffer(*buf), *remote_endpoint, [=](const boost::system::error_code &ec, size_t s){
            if(!ec){
                std::string message_in(buf->begin(), buf->begin()+s);
                std::string failmessage;
                if(message_in == "exit") return;
                else handle_request(socket, remote_endpoint);
                std::cout << message_in << std::endl;
                double a,b;
                bool isPlus = true;
                auto sep = message_in.find('+');
                if(sep == std::string::npos) {
                    sep = message_in.find('-');
                    if(sep == std::string::npos){
                        failmessage = "Couldn't find a + or -";
                    };
                    isPlus = false;
                }
                std::string s;
                try {
                    s = message_in.substr(0, sep);
                    a = std::stod(s);
                } catch(std::invalid_argument const &e){
                    failmessage = "Invalid argument " + s;
                } catch(std::out_of_range) {
                    failmessage = "First number too big/small";
                } catch (std::exception const& e) {
                    std::cout << typeid(e).name() << " from " << e.what() << std::endl;
                    return;
                }
                try {
                    s = message_in.substr(sep + 1);
                    b = std::stod(s);
                } catch(std::invalid_argument const &e) {
                    failmessage = "Invalid argument " + s;
                } catch(std::out_of_range) {
                    failmessage = "Second number too big/small";
                } catch (std::exception const& e) {
                    std::cout << typeid(e).name() << " from " << e.what() << std::endl;
                    return;
                }

                std::string message_out;

                if(failmessage.length()){
                    message_out = failmessage;
                }
                else {
                    message_out = std::to_string(isPlus ? a+b : a-b);
                }
                message_out += "\r\n";
                message_out += '\0';

                // Write to client
                socket->async_send_to(boost::asio::buffer(message_out), *remote_endpoint, [=](const boost::system::error_code &ec, size_t) {
                    // If not error:
                    if (!ec) {
                        std::cout << "server: reply sent to client" << std::endl;
                        std::cout << message_out << std::endl;
                    }
                });

            }
        });
    }

public:
    WebServer() : endpoint(udp::v4(), 8081), base_socket(io_service, endpoint){}

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