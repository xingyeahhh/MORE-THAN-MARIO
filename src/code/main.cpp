#include "../include/GameEngine.h"
#include <iostream>
#include <filesystem>

int main() {
  // Get the current working directory
  //std::filesystem::path cwd = std::filesystem::current_path();
  // Output it to the console
  //std::cout << "Current Working Directory: " << cwd << std::endl;



  GameEngine g("../../../bin/assets.txt");
  g.run();
}
