#pragma once
#include <memory>
#include <string>

using namespace std;

class TransportLayerProvider {
    public:
                        TransportLayerProvider  ();
        virtual         ~TransportLayerProvider () = 0;
        virtual void    connect                 (string server, string port) = 0;
        virtual string  send (string message,
                              string responseEnding = "\r\n") = 0;
};

typedef shared_ptr<TransportLayerProvider>  p_TLP;
