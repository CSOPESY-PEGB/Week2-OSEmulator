#include "screen.hpp"
#include "parser.hpp"
#include "commands.hpp"
#include "console.hpp"

#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>

enum class ScreenCommand {
    Resume,
    Start,
    Unknown
};

static constexpr int EXPECTED_ARGS_COUNT = 2;

void display_usage() {
    std::cout << "Usage:\n"
              << "  screen -r <name>   Resume and display the named screen session\n"
              << "  screen -s <name>   Start a new screen session with the given name\n";
}

ScreenCommand parse_command(const std::string& cmd) {
    if (cmd == "-r") return ScreenCommand::Resume;
    if (cmd == "-s") return ScreenCommand::Start;
    return ScreenCommand::Unknown;
}

bool create_process(const std::string& process_name, std::unordered_set<PCB>& processes) {
    PCB pcb(process_name, /* initial priority or status */ 100);
    try {
        if (!processes.insert(pcb).second) {
            std::cerr << "Process already exists: " << process_name << "\n";
            return false;
        }
        std::cout << "Process " << pcb.processName << " created\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating process: " << e.what() << "\n";
        return false;
    }
}

void resume(const std::string& name, std::unordered_set<PCB>& processes, bool& screenSession){
    PCB probe(name, 0);
        auto it = processes.find(probe);
        if (it == processes.end()) {
            std::cout << "Couldn't find process named: " << name << "\n";
            return;
        }

        // Found it, enter interactive session
        screenSession = true;
        std::cout << "\x1b[2J\x1b[H";       // clear screen + home
        
        // Get non-const reference to modify PCB
        PCB& pcb = const_cast<PCB&>(*it);
        std::cout << "Resumed Process: " << pcb.status() << "\n\n";
        
        // Print history when entering session
        pcb.printHistory();

        std::string line;
        while (screenSession && (std::cout << "~ ") && std::getline(std::cin, line)) {
            auto tokens = parse_tokens(line);
            if (tokens.empty()) continue;

            std::string commandOutput = "";  // Capture output for this command

            try {
                Commands c = from_str(tokens.front());
                tokens.erase(tokens.begin());

                if (c == Commands::Exit) {
                    commandOutput = "Exiting session for process: " + pcb.processName;
                    // Don't print here - just store in history and exit
                    pcb.addToHistory(line, commandOutput);
                    screenSession = false;
                    std::cout << "\x1b[2J\x1b[H";
                    console_prompt();
                    break;  // Exit the loop without printing
                } else if (c == Commands::Clear) {
                    pcb.clearHistory();
                    std::cout << "\x1b[2J\x1b[H";  // Clear screen
                    std::cout << "Resumed Process: " << pcb.status() << "\n\n";
                    // Don't add clear to history since we just cleared it
                    continue;
                } else {
                    // handle other in-session commands here
                    commandOutput = "Unhandled command in screen session.";
                    std::cout << commandOutput << "\n";
                }
            } catch (const std::exception& ex) {
                commandOutput = ex.what();
                std::cerr << commandOutput << "\n";
            }

            // Add command and its output to history
            pcb.addToHistory(line, commandOutput);
        }
}

void screen(std::vector<std::string>& args,
            std::unordered_set<PCB>& processes,
            bool& screenSession)
{
    if (args.size() != EXPECTED_ARGS_COUNT) {
        display_usage();
        return;
    }

    const ScreenCommand cmd = parse_command(args[0]);
    const std::string& name = args[1];

    switch (cmd) {
      case ScreenCommand::Start:
        create_process(name, processes);
        resume(name, processes, screenSession);
        break;

    case ScreenCommand::Resume: {
        resume(name, processes, screenSession);
        break;
    }



      default:
        display_usage();
        break;
    }
}