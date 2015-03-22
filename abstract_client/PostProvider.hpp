#pragma once
#include <memory>
#include <vector>
#include <string>
#include <exception>
#include "TransportLayerProvider.hpp"

using namespace std;

typedef vector<string> strings;

namespace post {

    enum  State {DISCONNECTED       = 0x1,
                 LOGIN_REQUIRED     = 0x2,
                 PASSWORD_REQUIRED  = 0x4,
                 AUTHORIZED         = 0x8};

    class IncorrectStateException : public exception {
        protected:
            State required;
            State actual;
        public:
            IncorrectStateException (State required, State actual);
    };

    class PostProvider {
        protected:
            /**
             * Current state to know what actions are allowed
             * and what are restricted.
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
             * Allowed in state State::AUTHORIZED.
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
             * Destruct this Post Provider.
             */
            virtual ~PostProvider ();
            /**
             * Establish connection with email server.
             * Alowed in state State::DISCONNECTED.
             */
            void connect ();
            /**
             * Sign in mailbox.
             * Allowed in state State::LOGIN_REQUIRED.
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
             * Allowed in state State::LOGIN_REQUIRED.
             * @param login User name.
             * @throws IncorrectStateException Thrown if connection wasn't
             * established or if user is already authorized.
             */
            //virtual void sendLogin (string login)
            //                       throw(IncorrectStateException) = 0;
            /**
             * Send only password if it's needed after login was sent.
             * Allowed in state State::PASSWORD_REQUIRED
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

            void    setTransportLayerProvider (p_TLP transportLayerProvider);
    };

    typedef shared_ptr<PostProvider>            p_PP;
}
