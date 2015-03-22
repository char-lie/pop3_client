#include "tls.hpp"
#include <iostream>
#include <exception>
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

void TLSTransportLayerProvider::connect (string server, string port)
                                        throw(TransportException) {
    this->checkConnectionState(false, "connect");
    system::error_code e;

    try {
        // Connect to server
        tcp::resolver resolver(this->i);
        tcp::resolver::query query(server, port);
        this->s->lowest_layer().connect(*resolver.resolve(query));
    }
    catch (boost::system::system_error) {
        throw ConnectionException("Unable to establish connection.");
    }

    try {
        // Handshake for TLS
        this->s->handshake(stream_base::client);
    }
    catch (...) {
        throw ConnectionException("Unable provide handshake.");
    }

    // Get response from the server
    boost::asio::streambuf response;
    read(*(this->s), response, transfer_at_least(1), e);
    this->connectionEstablished = true;
}

void TLSTransportLayerProvider::disconnect () throw(TransportException) {
    this->checkConnectionState(true, "disconnect");
    this->s->lowest_layer().close();
}

string TLSTransportLayerProvider::send (string message, string responseEnding)
                                       throw(TransportException) {
    this->checkConnectionState(true, "send a message");
    asio::streambuf response;
    system::error_code e;
    string result;

    // Transfer the message
    write(*(this->s), buffer(message, message.size()));
    // Read the response
    asio::read_until(*(this->s), response, responseEnding, e);
    // Convert the answer to string
    result = string((istreambuf_iterator<char>(&response)), 
                     istreambuf_iterator<char>());
    /*
    if (result.find(responseEnding) == string::npos) throw runtime_error(":'(");
    else {
        cerr << result.find(responseEnding);
        if (result.find(responseEnding) < 10)
            cerr << ": " << result;
        cerr << endl;
    }
    */
    return result;
}
