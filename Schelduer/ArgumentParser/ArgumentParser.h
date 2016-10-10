#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <boost/program_options.hpp>
#include <iostream>
#include "../ProgramVariables/ProgramVariables.h"

using namespace boost::program_options;
using namespace std;

class ArgumentParser
{
    public:
        ArgumentParser();
        static void Parse(int argc, char* argv[]);
};

#endif // ARGUMENTPARSER_H
