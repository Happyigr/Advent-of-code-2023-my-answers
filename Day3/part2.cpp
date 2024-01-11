#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int CheckMap(const std::string &gameMap, int mapLength) {

  std::vector<std::pair<std::string, int>> DIRS{
      {"UP", -mapLength},          {"DOWN", mapLength},         {"LEFT", -1},
      {"UPLEFT", -mapLength - 1},  {"DOWNLEFT", mapLength - 1}, {"RIGHT", 1},
      {"UPRIGHT", -mapLength + 1}, {"DOWNRIGHT", mapLength + 1}};

  int ans = 0;

  for (int ind = 0; ind < gameMap.size(); ind++) {

    // if a symbol was find
    if (gameMap[ind] == '*') {
      std::vector<int> foundedNums{};
      std::unordered_set<int> checkedIndexes;
      // checking all the symbols nearby
      for (std::pair<std::string, int> dir : DIRS) {
        int observedInd = ind + dir.second;

        // if this symbol is digit
        if (isdigit(gameMap[observedInd]) &&
            checkedIndexes.find(observedInd) == checkedIndexes.end()) {

          // go right to the last digit symbol, after which another symbol is
          int step = 0;
          while (isdigit(gameMap[observedInd + step + 1])) {
            if (observedInd + step + 1 > gameMap.size()) {
              break;
            }
            step++;
          }

          int num = 0;
          int degree = 1;
          // and then back to not digit symbol
          while (isdigit(gameMap[observedInd + step])) {
            checkedIndexes.insert(observedInd + step);
            num += (gameMap[observedInd + step] - '0') * degree;
            degree *= 10;
            step--;
            if (observedInd + step < 0) {
              break;
            }
          }
          foundedNums.push_back(num);
        }
      }
      if (foundedNums.size() == 2) {
        ans += foundedNums.back() * foundedNums.front();
      }
    }
  }
  return ans;
}

int main() {
  std::fstream file("input.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file." << '\n';
    return 1;
  }

  std::string map;
  std::string line;

  std::getline(file, line);
  int mapLength = line.size();
  map += line;

  while (std::getline(file, line)) {
    map += line;
  }

  file.close();

  for (int y = 0; y < int(map.size() / mapLength); y++) {
    for (int x = 0; x < mapLength; x++) {
      std::cout << map[x + y * mapLength];
    }
    std::cout << '\n';
  }

  std::cout << '\n' << CheckMap(map, mapLength) << '\n';
}
