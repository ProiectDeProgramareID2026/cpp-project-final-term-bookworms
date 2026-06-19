#pragma once

#include "Library.h"

#include <iosfwd>
#include <string>

// Entry point for parsing and executing CLI commands.
class CliApplication {
public:
    int run(int argc, char* argv[]);

private:
    Library library_;

    int dispatch(int argc, char* argv[]);

    static void printUsage(std::ostream& out = std::cerr);
    static bool parsePositiveId(const char* arg, int& outId);
    static bool validateAddBookArgs(const std::string& title, const std::string& author,
                                    const std::string& type, const std::string& location);
};
