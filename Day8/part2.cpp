#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct City {
  std::string left;
  std::string right;

  City() : left(""), right("") {}
  City(const std::string &left, const std::string &right)
      : left(left), right(right) {}
};

typedef std::unordered_map<std::string, City> unmap_str_City;

// returns all the cities before loop starts and the city name, after which the
// loop starts
auto GetLoopWithLoopStart(const std::string &startCity, const std::string &dirs,
                          const unmap_str_City &Cities) {

  int loops = 0;
  std::string currentCity = startCity;
  std::unordered_map<std::string, int> visitedCities{{startCity, 0}};
  for (loops = 1;; loops++) {
    for (char dir : dirs)
      dir == 'L' ? currentCity = Cities.at(currentCity).left
                 : currentCity = Cities.at(currentCity).right;

    if (visitedCities.find(currentCity) != visitedCities.end())
      break;
    else
      visitedCities[currentCity] = loops;
  }
  return std::make_pair(visitedCities, currentCity);
}

int GetFirstEndCity(const std::string &startCity, const std::string &dirs,
                    const unmap_str_City &Cities) {
  int loops;
  std::string currentCity = startCity;
  for (loops = 0; currentCity.back() != 'Z'; loops++)
    for (char dir : dirs)
      dir == 'L' ? currentCity = Cities.at(currentCity).left
                 : currentCity = Cities.at(currentCity).right;

  return loops;
}
// Функция для нахождения НОК неопределенного количества чисел
uint64_t findLCMofN(const std::vector<int> numbers, int n) {
  if (n < 2) {
    std::cerr << "For LCM finding at least 2 nums needed." << std::endl;
    return -1;
  }

  uint64_t result = numbers[0];

  for (int i = 1; i < n; ++i) {
    result = std::lcm(result, numbers[i]);
  }

  return result;
}

int main() {
  std::fstream file("input.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file" << '\n';
    return 1;
  }

  std::string dirs;
  std::getline(file, dirs);

  unmap_str_City Cities;
  std::vector<std::string> startCities;
  std::regex pattern(R"((\w+) = \((\w+), (\w+)\))");
  std::string line;

  while (std::getline(file, line)) {
    std::smatch match;
    if (std::regex_search(line, match, pattern)) {
      std::string cityName = match[1];
      Cities[cityName] = City(match[2], match[3]);
      if (cityName.back() == 'A')
        startCities.push_back(cityName);
    }
  }

  std::vector<int> endCitiesLoops;

  for (std::string name : startCities) {
    auto loop = GetLoopWithLoopStart(name, dirs, Cities);
    std::unordered_map<std::string, int> loopedCities = loop.first;
    std::string loopStart = loop.second;
    std::cout << name << " loop length = "
              << loopedCities.size() - loopedCities[loopStart]
              << " loop starts after loop " << loopedCities[loopStart] << '\n'
              << "ENDCITIES after loop:";

    for (std::pair<std::string, int> cityLoop : loopedCities) {
      if (cityLoop.first.back() == 'Z') {
        endCitiesLoops.push_back(cityLoop.second);
        std::cout << ' ' << cityLoop.first << '=' << cityLoop.second;
      }
    }
    std::cout << '\n';
  }

  uint64_t ans =
      findLCMofN(endCitiesLoops, endCitiesLoops.size()) * dirs.size();
  std::cout << ans << '\n';

  return 0;
}
