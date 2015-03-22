#include <boost/program_options.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "ac_includes.hpp"
#include "boost_tools/tls.hpp"
#include "pp/pop3.hpp"

using namespace std;
namespace po = boost::program_options;

int main(int ac, char* av[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "display this help message")
        ("login,l", po::value<string>(), "username")
        ("password,p", po::value<string>()->default_value(""), "password (optional)")
        ("server_name,s", po::value<string>(), "host:port");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);
    }
    catch (boost::program_options::unknown_option e) {
        cerr << "An error occured: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    if (vm.count("help")) {
        cout << "Usage: " << av[0] << " [options]" << endl;
        cout << desc;
        exit(EXIT_FAILURE);
    }

    string login, password, server_name;

    if (vm.count("login") && vm.count("server_name")) {
        login = vm["login"].as<string>();
        server_name = vm["server_name"].as<string>();
        password = vm["password"].as<string>();
    }
    else {
        cerr << "Not all mandatory parameters were set." << endl
             << "Please, run `" << av[0] << " --help' for more information."
             << endl;
        exit(EXIT_FAILURE);
    }

    string host = server_name.substr(0, server_name.find(":"));
    string port = server_name.substr(server_name.find(":")+1,
                                     server_name.size());
    p_TLP tls(new TLSTransportLayerProvider());
    tls->connect(host, port);

    if (password == "") {
        char* tmpPassword = getpass("Enter password: ");
        password = string(tmpPassword);
        free(tmpPassword);
    }

    post::p_PP pop3(new POP3PostProvider(tls));
    pop3->signin(login, password);

    MailClient mc(pop3);
    //tls->send("LIST\r\n");
    tls->send("TOP 1 0\r\n", ".\r\n");
    mc.signout();
    exit(EXIT_SUCCESS);
}
