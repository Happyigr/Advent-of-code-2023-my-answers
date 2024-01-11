#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void SeedsRefract(const std::vector<std::vector<int64_t>> &dataMap,
                  std::vector<int64_t> &seeds) {
  bool placeFound;
  std::vector<int64_t> ans;

  for (const auto &seed : seeds) {
    placeFound = false;
    for (const auto &data : dataMap) {
      // if seed is in range
      if (seed - data[1] >= 0 && seed < data[1] + data[2]) {
        ans.push_back(seed + (data[0] - data[1]));
        placeFound = true;
      }
    }
    if (!placeFound) {
      ans.push_back(seed);
    }
  }
  for (int i = 0; i < ans.size(); i++) {
    std::cout << ans[i] << ' ';
    seeds[i] = ans[i];
  }
  std::cout << '\n';
}

int main() {

  std::fstream file("example.txt");

  if (!file.is_open()) {
    std::cerr << "unable to open the file.";
    return 1;
  }
  std::string line;
  int64_t num;

  std::vector<int64_t> seeds;
  std::getline(file, line);
  std::stringstream ss(line.substr(line.find(':') + 2));

  // getting the first line of seeds
  while (ss >> num) {
    seeds.push_back(num);
  }

  bool isMap = false;
  std::vector<std::vector<int64_t>> dataMap;

  // deleteng the empty line after first row in input data
  std::getline(file, line);
  while (std::getline(file, line) || file.eof()) {
    // if it is the map data, then get it
    if (isMap && !line.empty()) {
      int64_t destRange, srcRange, range;
      ss = std::stringstream(line);
      if (ss >> destRange >> srcRange >> range) {
        dataMap.push_back(std::vector<int64_t>{destRange, srcRange, range});
      } else {
        std::cerr << "fail in reading line - " << line << '\n';
      }
    }

    // if an empty line catched, refract the data with the previous map
    if (line.empty() || file.eof()) {
      // seeds will be modified
      SeedsRefract(dataMap, seeds);
      isMap = false;
      dataMap.clear();
      if (file.eof()) {
        break;
      }
    }

    // if the ':' catched, the start of the map in the data catched ({name of
    // map}:)
    if (line.find(':') != std::string::npos) {
      isMap = true;
    }
  }

  int64_t ans = 0;
  for (int64_t seed : seeds) {
    if (seed < ans || ans == 0) {
      ans = seed;
    }
  }

  std::cout << ans;
  std::cout << '\n';
  return 0;
}
