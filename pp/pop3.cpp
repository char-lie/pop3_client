#include "pop3.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

using namespace boost;

namespace post {
    POP3PostProvider::POP3PostProvider () : PostProvider () {
    }

    POP3PostProvider::POP3PostProvider (p_TLP transportLayerProvider) :
                                        PostProvider (transportLayerProvider) {
    }

    POP3PostProvider::~POP3PostProvider () {
    }


    bool POP3PostProvider::isResponseOK(string response) throw(PostException) {
        if (starts_with(response, "+OK")) {
            return true;
        }
        else if (starts_with(response, "-ERR")) {
            return false;
        }
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
        if (this->isResponseOK(response)) {
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
        if (this->isResponseOK(response)) {
            this->setState(AUTHORIZED);
        }
        else {
            throw IncorrectAuthorizationDataException(false, false);
        }
    }

    void POP3PostProvider::signout () throw(PostException) {
        string response;
        this->checkState(AUTHORIZED);
        response = this->send("QUIT\r\n");
        if (this->isResponseOK(response)) {
            this->setState(LOGIN_REQUIRED);
        }
    }

    void POP3PostProvider::getEmailsIDs (strings& result)
                                        throw(PostException) {
        string response;
        result.clear();
        this->checkState(AUTHORIZED);
        /**
         * Get raw list of emails from server.
         */
        response = this->send("LIST\r\n", "\r\n.\r\n");
        if (!this->isResponseOK(response)) {
            throw ConnectionException("Server responsed negatively. "
                                      "Reason's unknown.");
        }
        strings emailInfo;
        int emailsCount;
        try {
            trim(response);
            split(emailInfo, response, is_any_of(" "), token_compress_on);
            emailsCount = stoi(emailInfo[1]);
        }
        catch (invalid_argument) {
            throw ConnectionException("Server response was strange: "
                                      "can't get emails count.");
        }
        catch (out_of_range) {
            throw ConnectionException("Server response was strange: "
                                      "you have a huge number of emails.");
        }
        /**
         * Prepare list of emails' IDs.
         */
        strings emailsList;
        split(emailsList, response, is_any_of("\r\n"), token_compress_on);
        emailsList.erase(emailsList.begin());
        emailsList.pop_back();
        string currentHeader;
        for (string email : emailsList) {
            trim(email);
            split(emailInfo, email, is_any_of(" "), token_compress_on);
            result.push_back(emailInfo[0]);
        }
    }

    void POP3PostProvider::getLettersHeaders (strings& headers)
                                        throw(PostException) {
        strings emailsIDs;
        string response;
        headers.clear();

        this->checkState(AUTHORIZED);

        this->getEmailsIDs(emailsIDs);
        string currentHeader;
        for (string emailID : emailsIDs) {
            currentHeader = this->send("TOP " + emailID + " 0\r\n",
                                       "\r\n.\r\n");
            if (!isResponseOK(currentHeader)) {
                string message = "Can't get message " + emailID + ". "
                                 "Maybe connection was lost?";
                throw ConnectionException(message);
            }
            else {
                headers.push_back(currentHeader);
            }
        }
    }
}
