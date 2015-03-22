#include "server_name_parsing.hpp"

namespace utils {

    BadServerName::BadServerName (string message) : exception() {
        this->message = message;
    }

    const char* BadServerName::what () const throw() {
        return this->message.c_str();
    }

    void parseServerName(const string& server_name, string& host,
                         string& port) throw(BadServerName) {
        int colonPosition = server_name.find(":");
        if (colonPosition == string::npos) {
            throw BadServerName("Can't parse server name "
                                "`"+ server_name + "'.");
        }
        host = server_name.substr(0, colonPosition);
        port = server_name.substr(colonPosition+1, server_name.size());
    }
}
