#include "MailClient.hpp"

namespace mail_client {

    // Mail Client Exception methods
    MailClientException::MailClientException (string message) : exception() {
        this->message = message;
    }

    const char* MailClientException::what() const throw() {
        return this->message.c_str();
    }

    // Null Post Provider Exception methods
    NullPostProviderException::NullPostProviderException () :
            MailClientException("Post Provider pointer should not be NULL.") {
    }

    // Closed Connection Exception methods
    ClosedConnectionException::ClosedConnectionException () :
                    MailClientException("Connection is needed to be opened.") {
    }

    // Closed Connection Exception methods
    ConnectionError::ConnectionError (string reason) :
          MailClientException("Can not open connection. Reason: " + reason) {
    }

    // Mail Client methods
    MailClient::MailClient () {
    }

    MailClient::MailClient (p_PP postProvider) throw(MailClientException) : MailClient() {
        this->setPostProvider(postProvider);
    }

    MailClient::~MailClient () {
    }

    void MailClient::connect (string host, string port)
                             throw(MailClientException) {
        try {
            this->postProvider->connect(host, port);
        }
        catch(const PostException& e) {
            throw ConnectionError(string(e.what()));
        }
    }

    void MailClient::signin (string login, string password)
                            throw(MailClientException) {
        if (!this->isConnected()) {
            throw ClosedConnectionException();
        }
        try {
            this->postProvider->signin(login, password);
        }
        catch(const PostException& e) {
            throw MailClientException("An error occured: " + string(e.what()));
        }
    }

    void MailClient::sendLogin (string login) throw(MailClientException) {
        if (!this->isConnected()) {
            throw ClosedConnectionException();
        }
        try {
            this->postProvider->sendLogin(login);
        }
        catch(const PostException& e) {
            throw MailClientException("An error occured: " + string(e.what()));
        }
    }

    void MailClient::sendPassword (string password) throw(MailClientException) {
        if (!this->isConnected()) {
            throw ClosedConnectionException();
        }
        try {
            this->postProvider->sendPassword(password);
        }
        catch(const PostException& e) {
            throw MailClientException("An error occured: " + string(e.what()));
        }
    }

    void MailClient::getLettersHeaders (strings& headers)
                                  throw(MailClientException) {
        if (!this->isConnected()) {
            throw ClosedConnectionException();
        }
        try {
            this->postProvider->getLettersHeaders(headers);
        }
        catch(const PostException& e) {
            throw MailClientException("An error occured: " + string(e.what()));
        }
    }

    void MailClient::getLettersHeadersParameters (strings& parameters,
               const string& parameterName) throw(MailClientException) {
        if (!this->isConnected()) {
            throw ClosedConnectionException();
        }
        try {
            this->postProvider->getLettersHeadersParameters(parameters,
                                                            parameterName);
        }
        catch(const PostException& e) {
            throw MailClientException("An error occured: " + string(e.what()));
        }
    }


    void MailClient::signout () throw(MailClientException) {
        if (!this->isConnected()) {
            throw ClosedConnectionException();
        }
        try {
            this->postProvider->signout();
        }
        catch(const PostException& e) {
            throw MailClientException("An error occured: " + string(e.what()));
        }
    }

    void MailClient::setPostProvider (p_PP postProvider)
                                     throw(MailClientException) {
        if (!postProvider) {
            throw NullPostProviderException();
        }
        this->postProvider = postProvider;
    }

    bool MailClient::isConnected () {
        if (!this->postProvider) {
            throw NullPostProviderException();
        }
        return this->postProvider->isConnected();
    }

    bool MailClient::isPasswordRequired () throw(MailClientException) {
        if (!this->postProvider) {
            throw NullPostProviderException();
        }
        return this->postProvider->isPasswordRequired();
    }

}
