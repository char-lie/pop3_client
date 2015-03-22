#include "tls.hpp"
#include <iostream>
#include <boost/array.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;

TLSTransportLayerProvider::TLSTransportLayerProvider () :
                           TransportLayerProvider() {
    // Constructing socket ssl stream
    context c(context::sslv3_client);
    s.reset(new stream<tcp::socket>(this->i, c));
}

TLSTransportLayerProvider::~TLSTransportLayerProvider () {
}

void TLSTransportLayerProvider::connect (string server, string port) {
    system::error_code e;

    // Connect to server
    tcp::resolver resolver(this->i);
    tcp::resolver::query query(server, port);
    try {
        this->s->lowest_layer().connect(*resolver.resolve(query));
    }
    catch (...) {
        cerr << "Can not connect to the server "
             << server << ":" << port << endl;
        return;
    }
    // Handshake for TLS
    this->s->handshake(stream_base::client);
    // Get response from the server
    boost::asio::streambuf response;
    read(*(this->s), response, transfer_at_least(1), e);
    cerr << &response;
}

string TLSTransportLayerProvider::send (string message, string responseEnding) {
    asio::streambuf response;
    system::error_code e;
    string result;

    // Transfer the message via the socket
    write(*(this->s), buffer(message, message.size()));
    // Read socket's answer
    asio::read_until(*(this->s), response, responseEnding, e);
    // Convert the answer to string
    result = string((istreambuf_iterator<char>(&response)), 
                     istreambuf_iterator<char>());
    cerr << result;
    return result;
}
