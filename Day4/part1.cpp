#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

int CheckCard(std::string &card) {

  // stream without game name
  std::stringstream ss(card.substr(card.find(':') + 2));
  std::unordered_set<int> winNums;
  std::vector<int> myNums;
  int num;

  while (ss >> num) {

    winNums.insert(num);

    if (ss.peek() == ' ') {
      ss.ignore();
    }

    if (ss.peek() == '|') {
      ss.ignore();
      break;
    }
  }

  while (ss >> num) {
    myNums.push_back(num);

    if (ss.peek() == ' ') {
      ss.ignore();
    }
  }

  int ans = 0;
  for (const int num : myNums) {
    if (winNums.find(num) != winNums.end()) {
      if (ans == 0) {
        ans++;
      } else {
        ans *= 2;
      }
    }
  }

  return ans;
}

int main() {

  std::fstream file("input.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file";
  }

  int ans = 0;
  std::string line;
  while (std::getline(file, line)) {
    ans += CheckCard(line);
  }
  std::cout << ans << '\n';

  return 0;
}
