#include "TransportLayerProvider.hpp"

namespace transport {

    // Transport Exceptions methods
    TransportException::TransportException () : exception() {
    }

    // Connection Exception methods
    ConnectionException::ConnectionException(string message) :
                          TransportException() {
        this->message = message;
    }

    const char* ConnectionException::what () const throw() {
        return this->message.c_str();
    }

    // Incorrect Connection State Exception methods
    IncorrectConnectionStateException::IncorrectConnectionStateException (
    bool connectionStateRequired, string actionName) : TransportException() {
        this->connectionStateRequired = connectionStateRequired;
        this->actionName = actionName;
    }

    const char* IncorrectConnectionStateException::what () const throw() {
        string result;
        result = "You tried to perform action `" + this->actionName + "'. " +
                 "You should " + (this->connectionStateRequired? "" : "dis") +
                 "connect.";
        return result.c_str();
    }
    // Transport Layer Provider methods
    TransportLayerProvider::TransportLayerProvider () {
        this->connectionEstablished = false;
    }

    TransportLayerProvider::~TransportLayerProvider () {
    }

    void TransportLayerProvider::checkConnectionState (bool requiredState,
         string actionName) throw(IncorrectConnectionStateException) {
        if (this->isConnected() != requiredState) {
            throw IncorrectConnectionStateException(requiredState, actionName);
        }
    }

    bool TransportLayerProvider::isConnected () {
        return this->connectionEstablished;
    }
}
