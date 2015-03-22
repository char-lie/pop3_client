#include "PostProvider.hpp"

namespace post {

    // Post Exception error methods
    PostException::PostException () : exception() {
    }

    // Invalid authorization error methods
    IncorrectAuthorizationDataException::IncorrectAuthorizationDataException (
                bool loginIncorrect, bool passwordIncorrect) : PostException() {
        this->loginIncorrect    = loginIncorrect;
        this->passwordIncorrect = passwordIncorrect;
    }

    const char* IncorrectAuthorizationDataException::what () const throw() {
        string answer;
        if ((this->loginIncorrect | this->passwordIncorrect) == 0) {
            answer += "Incorrect login and/or password";
        }
        if (this->loginIncorrect) {
            answer = "Incorrect login";
        }
        if (this->passwordIncorrect && answer == "") {
            answer = "Incorrect password";
        }
        else if (this->passwordIncorrect && answer != "") {
            answer += "and password";
        }
        answer += ".";
        return answer.c_str();
    }

    // Icorrect state error methods
    IncorrectStateException::IncorrectStateException (State actual,
                                        State required) : PostException() {
        this->required  = required;
        this->actual    = actual;
    }

    const char* IncorrectStateException::what () const throw() {
        string result;
        bool invalidState = (this->actual == UNKNOWN ||
                             this->required == UNKNOWN);
        if (!invalidState) {
            try {
                result = "Required state " + stateToString(this->required) +
                         " but " + stateToString(this->actual) + " is actual.";
            }
            catch (IncorrectStateException) {
                invalidState = true;
            }
        }
        if (invalidState) {
            result = "Invalid state";
        }
        return result.c_str();
    }

    // Post Provider methods
    PostProvider::PostProvider () {
        this->setState(DISCONNECTED);
    }

    PostProvider::PostProvider (p_TLP transportLayerProvider) : PostProvider() {
        this->setTransportLayerProvider(transportLayerProvider);
        this->setState(LOGIN_REQUIRED);
    }

    PostProvider::~PostProvider () {
    }

    void PostProvider::setState (State state) {
        this->state = state;
    }
    void PostProvider::checkState (State required) throw(PostException) {
        if ((this->state & required) == 0) {
            throw IncorrectStateException(required, this->state);
        }
    }

    string PostProvider::send (string message) throw(PostException) {
        return this->transportLayerProvider->send(message);
    }

    void PostProvider::setTransportLayerProvider (p_TLP transportLayerProvider)
                                            throw(PostException) {
        this->transportLayerProvider = transportLayerProvider;
        this->setState(LOGIN_REQUIRED);
    }

    // Other functions
    string stateToString (State state) throw(IncorrectStateException) {
        string result;
        switch (state) {
            case UNKNOWN:
                result = "UNKNOWN";
                break;
            case DISCONNECTED:
                result = "DISCONNECTED";
                break;
            case LOGIN_REQUIRED:
                result = "LOGIN_REQUIRED";
                break;
            case PASSWORD_REQUIRED:
                result = "PASSWORD_REQUIRED";
                break;
            case AUTHORIZED:
                result = "AUTHORIZED";
                break;
            default:
                throw IncorrectStateException(UNKNOWN, UNKNOWN);
                break;
        }
        return result;
    }

}
