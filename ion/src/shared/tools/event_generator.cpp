#include <iostream>
#include <fstream>
#include <random>
#include <cstdio>
#include <ion/events.h>

void writeEventsToFile(std::ofstream & file, int numberOfEvents, std::mt19937 & rng) {
  std::uniform_int_distribution<int> distribution(0,4*Ion::Events::Event::PageSize);

  int i = numberOfEvents;
  while (i > 0) {
    int prEventId = distribution(rng);
    Ion::Events::Event e(prEventId);
    if (e.isDefined() && e != Ion::Events::Termination) {
      i--;
      file << (unsigned char)(prEventId);
    }
  }
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: event_generator <count> <length_avg>" << std::endl;
    return -1;
  }

  std::random_device r;
  std::mt19937 mt(r());

  int count = std::stoi(argv[1]);
  int lengthAvg = std::stoi(argv[2]);
  int lengthStdDev = lengthAvg;

  std::normal_distribution<> lengthDistribution(lengthAvg, lengthStdDev);

  for (int i=0; i<count; i++) {
    char buffer[32];
    sprintf(buffer, "scenario_%04d.bin", i);
    std::string fileName(buffer);
    std::ofstream file;
    file.open(fileName);
    writeEventsToFile(file, lengthDistribution(mt), mt);
    file.close();
  }

  return 0;
}
