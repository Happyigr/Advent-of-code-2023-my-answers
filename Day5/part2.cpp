#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::vector<uint64_t>> vvui64;
typedef std::vector<uint64_t> vui64;

uint64_t GetLocation(const std::vector<vvui64> &maps, const uint64_t seed) {
  uint64_t ans = seed;
  bool placeFounded = false;
  for (const auto &map : maps) {
    for (const auto &line : map) {
      // if seed is in range than change it
      if (line[1] <= ans && ans < line[1] + line[2]) {
        ans += (line[0] - line[1]);
        break;
      }
    }
    // else leave so how it is
    // ans = ans
  }
  return ans;
}

int main() {
  std::fstream file("input.txt");
  std::stringstream ss;

  if (!file.is_open()) {
    std::cerr << "unable to open the file.";
    return 1;
  }
  std::string line;

  // getting the first line of seeds
  std::getline(file, line);
  ss = std::stringstream(line.substr(line.find(':') + 2));

  vui64 seeds;
  uint64_t maxSeed = 0;
  uint64_t minSeed = 10000000000000;
  uint64_t rangeStart, range;

  while (ss >> rangeStart >> range) {
    seeds.push_back(rangeStart);
    seeds.push_back(range);
    if (maxSeed < range + rangeStart - 1)
      maxSeed = range + rangeStart - 1;
    if (minSeed > rangeStart)
      minSeed = rangeStart;
  }
  std::cout << minSeed << '-' << maxSeed << '\n';

  // deleting empty line
  std::getline(file, line);

  vvui64 seed_soil;
  vvui64 soil_fertiz;
  vvui64 fertiz_water;
  vvui64 water_light;
  vvui64 light_temp;
  vvui64 temp_hum;
  vvui64 hum_loc;
  std::vector<vvui64> maps{seed_soil,  soil_fertiz, fertiz_water, water_light,
                           light_temp, temp_hum,    hum_loc};

  int mapNum = 0;
  while (std::getline(file, line) || file.eof()) {
    if (!line.empty() && line.find(':') == std::string::npos) {
      uint64_t destRange, srcRange, range;
      ss = std::stringstream(line);
      if (ss >> destRange >> srcRange >> range)
        maps[mapNum].push_back(vui64{destRange, srcRange, range});
      else
        std::cerr << "fail in reading line - " << line << '\n';
    }
    if (line.empty())
      mapNum++;
    if (file.eof()) {
      break;
    }
  }
  file.close();

  uint64_t step = 10000;
  uint64_t minLoc = maxSeed;
  uint64_t minLocSeed = 0;
  while (true) {
    // getting throgh all the seeds
    for (uint64_t seed = minSeed; seed <= maxSeed; seed += step) {
      // if it in the our input seeds continue
      for (int i = 0; i + 1 < seeds.size(); i += 2) {
        if (seeds[i] <= seed && seed < seeds[i] + seeds[i + 1]) {
          uint64_t seedLoc = GetLocation(maps, seed);

          /* std::cout << "Seed " << seed << " is in seeds LOCATION = " <<
           * seedLoc */
          /*           << '\n'; */
          if (seedLoc < minLoc) {
            minLoc = seedLoc;
            minLocSeed = seed;
          }

          break;
        }
      }
    }
    // if the difference between maxSeed and ans is bigger
    if (maxSeed - minLocSeed > minLocSeed - minSeed)
      maxSeed -= (maxSeed - minLocSeed) / 2;
    else
      minSeed += (minLocSeed - minSeed) / 2;

    std::cout << "STEP = " << step << " MIN = " << minSeed
              << " MIN-LOC-SEED = " << minLocSeed << " MAX = " << maxSeed
              << " MIN-LOC = " << minLoc << '\n';

    if (step == 1)
      break;
    else
      step /= 2;
  }
  std::cout << minLoc;

  std::cout << '\n';

  return 0;
}
