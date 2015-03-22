#include <cstdlib>
#include <fstream>
#include "../boost_tools/tls.hpp"
#include "../pp/pop3.hpp"
#include "command_line.hpp"
#include "server_name_parsing.hpp"
#include "task.hpp"

using namespace boost::program_options;

namespace utils {

    p_MC mailboxEnter (const string& host,  const string& port,
                       const string& login, const string& password)
                      throw(MailClientException) {
        p_TLP transportLayerProvider(new TLSTransportLayerProvider());
        p_PP postProvider(new POP3PostProvider(transportLayerProvider));
        p_MC mailClient(new MailClient(postProvider));
        mailClient->connect(host, port);
        if (password != "") {
            mailClient->signin(login, password);
        }
        else {
            mailClient->sendLogin(login);
            // Enter password if required
            if (mailClient->isPasswordRequired()) {
                // Bad method, but nothing better was found
                char* tmpPassword = getpass("Enter password: ");
                mailClient->sendPassword(tmpPassword);
                // That method was really bad...but C-like memory design
                // is not the worse thing in C++11 code, huh?
                free(tmpPassword);
            }
        }
        return mailClient;
    }

    int getMessagesHeaders (const p_MC& mailClient, ostream& out) {
        strings headers = mailClient->getLettersHeaders();
        for (auto e : headers) {
            out << e << endl;
        }
        return headers.size();
    }

    int getCommandLineParameters (int argumentsCount, char* arguments[],
                  string& host, string& port, string& login, string& password) {
        /**
         * Prepare command line arguments processing.
         */
        options_description description = prepareCommandLineOptions ();
        variables_map variablesMap;
        try {
            variablesMap = parseCommandLine(argumentsCount, arguments, description);
        }
        catch (const unknown_option& e) {
            cerr << "An error occured: " << e.what() << endl;
            return EXIT_FAILURE;
        }
        catch (const invalid_command_line_syntax& e) {
            cerr << "An error occured: " << e.what() << endl;
            return EXIT_FAILURE;
        }
        /**
         * Display help menu if it was requested.
         */
        if (variablesMap.count("help")) {
            displayUsage (cerr, description, arguments[0]);
            return EXIT_FAILURE;
        }
        /**
         * Process variables.
         */
        string server_name;
        if (!getParameters(variablesMap, login, password, server_name)) {
            cerr << "Not all mandatory parameters were set." << endl
                 << "Please, run `" << arguments[0]
                 << " --help' for more information." << endl;
            return EXIT_FAILURE;
        }
        parseServerName(server_name, host, port);

        return EXIT_SUCCESS;
    }

    int task (const string& host,  const string& port,
              const string& login, const string& password) {
        p_MC mailClient;
        try {
            mailClient = mailboxEnter(host, port, login, password);
        }
        catch (const MailClientException& e) {
            cerr << "Error occured when tried to enter the mailbox: "
                 << e.what() << endl;
            return EXIT_FAILURE;
        }
        try {
            string outputFilename = "letters.txt";
            ofstream out(outputFilename);
            if (!out.is_open()) {
                throw ios_base::failure("Can't open file " +
                                        outputFilename + ".");
            }
            out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
            cout << getMessagesHeaders(mailClient, out) << endl;
            out.close();
        }
        catch (const ios_base::failure& e) {
            cerr << "Error occured when application worked with file: "
                 << e.what() << endl;
            return EXIT_FAILURE;
        }
        catch (const MailClientException& e) {
            cerr << "Mail Client error occured when tried to read messages: "
                 << e.what() << endl;
            return EXIT_FAILURE;
        }
        try {
            mailClient->signout();
        }
        catch (const MailClientException& e) {
            cerr << "Error occured when tried to sign out from mailbox: "
                 << e.what() << endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}
