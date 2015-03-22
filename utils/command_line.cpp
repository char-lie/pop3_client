#include "command_line.hpp"

namespace utils {

    options_description prepareCommandLineOptions () {
        options_description description("Allowed options");
        description.add_options()
            ("help,h", "display this help message")
            ("login,l", value<string>(), "username")
            ("password,p", value<string>()->default_value(""),
             "password (optional)")
            ("server_name,s", value<string>(), "host:port");
        return description;
    }

    variables_map parseCommandLine (int argumentsCount, char* arguments[],
                                    options_description& description) {
        variables_map variablesMap;
        store(parse_command_line(argumentsCount, arguments, description),
              variablesMap);
        notify(variablesMap);
        return variablesMap;
    }

    void displayUsage (ostream& out, options_description& description,
                       char app[]) {
        out << "Usage: " << app << " [options]" << endl;
        out << description;
    }

    bool getParameters (variables_map& variablesMap, string& login,
                        string& password, string& server_name) {
        if (!(variablesMap.count("login")
              && variablesMap.count("server_name"))) {
            return false;
        }
        else {
            login = variablesMap["login"].as<string>();
            password = variablesMap["password"].as<string>();
            server_name = variablesMap["server_name"].as<string>();
            return true;
        }
    }
}
