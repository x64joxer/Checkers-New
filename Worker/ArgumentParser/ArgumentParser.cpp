#include "ArgumentParser.h"

ArgumentParser::ArgumentParser()
{

}

void ArgumentParser::Parse(int argc, char* argv[])
{
    options_description desc("Allowed options");

    bool trafficMode;
    unsigned long long delayBetweenBestResultResponses;

    std::string serverIP;
    std::string serverPort;
    std::string logDirectory;

    desc.add_options()
    ("help,h", "Print usage message")
    ("traffic,t", bool_switch(&trafficMode), "Run in traffic mode (for test purposes)")
    ("delay,d", boost::program_options::value<unsigned long long>(&delayBetweenBestResultResponses), "Delay between best result responses")
    ("serverIP,i",  boost::program_options::value<std::string>(&serverIP), "Server IP")
    ("serverPort,p", boost::program_options::value<std::string>(&serverPort), "Server port")
    ("logDirectory,l", boost::program_options::value<std::string>(&logDirectory), "Log directory");

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

    if (vm.count("delay")) {
        ProgramVariables::SetDelayBetweenBestResultResponses(delayBetweenBestResultResponses);
    }

    if (vm.count("serverIP")) {
        ProgramVariables::SetIpForScheduler(serverIP);
    }

    if (vm.count("serverPort")) {
        ProgramVariables::SetPorForScheduler(serverPort);
    }

    if (vm.count("logDirectory")) {
        ProgramVariables::SetLogDirectory(logDirectory);
    }
}
