#pragma once
#include <string>
#include <exception>

using namespace std;

namespace utils {
    /**
     * Thrown when server name can not be parsed.
     */
    class BadServerName : public std::exception {
        protected:
            string message;
        public:
            BadServerName (string message);
            virtual const char* what() const throw();
    };
    /**
     * Parse server name: "host:port" -> "host" and "port".
     * @parameter server_name Raw server name which needs to be parsed.
     * @parameter host String to hold host name.
     * @parameter port String to hold port.
     * @throws BadServerName Thrown if server name can not be parsed.
     */
    void parseServerName(const string& server_name, string& host, string& port)
                        throw(BadServerName);
}
