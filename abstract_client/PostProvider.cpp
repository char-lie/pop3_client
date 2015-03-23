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
        bool invalidState = (this->actual == UNKNOWN &&
                             this->required == UNKNOWN);
        try {
            string strRequired = stateToString(this->required);
            string strActual = stateToString(this->actual);
        }
        catch (IncorrectStateException) {
            invalidState = true;
        }
        if (!invalidState) {
            result = "Required state is " + stateToString(this->required);
            if (this->required != UNKNOWN) {
                result += " but " + stateToString(this->actual) + " is actual.";
            }
            else {
                result += ".";
            }
        }
        else {
            result = "Invalid state";
        }
        return result.c_str();
    }

    // Connection Error methods
    ConnectionError::ConnectionError (string message) : PostException() {
        this->message = message;
    }

    const char* ConnectionError::what () const throw() {
        return this->message.c_str();
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

    void PostProvider::connect(string host, string port) throw(PostException) {
        if (!this->transportLayerProvider) {
            throw ConnectionError("Transport Layer Provider wasn't set");
        }
        try {
            this->transportLayerProvider->connect(host, port);
        }
        catch (const TransportException& e) {
            string message = string(e.what());
            throw ConnectionError(message);
        }
        this->setState(LOGIN_REQUIRED);
    }

    void PostProvider::setState (State state) {
        this->state = state;
    }
    void PostProvider::checkState (int required) throw(PostException) {
        if ((this->state & required) == 0) {
            throw IncorrectStateException(this->state, UNKNOWN);
        }
    }

    void PostProvider::checkState (State required) throw(PostException) {
        if ((this->state & required) == 0) {
            throw IncorrectStateException(this->state, required);
        }
    }

    string PostProvider::send (string message, string responseEnding)
                              throw(PostException) {
        return this->transportLayerProvider->send(message, responseEnding);
    }

    void PostProvider::setTransportLayerProvider (p_TLP transportLayerProvider)
                                            throw(PostException) {
        this->transportLayerProvider = transportLayerProvider;
        this->setState(LOGIN_REQUIRED);
    }

    bool PostProvider::isConnected () {
        return this->transportLayerProvider->isConnected();
    }

    bool PostProvider::isPasswordRequired () {
        return (this->state == PASSWORD_REQUIRED);
    }
    
    void PostProvider::getLettersHeadersParameters (strings& parameters,
                      const string& parameterName) throw(PostException) {
        strings headers;
        parameters.clear();
        int valueStart, valueLength;
        this->getLettersHeaders(headers);
        for (string header : headers) {
            valueStart = header.find(parameterName + ": ");
            if (valueStart == string::npos) {
                parameters.push_back("");
                continue;
            }
            valueStart += parameterName.size() + 2;
            valueLength = header.find("\r\n", valueStart) - valueStart;
            parameters.push_back(header.substr(valueStart, valueLength));
        }
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
