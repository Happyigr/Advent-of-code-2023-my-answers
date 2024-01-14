#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int64_t GetPrevValue(const std::vector<int64_t> &vals) {
  bool allNulls = true;
  std::vector<int64_t> deltaVals;

  for (auto i = vals.size() - 1; i > 0; i--) {
    if (vals[i] != 0)
      allNulls = false;
    deltaVals.insert(deltaVals.begin(), vals[i - 1] - vals[i]);
  }

  if (allNulls)
    return 0;
  else
    return GetPrevValue(deltaVals) + vals.front();
}

int main(int argc, char *argv[]) {
  std::fstream file;
  if (argc > 1)
    file = std::fstream("input.txt");
  else
    file = std::fstream("example.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open the file\n";
    return 1;
  }

  std::string line;
  std::istringstream iss;
  int64_t num;

  int64_t ans = 0;
  while (std::getline(file, line)) {
    iss = std::istringstream(line);

    std::vector<int64_t> sensorData;
    while (iss >> num)
      sensorData.push_back(num);
    ans += GetPrevValue(sensorData);
    std::cout << line << " prev val is - " << GetPrevValue(sensorData) << '\n';
  }
  std::cout << ans << '\n';

  return 0;
}
