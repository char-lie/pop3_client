#pragma once
#include <boost/program_options.hpp>
#include <iostream>

using namespace boost::program_options;
using namespace std;

namespace utils {
    /**
     * Prepare command line arguments processing.
     */
    options_description prepareCommandLineOptions ();
    /**
     * Parse command line.
     * @param argumentsCount Number of command line arguments:
     * first parameter in entry point function (main).
     * @param arguments Arguments with parameters:
     * second parameter in entry point function (main).
     * @param description Command line parameters description.
     */
    variables_map parseCommandLine (int argumentsCount, char* arguments[],
                                    options_description& description);
    /**
     * Display help menu.
     * @param out Stream to write help to.
     * @param description Parameters description to describe.
     * @param app Application name for usage text.
     */
    void displayUsage (ostream& out, options_description& description,
                       char app[]);
    /**
     * Get parameters' values from variables map.
     * @param variablesMap Variables map to read parameters from it.
     * @param login Reference to write login to it.
     * @param param Reference to write password to it.
     * @param server_name Reference to write server name to it.
     * @return Returns `true' in the case of success reading,
     * returns `false' otherwise.
     */
    bool getParameters (variables_map& variablesMap, string& login,
                        string& password, string& server_name);
}
