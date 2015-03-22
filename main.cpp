#include <iostream>
#include "utils/task.hpp"

using namespace utils;
using namespace std;

int main(int argumentsCount, char* arguments[]) {
    int exitCode;
    string login, password, host, port;
    exitCode = getCommandLineParameters(argumentsCount, arguments, host, port,
                                        login, password);
    if (exitCode != EXIT_SUCCESS) {
        exit(exitCode);
    }
    exitCode = task(host, port, login, password);
    if (exitCode != EXIT_SUCCESS) {
        exit(exitCode);
    }
    exit(EXIT_SUCCESS);
}
