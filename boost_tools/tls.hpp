#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <memory>
#include "../ac_includes.hpp"

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ssl;
using namespace boost::asio::ip;

namespace transport {
    class TLSTransportLayerProvider : public TransportLayerProvider {
        private:
            io_service i;
            std::shared_ptr<stream<ip::tcp::socket>> s;
        public:
            TLSTransportLayerProvider ();
            ~TLSTransportLayerProvider ();
            void connect (string server, string port) throw(TransportException);
            void disconnect () throw(TransportException);
            string send (string message, string responseEnding = "\r\n")
                        throw(TransportException);
    };
}
