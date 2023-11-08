#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// localhost 10000 "Hello, world!"

// Sends a WebSocket message and prints the response
int main(int argc, char** argv)
{
    try
    {
        // Check command line arguments.
        if(argc != 4)
        {
            std::cerr <<
                "Usage: app <host> <port> <text>\n" <<
                "Example:\n" <<
                "    app echo.websocket.org 80 \"Hello, world!\"\n";
            return EXIT_FAILURE;
        }
        std::string host = argv[1];
        auto const  port = argv[2];
        auto const  text = argv[3];

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        auto ep = net::connect(ws.next_layer(), results);

        // Update the host string
        host += ':' + std::to_string(ep.port());

        // Perform the websocket handshake
        ws.handshake(host, "/");

        // This buffer will hold the incoming message
        beast::flat_buffer buffer;

        // Read a message into our buffer
        ws.read(buffer);

        // The make_printable() function helps print a ConstBufferSequence
        std::cout << "recv: " << beast::make_printable(buffer.data()) << std::endl;

        // Send the message
        // ws.write(net::buffer(std::string(text)));
        ws.write(net::buffer(std::string(text)));

        // Read a message into our buffer
        buffer.clear();
        ws.read(buffer);

        // The make_printable() function helps print a ConstBufferSequence
        std::cout << "recv: " << beast::make_printable(buffer.data()) << std::endl;

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}