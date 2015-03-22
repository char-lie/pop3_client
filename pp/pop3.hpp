#include "../ac_includes.hpp"

class POP3PostProvider : public post::PostProvider {
    public:
                    POP3PostProvider    ();
                    POP3PostProvider    (p_TLP transportLayerProvider);
                    ~POP3PostProvider   ();
        void        signin              (string login, string password = "")
                                        throw(post::IncorrectStateException);
        void        signout             ();
        strings     getLettersHeaders   ();
};
