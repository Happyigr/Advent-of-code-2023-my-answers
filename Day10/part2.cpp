#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Dot {
  int x;
  int y;

  Dot(){};
  Dot(int _x, int _y) : x(_x), y(_y) {}

  bool operator==(const Dot &dot) const { return x == dot.x && y == dot.y; }
  Dot operator+(const Dot &dot) const { return Dot(dot.x + x, dot.y + y); }
};

struct DotHash {
  std::size_t operator()(const Dot &dot) const {
    // You can customize the hash function based on your requirements
    return std::hash<int>()(dot.x) ^ (std::hash<int>()(dot.y) << 1);
  }
};

const std::unordered_map<char, std::unordered_map<char, char>> dirPipeToDir{
    {'R', {{'-', 'R'}, {'J', 'U'}, {'7', 'D'}}},
    {'L', {{'-', 'L'}, {'L', 'U'}, {'F', 'D'}}},
    {'U', {{'|', 'U'}, {'7', 'L'}, {'F', 'R'}}},
    {'D', {{'|', 'D'}, {'L', 'R'}, {'J', 'L'}}}};

const std::unordered_map<char, Dot> dirToSteps{
    {'R', {1, 0}}, {'L', {-1, 0}}, {'D', {0, 1}}, {'U', {0, -1}}};

struct Cat {
  char dir;
  Dot pos;

  Cat() : pos(0, 0), dir('0'){};
  Cat(Dot _pos) : pos(_pos.x, _pos.y), dir('0'){};

  bool Move(std::vector<std::string> &map) {
    pos = pos + dirToSteps.at(dir);
    if (map[pos.y][pos.x] == 'S')
      return false;
    dir = dirPipeToDir.at(dir).at(map[pos.y][pos.x]);
    return true;
  };
};

int Shoelace(const std::vector<Dot> &polygon) {
  // Shoelace theorem: A = 1/2 * sum((x[i+1] + x[i])(y[i+1]-y[i]))
  int area = 0;

  for (int i = 0; i < polygon.size() - 1; ++i) {
    area += (polygon[i].x * polygon[i + 1].y - polygon[i + 1].x * polygon[i].y);
  }

  // Add the contribution of the last edge
  area += (polygon.back().x * polygon[0].y - polygon[0].x * polygon.back().y);

  // Take the absolute value and divide by 2
  area = std::abs(area) / 2.0;

  return area;
}

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

  Dot startPos;
  for (int i = 0; i < map.size(); i++)
    if (map[i].find('S') != std::string::npos)
      startPos = Dot(map[i].find('S'), i);

  Cat cat = Cat(startPos);

  // getting start directions of the cats (there are 2 dirs)
  for (const auto &dirStep : dirToSteps) {
    auto dir = dirStep.first;
    auto steps = dirStep.second;
    if (dirPipeToDir.at(dir).find(
            map[cat.pos.y + steps.y][cat.pos.x + steps.x]) !=
        dirPipeToDir.at(dir).end())
      // 0 is the value by default
      cat.dir = dir;
  }

  std::cout << "cat1dir = " << cat.dir << '\n';

  std::vector<Dot> boundaries{startPos};
  cat.Move(map);
  int steps = 1;
  while (cat.Move(map)) {
    steps++;
    boundaries.push_back(cat.pos);
  }
  // insert the last pipe
  boundaries.push_back(cat.pos);

  // picks theorem: A = I + B/2 -1
  // I is the points in the polygon
  // I = A - B/2 + 1
  int Area = Shoelace(boundaries);
  int ans = Area - boundaries.size() / 2 + 1;

  std::cout << "Boundaries: " << boundaries.size() << "\nArea: " << Area
            << "\nAns: " << ans << '\n';

  std::cout << "\n\nSteps: " << steps << '\n';

  return 0;
}
