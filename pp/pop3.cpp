#include "pop3.hpp"

namespace post {
    POP3PostProvider::POP3PostProvider () : PostProvider () {
    }

    POP3PostProvider::POP3PostProvider (p_TLP transportLayerProvider) :
                                        PostProvider (transportLayerProvider) {
    }

    POP3PostProvider::~POP3PostProvider () {
    }

    void POP3PostProvider::signin (string login, string password)
                                  throw(PostException) {
        this->sendLogin(login);
        this->sendPassword(password);
    }

    void POP3PostProvider::sendLogin (string login) throw(PostException) {
        string response;
        this->checkState(LOGIN_REQUIRED);
        response = this->send("USER " + login + "\r\n");
        if (isResponseOK(response)) {
            this->setState(PASSWORD_REQUIRED);
        }
        else {
            throw IncorrectAuthorizationDataException(true, false);
        }
    }

    void POP3PostProvider::sendPassword (string password) throw(PostException) {
        string response;
        this->checkState(PASSWORD_REQUIRED);
        response = this->send("PASS " + password + "\r\n");
        if (isResponseOK(response)) {
            this->setState(AUTHORIZED);
        }
        else {
            throw IncorrectAuthorizationDataException(false, false);
        }
    }

    void POP3PostProvider::signout () throw(PostException) {
        this->send("QUIT\r\n");
    }


    strings POP3PostProvider::getLettersHeaders () throw(PostException) {
        strings strs;
        strs.push_back("Hello");
        return strs;
    }
}
