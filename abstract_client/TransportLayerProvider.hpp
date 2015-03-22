#pragma once
#include <memory>
#include <string>
#include <exception>

using namespace std;

namespace transport {

    class TransportException : public exception {
        public:
            TransportException ();
            virtual const char* what () const throw() = 0;
    };

    class ConnectionException : public TransportException {
        protected:
            /**
             * Error message to show.
             */
            string message;
        public:
            ConnectionException (string message);
            virtual const char* what () const throw();
    };

    class IncorrectConnectionStateException : public TransportException {
        private:
            /**
             * Indicates if Transport Layer Provider should be
             * connected (`true') or disconnected (`false') to
             * perform needed action.
             */
            bool connectionStateRequired;
            /**
             * Action which user tried to perform.
             */
            string actionName;
        public:
            /**
             * Construct.
             */
            IncorrectConnectionStateException (bool connectionStateRequired,
                                               string actionName);
            virtual const char* what () const throw();
    };
    /**
     * Transport Layer Provider -- via this thing Post Provider communicates
     * with email server.
     * This can be TCP, UDP, TCP with TLS etc.
     */
    class TransportLayerProvider {
        protected:
            /**
             * Indicates whether connection is established (`true')
             * or not (`false').
             */
            bool connectionEstablished;
        public:
            /**
             * Construct.
             */
            TransportLayerProvider  ();
            /**
             * Close connection (if needed) and destruct.
             */
            virtual ~TransportLayerProvider () = 0;
            /**
             * Connect to server.
             * @param server Server IP or url.
             * @param port Port to connect to.
             */
            virtual void connect (string server, string port)
                                 throw(TransportException) = 0;
            /**
             * Send message to the server.
             * @param message Message to be sent.
             * @param responseEnding String which indicates end of server
             * response.
             * @return Response of the server.
             */
            virtual string send (string message, string responseEnding = "\r\n")
                                throw(TransportException) = 0;
            /**
             * Disconnect from the server.
             */
            virtual void disconnect () throw(TransportException) = 0;
            /**
             * Check whether connection is established or not.
             * @return Returns `true' if connection is established,
             * returns `false' otherwise.
             */
            bool isConnected ();
            /**
             * Compare actual and required connection state.
             * @param requiredState Required state for current action.
             * @param actionName Current action name for exception constructor.
             * @throws IncorrectConnectionStateException Thrown if required
             * state is not equal to actual state.
             */
            void checkConnectionState (bool requiredState, string actionName)
                                      throw(IncorrectConnectionStateException);
    };

    typedef shared_ptr<TransportLayerProvider>  p_TLP;
}
