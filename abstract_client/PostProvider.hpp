#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include "TransportLayerProvider.hpp"

using namespace std;

typedef vector<string> strings;

/**
 * Namespace which contains features, which are needed for Post Provider,
 * and Post Provider class itself.
 */
namespace post {

    /**
     * Post Provider states.
     */
    enum  State {
        UNKNOWN            = 0x0, // Plug for invalid state.
        DISCONNECTED       = 0x1, // Not connected to email server.
        LOGIN_REQUIRED     = 0x2, // Connected, but login required.
        PASSWORD_REQUIRED  = 0x4, // Login sent, password required.
        AUTHORIZED         = 0x8  // Connected and authorized successfully.
    };

    /**
     * Common class for Post namespace exceptions.
     */
    class PostException : public exception {
        public:
            PostException ();
            virtual const char* what () const throw() = 0;
    };

    /**
     * Thrown when Post Provider tries to perform action in wrong state.
     */
    class IncorrectStateException : public PostException {
        protected:
            /**
             * State which should be. If several are required, can contain any
             * of them.
             */
            State required;
            /**
             * Actual Post Provider state.
             */
            State actual;
        public:
            /**
             * Construct new exception with parameters described earlier.
             * @param actual Actual state of Post Provider. Leave it
             * `UNKNOWN' to throw "Invalid state" (e.g., for switch-case).
             * @param required Required state of Post Provider for current
             * action performing. Leave it `UNKNOWN' to throw "Invalid state"
             * (e.g., for switch-case).
             */
            IncorrectStateException (State actual = UNKNOWN,
                                     State required = UNKNOWN);
            virtual const char* what () const throw();
    };

    /**
     * Thrown when authorization data is incorrect.
     */
    class IncorrectAuthorizationDataException : public PostException {
        protected:
            /**
             * Set true if login is incorrect.
             */
            bool loginIncorrect;
            /**
             * Set true if password is incorrect.
             */
            bool passwordIncorrect;
        public:
            /**
             * Construct new authorization error.
             * If both parameters are `false', it means that one of or both
             * login and password are incorrect, but you don't know for sure.
             * @param loginIncorrect If you're sure that login is incorrect,
             * set this true.
             * @param passwordIncorrect If you're sure that password is
             * incorrect, set this true.
             */
            IncorrectAuthorizationDataException (
                bool loginIncorrect = false, bool passwordIncorrect = false);
            virtual const char* what () const throw();
    };

    /**
     * Thrown when server gives unexpected response.
     */
    class InvalidResponseException : public PostException {
        protected:
            /**
             * Server response.
             */
            string response;
        public:
            InvalidResponseException (string response);
            virtual const char* what() const throw();
    };

    /**
     * Post Provider class.
     * Abstract class for interacting with email server on application level.
     * Purpose: send commands according to protocol (POP3, IMAP) via
     * Transport Layer Provider (TCP, UDP) and give results to user.
     */
    class PostProvider {
        protected:
            /**
             * Current state is needed to know which actions are allowed
             * and which are restricted.
             */
            State state;
            /**
             * Setter for state.
             */
            void setState(State state);
            /**
             * Transport Layer Provider for communication with email server.
             */
            p_TLP transportLayerProvider;
            /**
             * State checking function for throwing exception.
             * Compares actual state (this->state) with required and throws 
             * exception if they're not fit. Otherwise does nothing.
             * Several states can be passed with binary `or', e.g.:
             * checkState(LOGIN_REQUIRED | PASSWORD_REQUIRED | AUTHORIZED);
             * @param required Required state.
             * @throw IncorrectStateException Thrown if current and required
             * states are not equal.
             */
            void checkState(State required) throw(PostException);
            /**
             * Send message via Transport Layer Provider.
             * Allowed in states AUTHORIZED, PASSWORD_REQUIRED, AUTHORIZED
             * (not allowed when DISCONNECTED).
             * @param message The message to send to email server.
             * @returns Answer of the server.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established
             */
            string send (string message) throw(PostException);
            /**
             * Checks wether mail server answered OK or not OK.
             * @param response Response to check.
             * @return Returns `true' if server answered "OK" and `false'
             * if server answered "not OK".
             * @throws InvalidResponseException Thrown if server response
             * wasn't recognised.
             */
            virtual bool isResponseOK(string response)
                                throw(PostException) = 0;
        public:
            /**
             * Construct new Post Provider.
             */
            PostProvider ();
            /**
             * Construct new Post Provider and set Transport Layer Provider.
             * @param transportLayerProvider Transport Layer Providier for
             * communicating with email server via it.
             */
            PostProvider (p_TLP transportLayerProvider);
            /**
             * Destruct this Post Provider:
             * should sign out (if needed) and close opened connection.
             */
            virtual ~PostProvider ();
            /**
             * Establish connection with email server.
             * Alowed in state DISCONNECTED.
             * Not allowed if Transport Layer Provider wasn't set.
             * @throws IncorrectStateException Thrown if connection was already
             * established.
             */
            void connect () throw(PostException);
            /**
             * Sign in to mailbox.
             * Allowed in state LOGIN_REQUIRED.
             * @param login User name.
             * @param password User password.
             * If password is blank, only user name will be sent.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established or if user is already authorized.
             */
            virtual void signin (string login, string password = "")
                    throw(PostException) = 0;
            /**
             * Send only login. If password required, it should be sent next.
             * Allowed in state LOGIN_REQUIRED.
             * @param login User name.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established or if user is already authorized.
             */
            virtual void sendLogin (string login) throw(PostException) = 0;
            /**
             * Send only password if it's needed after login was sent.
             * Allowed in state PASSWORD_REQUIRED.
             * @param password User password.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established, or user name wasn't sent, or if user is already
             * authorized.
             */
            virtual void sendPassword (string password)
                                      throw(PostException) = 0;
            /**
             * Sign out from mailbox.
             * Allowed in state AUTHORIZED.
             * @throws IncorrectStateException Thrown if not authorized.
             */
            virtual void signout () throw(PostException) = 0;
            /**
             * Get letters headers.
             * Allowed in state AUTHORIZED.
             * @throws IncorrectStateException Thrown if not authorized.
             */
            virtual strings getLettersHeaders () throw(PostException) = 0;
            /**
             * Set Transport Layer Provider.
             * Allowed in state DISCONNECTED.
             * @param transportLayerProvider Transport Layer Provider to send
             * messages via it.
             * @throws IncorrectStateException Thrown if Post Provider is
             * connected via another Transport Layer Provider.
             */
            void setTransportLayerProvider (p_TLP transportLayerProvider)
                                           throw(PostException);
    };

    /**
     * Convert State to string.
     * @param state State to be converted.
     * @return Stringified state name.
     * @throws IncorrectStateException Thrown when state is unknown.
     */
    string stateToString (State state) throw(IncorrectStateException);

    /**
     * Shortcut to Post Provider shared pointer
     */
    typedef shared_ptr<PostProvider>            p_PP;
}
