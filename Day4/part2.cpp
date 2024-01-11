#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

void CheckCard(const std::string &card, int *cards) {
  std::stringstream ss(card.substr(4));
  int num;

  int cardNum;
  while (ss >> num) {
    cardNum = num;
    if (ss.peek() == ' ') {
      ss.ignore();
    }
    if (ss.peek() == ':') {
      ss.ignore();
      break;
    }
  }

  std::unordered_set<int> winNums;
  std::vector<int> myNums;

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
      ans++;
    }
  }

  for (int i = 1; i <= ans; i++) {
    cards[i + cardNum] += cards[cardNum];
  }
}

int main() {

  std::fstream file("input.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file";
  }

  int ans = 0;
  std::string line;
  int cardsAmount = 202;
  int *cards = new int[cardsAmount];

  for (int i = 0; i < cardsAmount; i++) {
    cards[i] = 1;
  }

  while (std::getline(file, line)) {
    CheckCard(line, cards);
  }
  for (int i = 1; i < cardsAmount; i++) {
    ans += cards[i];
  }

  delete[] cards;
  std::cout << ans << '\n';

  return 0;
}
