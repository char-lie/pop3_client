#pragma once
#include <memory>
#include <vector>
#include <string>
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
        DISCONNECTED       = 0x1, // Not connected to email server.
        LOGIN_REQUIRED     = 0x2, // Connected, but login required.
        PASSWORD_REQUIRED  = 0x4, // Login sent, password required.
        AUTHORIZED         = 0x8  // Connected and authorized successfully.
    }

    /**
     * Exception to throw when Post Provider tries to perform action in wrong
     * state.
     */
    class IncorrectStateException : public exception {
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
             */
            IncorrectStateException (State required, State actual);
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
            void checkState(State required) throw(IncorrectStateException);
            /**
             * Send message via Transport Layer Provider.
             * Allowed in states AUTHORIZED, PASSWORD_REQUIRED, AUTHORIZED
             * (not allowed when DISCONNECTED).
             * @param message The message to send to email server.
             * @returns Answer of the server.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established
             */
            string send (string message) throw(IncorrectStateException);
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
            void connect () throw(IncorrectStateException);
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
                                throw(IncorrectStateException) = 0;
            /**
             * Send only login. If password required, it should be sent next.
             * Allowed in state LOGIN_REQUIRED.
             * @param login User name.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established or if user is already authorized.
             */
            //virtual void sendLogin (string login)
            //                       throw(IncorrectStateException) = 0;
            /**
             * Send only password if it's needed after login was sent.
             * Allowed in state PASSWORD_REQUIRED.
             * @param password User password.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established, or user name wasn't sent, or if user is already
             * authorized.
             */
            //virtual void sendPassword (string password)
            //                          throw(IncorrectStateException) = 0;
            /**
             * Sign out from mailbox.
             */
            virtual void signout () = 0;
            /**
             * Get letters headers.
             */
            virtual strings getLettersHeaders () = 0;
            /**
             * Set Transport Layer Provider.
             * Allowed in state DISCONNECTED.
             * @param transportLayerProvider Transport Layer Provider to send
             * messages via it.
             * @throws IncorrectStateException Thrown if Post Provider is
             * connected via another Transport Layer Provider.
             */
            void setTransportLayerProvider (p_TLP transportLayerProvider)
                                           throw(IncorrectStateException);
    };

    /**
     * Shortcut to Post Provider shared pointer
     */
    typedef shared_ptr<PostProvider>            p_PP;
}
