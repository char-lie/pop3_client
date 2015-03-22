#include "pop3.hpp"

POP3PostProvider::POP3PostProvider () : PostProvider () {
}

POP3PostProvider::POP3PostProvider (p_TLP transportLayerProvider) :
                                    PostProvider (transportLayerProvider) {
}

POP3PostProvider::~POP3PostProvider () {
}

void POP3PostProvider::signin (string login, string password) throw(post::IncorrectStateException) {
    this->send("USER " + login + "\r\n");
    if (password != "") {
        this->send("PASS " + password + "\r\n");
    }
}

void POP3PostProvider::signout () {
    this->send("QUIT\r\n");
}


strings POP3PostProvider::getLettersHeaders () {
    strings strs;
    strs.push_back("Hello");
    return strs;
}
