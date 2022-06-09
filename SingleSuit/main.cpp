#include <iostream>
#include <sstream>
#include <api/api.hpp>

std::string readAllInput() {
  std::stringstream ss;
  std::string buffer;
  while (std::getline(std::cin, buffer)) {
    ss << buffer;
    if (std::cin.peek() != EOF) {
      ss << std::endl;
    }
  }
  return ss.str();
} 

int main() {

  auto input = readAllInput();
  try {
    std::cout << api::call_function(input) << std::endl;
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}