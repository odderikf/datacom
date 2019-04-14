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
                handle_request(socket);
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
                std::cout << message << std::endl;

                auto write_buffer = std::make_shared<boost::asio::streambuf>();
                std::ostream write_stream(write_buffer.get());

                std::string response = "HTTP/1.1 200 OK\r\n\r\n<html>\r\n<body>\r\n";
                response += "<h1>Velkommen</h1>\r\n";
                response += "<ul>";
                while(std::getline(read_stream, message)){
                    message.pop_back();
                    if(message != "")
                        response += ("<li>" + message + "</li>\r\n");
                }
                response += "</ul>\r\n";
                response += "</body>\r\n</html>\r\n\r\n";
                write_stream << response;

                async_write(*socket, *write_buffer, [=](const boost::system::error_code &ec, size_t) {
                    if (!ec) {
                        std::cout << "server: reply sent to client" << std::endl;
                    }
                });

            }
        });
    }

public:
    WebServer() : endpoint(tcp::v4(), 80), acceptor(io_service, endpoint){}

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
