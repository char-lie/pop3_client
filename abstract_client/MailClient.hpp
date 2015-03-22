#pragma once
#include <memory>
#include <vector>
#include <string>
#include <exception>
#include "PostProvider.hpp"

using namespace std;
using namespace post;

typedef vector<string> strings;

namespace mail_client {

    /**
     * General class for Mail Client exceptions.
     */
    class MailClientException : public exception {
        protected:
            /**
             * Error message.
             */
            string message;
        public:
            MailClientException (string message);
            const char* what () const throw();
    };

    /**
     * Thrown when someone tries to set NULL Post Provider.
     */
    class NullPostProviderException : public MailClientException {
        public:
            NullPostProviderException ();
    };

    /**
     * Thrown if connection wasn't established or closed at the moment.
     */
    class ClosedConnectionException: public MailClientException {
        public:
            ClosedConnectionException ();
    };

    /**
     * Thrown if error occured during connection operation.
     */
    class ConnectionError: public MailClientException {
        public:
            /**
             * Construct new error.
             * @param reason Reason of connection openin inability.
             */
            ConnectionError (string reason);
    };

    /**
     * Mail Client is a high-level class for accessing mail server and
     * communicating with it.
     */
    class MailClient {
        protected:
            /**
             * Pointer to Post Provider object for Application Level
             * communication with email server.
             */
            p_PP  postProvider;
        public:
            /**
             * Construct Mail Client without Post Provider
             */
            MailClient ();
            /**
             * Construct Mail Client with not null Post Provider.
             * @parameter postProvider Post Provider for communication with
             * email server.
             * @throws NullPostProviderException Thrown when postProvider is
             * a NULL pointer.
             */
            MailClient (p_PP postProvider) throw(MailClientException);
            /**
             * Signout, disconnect (if needed) and destruct.
             */
            virtual ~MailClient ();
            /**
             * Connect to mail server.
             * @param server Server IP or url.
             * @param port Port to connect to.
             * @throws MailClientException Thrown if connection operation was
             * failed.
             */
            void connect (string host, string port) throw(MailClientException);
            /**
             * Sign in to mailbox.
             * @param login User name.
             * @param password User password.
             * @throws MailClientException Thrown if connection wasn't
             * established or if user is already authorized.
             */
            void signin (string login, string password)
                        throw(MailClientException);
            /**
             * Send only login. If password required, it should be sent next.
             * Allowed in state LOGIN_REQUIRED.
             * @param login User name.
             * @throws MailClientException Thrown if connection wasn't
             * established, if user is already authorized or if login is wrong.
             */
            void sendLogin (string login) throw(MailClientException);
            /**
             * Send only password if it's needed after login was sent.
             * Allowed in state PASSWORD_REQUIRED.
             * @param password User password.
             * @throws MailClientException Thrown if connection wasn't
             * established, if user is already authorized or if password is
             * wrong.
             */
            void sendPassword (string password) throw(MailClientException);
            /**
             * Sign out from mailbox.
             * @throws MailClientException Thrown if not authorized.
             */
            void signout () throw(MailClientException);
            /**
             * Get letters headers.
             * @throws MailClientException Thrown if not authorized.
             * @return Vector of strings. Each string contains header of one
             * letter.
             */
            strings getLettersHeaders () throw(MailClientException);
            /**
             * Set not null Post Provider.
             * @parameter postProvider Post Provider for communication with
             * email server.
             * @throws NullPostProviderException Thrown when postProvider is
             * a NULL pointer.
             */
            void setPostProvider (p_PP postProvider) throw(MailClientException);
            /**
             * Is Post Provider connected to email server?
             * @return Returns `true' if connected and `false' otherwise.
             */
            bool isConnected ();
            /**
             * Check whether password is needed now.
             * @return Returns `true' if you have to enter password now,
             * returns `false' otherwise.
             * @throws NullPostProviderException Thrown if Post Provider pointer
             * is NULL.
             */
            bool isPasswordRequired () throw(MailClientException);
    };

    /**
     * Shortcut for Mail Client pointer.
     */
    typedef shared_ptr<MailClient> p_MC;

}
