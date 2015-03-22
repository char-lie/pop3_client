#include "PostProvider.hpp"

namespace post {

    IncorrectStateException::IncorrectStateException(State required,
                                        State actual) : exception() {
        this->required  = required;
        this->actual    = actual;
    }

    PostProvider::PostProvider () {
        this->state = State::DISCONNECTED;
    }

    PostProvider::PostProvider (p_TLP transportLayerProvider) : PostProvider() {
        this->setTransportLayerProvider(transportLayerProvider);
    }

    PostProvider::~PostProvider () {
    }

    void PostProvider::checkState(State required)
                            throw(IncorrectStateException) {
        if ((this->state & required) == 0) {
            throw IncorrectStateException(required, this->state);
        }
    }

    string PostProvider::send (string message) throw(IncorrectStateException) {
        return this->transportLayerProvider->send(message);
    }

    void PostProvider::setTransportLayerProvider (
                                                p_TLP transportLayerProvider) {
        this->transportLayerProvider = transportLayerProvider;
    }
}
