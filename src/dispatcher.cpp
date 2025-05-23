#include "dispatcher.hpp"

#include <cstdlib>
#include <iostream>

#include "console.hpp"
#include "screen.hpp"
#include "commands.hpp"

void dispatch(Commands cmd, std::vector<std::string>& args, Config& cfg,
              std::unordered_set<PCB>& procs, bool& screenSession) {
  switch (cmd) {
    case Commands::Initialize:
      cfg = Config::fromFile(args.empty() ? "config.txt" : args[0]);
      break;
    case Commands::SchedulerStop:
    case Commands::SchedulerTest:
    case Commands::ReportUtil:
        std::cout << to_str(cmd) << " command recognized. Doing something." << std::endl;
        break;
    case Commands::Screen:
      screen(args, procs, screenSession);
      break;
    
    case Commands::Clear:
      //std::cout <<  "\033[2J\033[1;1H";;
      system("clear");
      console_prompt();
      break;

    case Commands::Exit:
      std::exit(0);
  }
}
