#include "ArgumentParser.h"

ArgumentParser::ArgumentParser()
{

}

void ArgumentParser::Parse(int argc, char* argv[])
{
    options_description desc("Allowed options");

    bool trafficMode;    
    std::string serverPort;
    unsigned short cores;

    desc.add_options()
    ("help,h", "Print usage message")    
    ("traffic,t", bool_switch(&trafficMode), "Run in traffic mode (for test purposes)")
    ("cores,c", boost::program_options::value<unsigned short>(&cores), "Max thread for message queue")
    ("port,p", boost::program_options::value<std::string>(&serverPort), "Server port");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        exit(0);
    }

    if (vm.count("traffic")) {
        ProgramVariables::SetTrafficFlag(trafficMode);
    }

    if (vm.count("cores"))
    {
        if (cores == 0)
        {
            cores = 1;
        }
        ProgramVariables::SetMaxThredForMessageQueue(cores);
    }

    if (vm.count("port")) {
        ProgramVariables::SetPorForScheduler(serverPort);
    }
}
