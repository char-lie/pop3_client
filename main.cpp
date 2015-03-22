#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include "ac_includes.hpp"
#include "boost_tools/tls.hpp"
#include "pp/pop3.hpp"
#include "utils/command_line.hpp"
#include "utils/server_name_parsing.hpp"

using namespace boost::program_options;
using namespace utils;
using namespace std;

int main(int argumentsCount, char* arguments[]) {
    /**
     * Prepare command line arguments processing.
     */
    options_description description = prepareCommandLineOptions ();
    variables_map variablesMap;
    try {
        variablesMap = parseCommandLine(argumentsCount, arguments, description);
    }
    catch (unknown_option e) {
        cerr << "An error occured: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    /**
     * Display help menu if it was requested.
     */
    if (variablesMap.count("help")) {
        displayUsage (cerr, description, arguments[0]);
        exit(EXIT_FAILURE);
    }
    /**
     * Process variables.
     */
    string login, password, server_name;
    if (!getParameters(variablesMap, login, password, server_name)) {
        cerr << "Not all mandatory parameters were set." << endl
             << "Please, run `" << arguments[0]
             << " --help' for more information." << endl;
        exit(EXIT_FAILURE);
    }
    string host, port;
    parseServerName(server_name, host, port);
    /**
     * Create Mail Client.
     */
    mail_client::MailClient mc(p_PP(new POP3PostProvider(
                              p_TLP(new TLSTransportLayerProvider()))));
    // Connect
    mc.connect(host, port);
    if (password != "") {
        mc.signin(login, password);
    }
    else {
        mc.sendLogin(login);
        // Enter password if required
        if (mc.isPasswordRequired()) {
            char* tmpPassword = getpass("Enter password: ");
            password = string(tmpPassword);
            free(tmpPassword);
        }
        mc.sendPassword(password);
    }
    /**
     * Read the list.
     */
    ofstream out("letters.txt");
    strings headers = mc.getLettersHeaders();
    for (auto e : mc.getLettersHeaders()) {
        out << e << endl;
    }
    out.close();
    cout << headers.size() << endl;
    /**
     * Close connection.
     */
    mc.signout();
    exit(EXIT_SUCCESS);
}
