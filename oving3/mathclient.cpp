#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class MathClient {
    boost::asio::io_service io_service;
    tcp::resolver resolver;

    void ask(std::shared_ptr<tcp::socket> const& socket){
        // write buffer with automatic memory management through reference counting
        auto write_buffer = std::make_shared<boost::asio::streambuf>();
        std::ostream write_stream(write_buffer.get());
        std::string message;

        std::getline(std::cin, message);
        write_stream << message << "\r\n"
                                   "";
        if(message == "exit") return;

        boost::asio::async_write(*socket, *write_buffer, [this, socket, write_buffer](const boost::system::error_code &ec, size_t) {
            // If not error:
            if (!ec) {
                auto read_buffer = std::make_shared<boost::asio::streambuf>();
                // Read from client until newline ("\r\n")
                boost::asio::async_read_until(*socket, *read_buffer, "\r\n", [this, socket, read_buffer](const boost::system::error_code &ec, size_t) {
                    // If not error:
                    if (!ec) {
                        std::string message;
                        std::istream read_stream(read_buffer.get());
                        std::getline(read_stream, message);
                        std::cout << message << std::endl;
                        ask(socket);
                    }
                });
            }
        });
    }

public:
    MathClient(const std::string &host, unsigned short port) : resolver(io_service) {
        // Create query from host and port
        auto query = tcp::resolver::query(host, std::to_string(port));
        // Resolve query (DNS-lookup if needed)
        resolver.async_resolve(query, [this](const boost::system::error_code &ec,
                                             boost::asio::ip::tcp::resolver::iterator it) {
            // If not error:
            if (!ec) {
                std::cout << "client: query resolved" << std::endl;

                auto socket = std::make_shared<tcp::socket>(io_service);

                boost::asio::async_connect(*socket, it, [this, socket](const boost::system::error_code &ec,
                                                                       boost::asio::ip::tcp::resolver::iterator /*it*/) {
                    // If not error:
                    if (!ec) {

                        std::cout << "client: connected to server" << std::endl;
                        std::cout << "Syntaks: a+b eller a-b" << std::endl;
                        ask(socket);

                    }
                });
            }
        });

        io_service.run();
    }
};

int main() {
    MathClient("127.0.0.1", 8081);
}