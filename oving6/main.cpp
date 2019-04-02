#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class WebServer{
    boost::asio::io_service io_service;
    tcp::endpoint endpoint;
    tcp::acceptor acceptor;

    void accept_request(){
        auto socket = std::make_shared<tcp::socket>(io_service);
        acceptor.async_accept(*socket, [this, socket](const boost::system::error_code &ec){
            accept_request();

            if(!ec){
                std::cout << "boop" << std::endl;
                handle_request(socket);
            }

        });
    }

    void handle_request(const std::shared_ptr<tcp::socket> &socket){
        auto read_buffer = std::make_shared<boost::asio::streambuf>();
        boost::asio::async_read_until(*socket, *read_buffer, "\r\n", [=](const boost::system::error_code &ec, size_t){
            if(!ec){
                std::string message;
                std::string failmessage;
                std::istream read_stream(read_buffer.get());
                std::getline(read_stream, message);
                message.pop_back();
                if(message == "exit") return;
                std::cout << message << std::endl;
                double a,b;
                bool isPlus = true;
                auto sep = message.find('+');
                if(sep == std::string::npos) {
                    sep = message.find('-');
                    if(sep == std::string::npos){
                        failmessage = "Couldn't find a + or -";
                    };
                    isPlus = false;
                }
                std::string s;
                try {
                    s = message.substr(0, sep);
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
                    s = message.substr(sep + 1);
                    b = std::stod(s);
                } catch(std::invalid_argument const &e) {
                    failmessage = "Invalid argument " + s;
                } catch(std::out_of_range) {
                    failmessage = "Second number too big/small";
                } catch (std::exception const& e) {
                    std::cout << typeid(e).name() << " from " << e.what() << std::endl;
                    return;
                }

                auto write_buffer = std::make_shared<boost::asio::streambuf>();
                std::ostream write_stream(write_buffer.get());
                if(failmessage.length()) write_stream << failmessage << "\r\n";
                else write_stream << (isPlus ? a+b : a-b) << "\r\n";

                // Write to client
                async_write(*socket, *write_buffer, [this, socket, write_buffer](const boost::system::error_code &ec, size_t) {
                    // If not error:
                    if (!ec) {
                        std::cout << "server: reply sent to client" << std::endl;
                        handle_request(socket);
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