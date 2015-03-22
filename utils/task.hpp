#pragma once
#include "../ac_includes.hpp"

using namespace mail_client;

namespace utils {
    /**
     * Enter the mailbox and return pointer to mail client.
     * @param host Email server host.
     * @param port Email service port.
     * @param login User login.
     * @param password User password.
     * @return Returns shared pointer to new mailbox client.
     * @throws MailClientException Thrown if something's gone wrong with
     * Mail Client.
     */
    p_MC mailboxEnter (const string& host,  const string& port,
                       const string& login, const string& password)
                      throw(MailClientException);
    /**
     * Read messages headers in a file and return their number.
     * @param mailClient Mail Client which is ready to get messages from
     * mailbox.
     * @param out Output stream which should contain headers.
     * @return Returns number of received messages.
     * @throws ios_base::failure Thrown if stream error occured.
     * @throws MailClientException Thrown if connection error or another
     * Mail Client problem ocured.
     */
    int getMessagesHeaders (const p_MC& mailClient, ostream& out);
    /**
     * Read needed command line parameters.
     * @param host Reference to email server host.
     * @param port Reference to email service port.
     * @param login User login.
     * @param password User password.
     * @return Returns EXIT_SUCCESS if no errors occured,
     * returns EXIT_FAILURE otherwise.
     */
    int getCommandLineParameters (int argumentsCount, char* arguments[],
                  string& host, string& port, string& login, string& password);
    /**
     * Complete my task: connect to server, get emails list, write it to
     * file `letters.txt' and display number of emails on display.
     * @param host Email server host.
     * @param port Email service port.
     * @param login User login.
     * @param password User password.
     * @return Returns EXIT_SUCCESS if no errors occured,
     * returns EXIT_FAILURE otherwise.
     */
    int task (const string& host,  const string& port,
              const string& login, const string& password);
}
