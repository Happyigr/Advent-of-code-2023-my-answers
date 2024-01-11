#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
struct CardComparator {
  std::map<char, int> rankValues{
      {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3},  {'6', 4},  {'7', 5}, {'8', 6},
      {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12}};

  bool operator()(const std::string &a, const std::string &b) const {
    // Extract the rank characters from the keys
    for (int i = 0; i < a.size(); i++) {

      char rankA = a[i];
      char rankB = b[i];

      // If the ranks are different, use their numerical values for comparison
      if (rankA != rankB) {
        int valueA = rankValues.at(rankA);
        int valueB = rankValues.at(rankB);
        return valueA > valueB;
      }
    }
    // If the ranks are the same, compare the suits
    return a.substr(1) > b.substr(1);
  }
};

typedef std::array<std::map<std::string, int, CardComparator>, 7> a7msi;
typedef std::map<std::string, int> msi;
typedef std::pair<std::string, int> psi;

int GetTier(std::string hand) {
  // from 6 (weakest) to 0 (strongest)
  std::unordered_map<char, int> cardsAmount;
  int tier = 6;

  for (char card : hand)
    cardsAmount[card]++;

  for (const auto &pair : cardsAmount) {
    // pair.second is amount of this card in the hand
    if (pair.second == 5)
      tier = 0;
    else if (pair.second == 4)
      tier = 1;
    // tier = 5 means the hand has one pair
    // tier = 3 means the hand has three of a kind
    else if (pair.second == 3 && tier == 5 || pair.second == 2 && tier == 3)
      tier = 2;
    else if (pair.second == 3)
      tier = 3;
    else if (pair.second == 2 && tier == 5)
      tier = 4;
    else if (pair.second == 2)
      tier = 5;
  }

  return tier;
}

psi GetHandBid(std::string line) {
  std::istringstream iss(line);
  int bid;
  std::string hand;

  if (iss >> hand >> bid) {
    return std::make_pair(hand, bid);
  } else {
    std::cerr << "Fail in read Line - " << line << '\n';
  }

  return std::make_pair("", 0);
}

int main() {
  std::fstream file("input.txt");

  if (!file.is_open()) {
    std::cerr << "unabel to open the file" << '\n';
    return 1;
  }

  a7msi HandsByTier;
  int MaxHands = 0;

  std::string line;
  while (std::getline(file, line)) {
    psi HandBid = GetHandBid(line);
    // a card in the sorted map of tier is equal to bid;
    HandsByTier[GetTier(HandBid.first)][HandBid.first] = HandBid.second;
    MaxHands++;
  }

  int place = 0;
  int ans = 0;
  for (const auto &sortedHands : HandsByTier) {
    for (const auto &HandBid : sortedHands) {
      std::cout << "HAND: " << HandBid.first << " has - " << place + 1
                << " - place and BID: " << HandBid.second << '\n';
      ans += HandBid.second * (MaxHands - place);
      place++;
    }
  }
  std::cout << ans << '\n';

  return 0;
}
