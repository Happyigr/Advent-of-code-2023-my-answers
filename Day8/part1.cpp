#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>

typedef std::unordered_map<std::string, std::pair<std::string, std::string>>
    unm_sp_ss;

int main() {
  std::fstream file("input.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file" << '\n';
    return 1;
  }

  std::string dirs;
  std::getline(file, dirs);

  unm_sp_ss places;
  std::regex pattern(R"((\w+) = \((\w+), (\w+)\))");
  std::string line;
  while (std::getline(file, line)) {
    std::smatch match;
    if (std::regex_search(line, match, pattern)) {
      places[match[1]] = std::make_pair(match[2], match[3]);
    }
  }

  int steps = 0;
  std::string currentPlace = "AAA";
  while (currentPlace != "ZZZ") {
    for (char dir : dirs) {
      steps++;
      switch (dir) {
      case 'L':
        currentPlace = places[currentPlace].first;
        break;
      case 'R':
        currentPlace = places[currentPlace].second;
        break;
      }
      if (currentPlace == "ZZZ")
        break;
    }
  }

  std::cout << steps << '\n';
  return 0;
}
