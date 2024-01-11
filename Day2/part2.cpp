#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

int CheckGame(std::string game) {
  // game example - Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
  std::map<std::string, int> balls{{"blue", 0}, {"red", 0}, {"green", 0}};
  size_t colonPos = game.find(':');
  int gameNum = std::stoi(game.substr(5, colonPos - 1));
  game = game.substr(colonPos + 2);

  int prev_i = 0;
  for (int i = 0; i < game.size(); i++) {
    if ((game[i] == ';') || (game[i] == ',') || (i == game.size() - 1)) {
      // getting the pair of color and amount of balls
      std::string pair;
      if (i == game.size() - 1) {
        pair = game.substr(prev_i);
      } else {
        pair = game.substr(prev_i, i - prev_i);
      }
      std::istringstream ballPair(pair);
      int quantity;
      std::string color;

      if (ballPair >> quantity >> color) {
        balls[color] = std::max(balls[color], quantity);
      } else {
        std::cerr << "error in reading quantoty and color of balls in "
                  << game.substr(0, i) << std::endl;
      }
      prev_i = i + 2;
    }
  }
  return balls["blue"] * balls["green"] * balls["red"];
}

int main() {
  std::ifstream file("input");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file" << std::endl;
    return 1;
  }

  int ans = 0;

  int gameSum;
  std::string line;
  while (std::getline(file, line)) {
    ans += CheckGame(line);
    /* gameSum = CheckGame(line, balls); */
    /* ans += gameSum; */
    /* std::cout << gameSum << " - " << line << std::endl; */
    /* getchar(); */
  }

  std::cout << ans << '\n';
  file.close();

  return 0;
}
