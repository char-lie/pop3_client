#pragma once
#include "../ac_includes.hpp"
#include <exception>

using namespace transport;

namespace post {

    /**
     * Post Provider for POP3 protocol.
     */
    class POP3PostProvider : public post::PostProvider {
        private:
            /**
             * Get IDs of emails to access them in future.
             * @param result Strings array reference, which will contain IDs.
             * @throws IncorrectStateException Thrown if state is not
             * AUTHORIZED.
             * @throws ConnectionException Thrown if server respond is strange.
             */
            void getEmailsIDs (strings& result) throw(PostException);
        protected:
            /**
             * Checks wether mail server response is OK or ERR.
             * @param response Response to check.
             * @return Returns `true' if server answered "+OK" and `false'
             * if server answered "-ERR".
             * @throws InvalidResponseException Thrown if server response
             * wasn't recognised as OK neither ERR.
             */
            bool isResponseOK(string response)
                        throw(PostException) {
                if (response.find("+OK") != string::npos) {
                    return true;
                }
                else if (response.find("-ERR") != string::npos) {
                    return false;
                }
            }
        public:
            /**
             * Descriptions of further methods can be found in Post Privider
             * class.
             * I'm not so lazy -- I just can forget to change comments here
             * if I'll change them in base class. Though yes, I'm lazy.
             */
            POP3PostProvider ();
            POP3PostProvider (p_TLP transportLayerProvider);
            ~POP3PostProvider ();
            void signin (string login, string password) throw(PostException);
            void sendLogin (string login) throw(PostException);
            void sendPassword (string password) throw(PostException);
            void signout () throw(PostException);
            strings getLettersHeaders () throw(PostException);
    };
}
