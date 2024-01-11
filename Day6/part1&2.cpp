#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

uint64_t WinCombinations(uint64_t time, uint64_t goal) {
  uint64_t firstCombo = 0;
  uint64_t secondCombo = 0;
  // at 0 and time we will always lose
  for (uint64_t usedTime = 1; usedTime < time; usedTime++) {
    // usedTime is equal to velocity of boat
    if (usedTime * (time - usedTime) > goal) {
      firstCombo = usedTime;
      break;
    }
  }
  for (uint64_t usedTime = time - 1; usedTime >= firstCombo; usedTime--) {
    if (usedTime * (time - usedTime) > goal) {
      secondCombo = usedTime;
      break;
    }
  }

  return secondCombo - firstCombo + 1;
}

std::vector<uint64_t> GetNums(const std::string &line) {
  std::istringstream ss(line);
  uint64_t num;
  std::vector<uint64_t> ans;
  while (ss >> num) {
    ans.push_back(num);
  }
  return ans;
}

int main() {
  std::fstream file("input2.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file" << '\n';
    return 1;
  }

  std::string line;

  std::getline(file, line);
  std::vector<uint64_t> times = GetNums(line.substr(line.find(':') + 2));
  std::getline(file, line);
  std::vector<uint64_t> goals = GetNums(line.substr(line.find(':') + 2));

  uint64_t ans = 1;
  for (int i = 0; i < times.size(); i++) {
    std::cout << WinCombinations(times[i], goals[i]) << '\n';
    ans *= WinCombinations(times[i], goals[i]);
  }
  std::cout << ans << '\n';
}
