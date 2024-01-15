#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

const std::unordered_map<char, std::unordered_map<char, char>> dirPipeToDir{
    {'R', {{'-', 'R'}, {'J', 'U'}, {'7', 'D'}}},
    {'L', {{'-', 'L'}, {'L', 'U'}, {'F', 'D'}}},
    {'U', {{'|', 'U'}, {'7', 'L'}, {'F', 'R'}}},
    {'D', {{'|', 'D'}, {'L', 'R'}, {'J', 'L'}}}};

const std::unordered_map<char, std::pair<int, int>> dirToSteps{
    {'R', {0, 1}}, {'L', {0, -1}}, {'D', {1, 0}}, {'U', {-1, 0}}};

const std::vector<char> dirs{'R', 'L', 'D', 'U'};

struct Cat {
  char dir;
  int x;
  int y;

  Cat(){};
  Cat(std::pair<int, int> _pos) : y(_pos.first), x(_pos.second), dir('0'){};
  Cat(int _y, int _x, char _dir) : x(_x), y(_y), dir(_dir){};

  void Move(const std::vector<std::string> &map) {
    x += dirToSteps.at(dir).second;
    y += dirToSteps.at(dir).first;
    dir = dirPipeToDir.at(dir).at(map[y][x]);
  };
};

auto GetMap(std::fstream &file) {
  std::vector<std::string> map;
  std::string line;
  while (std::getline(file, line))
    map.push_back(line);
  file.close();
  return map;
}

int main(int argc, char *argv[]) {
  std::fstream file;
  argc > 1 ? file = std::fstream("input.txt")
           : file = std::fstream("example.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file\n";
    return 1;
  }

  auto map = GetMap(file);

  std::pair<int, int> startPos;
  for (int i = 0; i < map.size(); i++)
    if (map[i].find('S') != std::string::npos)
      startPos = std::make_pair(i, map[i].find('S'));

  Cat cat1 = Cat(startPos);
  Cat cat2 = Cat(startPos);

  // getting start directions of the cats (there are 2 dirs)
  for (const auto &dir : dirs)
    if (dirPipeToDir.at(dir).find(map[cat1.y + dirToSteps.at(dir).first]
                                     [cat1.x + dirToSteps.at(dir).second]) !=
        dirPipeToDir.at(dir).end())
      cat1.dir == '0' ? cat1.dir = dir : cat2.dir = dir;

  std::cout << "cat1dir = " << cat1.dir << " cat2dir = " << cat2.dir << '\n';

  int steps;
  while (true) {
    steps++;
    cat1.Move(map);
    cat2.Move(map);
    if (cat1.x == cat2.x && cat1.y == cat2.y)
      break;
  }

  std::cout << steps << '\n';

  return 0;
}
