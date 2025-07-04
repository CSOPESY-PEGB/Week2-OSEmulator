#include "console.hpp"

#include <iostream>

namespace osemu {

void console_prompt() {
  std::cout << R"(

█ ▄▄  ▄███▄     ▄▀  ███   
█   █ █▀   ▀  ▄▀    █  █  
█▀▀▀  ██▄▄    █ ▀▄  █ ▀ ▄ 
█     █▄   ▄▀ █   █ █  ▄▀ 
 █    ▀███▀    ███  ███   
  ▀                       
                          
)";
  std::cout << "\e[1;32mBy: {Paul Ivan Enclonar, Joel Ethan Batac, Joshua Gilo, Peter Parker} \n";
  std::cout << "OS Emulator v0.1\n"; 
  std::cout << "\e[1;32mHello, Welcome to PEGP Command line! \e[0m " << std::endl;
  std::cout << "\e[3;33mType 'exit' to quit, 'clear' to clear the screen.\e[0m" << std::endl;
}

}
