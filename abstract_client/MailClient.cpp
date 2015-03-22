#include "MailClient.hpp"

MailClient::MailClient () {
}

MailClient::MailClient (p_PP postProvider) : MailClient() {
    this->setPostProvider(postProvider);
}

MailClient::~MailClient () {
}

void MailClient::signin (string login, string password) {
    this->postProvider->signin(login, password);
}

strings MailClient::getLettersHeaders () {
    return this->postProvider->getLettersHeaders();
}

void MailClient::signout () {
    this->postProvider->signout();
}

void MailClient::setPostProvider (p_PP postProvider) {
    this->postProvider = postProvider;
}
