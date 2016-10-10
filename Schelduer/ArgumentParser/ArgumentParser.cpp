#include "ArgumentParser.h"

ArgumentParser::ArgumentParser()
{

}

void ArgumentParser::Parse(int argc, char* argv[])
{
    options_description desc("Allowed options");

    bool trafficMode;    

    desc.add_options()
    ("help,h", "Print usage message")
    ("traffic,t", bool_switch(&trafficMode), "Run in traffic mode (for test purposes)")    ;

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        exit(0);
    }


    if (vm.count("traffic")) {
        ProgramVariables::SetTrafficFlag(trafficMode);
    }

}
