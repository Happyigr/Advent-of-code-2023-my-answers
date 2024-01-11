#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// Function to extract the first and last numbers from a line
int GetNum(const std::string &inputline) {
  const std::vector<std::pair<std::string, int>> WORDNUMS({{"one", 1},
                                                           {"two", 2},
                                                           {"three", 3},
                                                           {"four", 4},
                                                           {"five", 5},
                                                           {"six", 6},
                                                           {"seven", 7},
                                                           {"eight", 8},
                                                           {"nine", 9}});

  std::cout << inputline << ' ';

  std::string buffer = "";
  int firstDigit = 0;
  for (int i = 0; i < inputline.size(); i++) {

    if (isdigit(inputline[i])) {
      firstDigit = inputline[i] - '0';
      break;
    }

    buffer += inputline[i];
    for (const auto &numWord : WORDNUMS) {
      std::regex pattern(numWord.first);
      if (std::regex_search(buffer, pattern)) {
        firstDigit = numWord.second;
        break;
      }
    }
    if (firstDigit != 0) {
      break;
    }
  }

  buffer = "";
  int secondDigit = 0;
  for (int i = inputline.size() - 1; i >= 0; i--) {

    if (isdigit(inputline[i])) {
      secondDigit = int(inputline[i]) - '0';
      break;
    }

    buffer = inputline[i] + buffer;
    for (const auto &numWord : WORDNUMS) {
      std::regex pattern(numWord.first);
      if (std::regex_search(buffer, pattern)) {
        secondDigit = numWord.second;
        break;
      }
    }
    if (secondDigit != 0) {
      break;
    }
  }

  if (secondDigit == 0) {
    secondDigit = firstDigit;
  }
  std::cout << firstDigit * 10 + secondDigit << std::endl;

  return firstDigit * 10 + secondDigit;
}

int main() {
  // Open the file
  std::ifstream file("input.txt");

  // Check if the file is open successfully
  if (!file.is_open()) {
    std::cerr << "Unable to open the file." << std::endl;
    return 1;
  }

  std::string line;
  int ans;

  while (std::getline(file, line)) {
    ans += GetNum(line);
    /* getchar(); */
  }

  std::cout << ans << std::endl;
  file.close();

  return 0;
}
