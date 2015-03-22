#pragma once
#include <memory>
#include <vector>
#include <string>
#include "PostProvider.hpp"

using namespace std;
using namespace post;

typedef vector<string> strings;

class MailClient {
    protected:
        p_PP  postProvider;
    public:
                    MailClient          ();
                    MailClient          (p_PP postProvider);
        virtual     ~MailClient         ();
        void        signin              (string login, string password);
        void        signout             ();
        strings     getLettersHeaders   ();
        void        setPostProvider     (p_PP postProvider);
};
