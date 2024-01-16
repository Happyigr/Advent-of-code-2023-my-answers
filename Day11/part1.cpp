#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct vec2 {
  int x;
  int y;

  vec2() {}
  vec2(int _x, int _y) : x(_x), y(_y) {}

  vec2 operator+(const vec2 &other) const {
    return vec2(x + other.x, y + other.y);
  }
  vec2 operator-(const vec2 &other) const {
    return vec2(x - other.x, y - other.y);
  }
  int length() const { return x + y; }
};

inline int getDistanceBetween(vec2 v1, vec2 v2) {
  return (abs(v1.x - v2.x) + abs(v1.y - v2.y));
}

void RefractMap(std::vector<std::string> &map) {
  std::vector<int> rowsWithoutGalaxies;
  for (auto i = 0; i < map.size(); i++)
    if (map[i].find('#') == std::string::npos)
      rowsWithoutGalaxies.push_back(i);

  for (int i = rowsWithoutGalaxies.size() - 1; i >= 0; i--)
    map.insert(map.begin() + rowsWithoutGalaxies[i] + 1, 1,
               map[rowsWithoutGalaxies[i]]);

  std::vector<int> columnsWithoutGalaxies;
  for (auto x = 0; x < map[0].size(); x++) {
    bool withGalaxy = false;
    for (auto row : map)
      if (row[x] == '#')
        withGalaxy = true;
    if (!withGalaxy)
      columnsWithoutGalaxies.push_back(x);
  }

  for (int i = columnsWithoutGalaxies.size() - 1; i >= 0; i--)
    for (auto &line : map)
      line.insert(line.begin() + columnsWithoutGalaxies[i] + 1, 1, '.');
}

auto GetMap(int argc) {
  std::fstream file;
  if (argc > 1)
    file = std::fstream("input.txt");
  else
    file = std::fstream("example.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file";
    return std::vector<std::string>{};
  }
  std::string line;
  std::vector<std::string> map;

  while (std::getline(file, line))
    map.push_back(line);

  return map;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> map = GetMap(argc);
  if (map.size() == 0)
    return 1;

  RefractMap(map);
  for (auto line : map)
    std::cout << line << '\n';

  std::vector<vec2> galaxies;

  for (int y = 0; y < map.size(); y++)
    for (int x = 0; x < map[0].size(); x++)
      if (map[y][x] == '#')
        galaxies.push_back(vec2(x, y));

  int ans = 0;
  int steps = 0;
  /* std::cout << "\nDistances: " << '\n'; */
  for (auto i = 0; i < galaxies.size() - 1; i++)
    for (auto j = i + 1; j < galaxies.size(); j++) {
      /* std::cout << "Galaxies: " << i << ' ' << j << '=' */
      /*           << getDistanceBetween(galaxies[i], galaxies[j]) << '\n'; */
      ans += getDistanceBetween(galaxies[i], galaxies[j]);
    }

  /* for (auto gal : galaxies) */
  /* std::cout << gal.x << ' ' << gal.y << '\n'; */
  std::cout << ans << '\n';

  return 0;
}
