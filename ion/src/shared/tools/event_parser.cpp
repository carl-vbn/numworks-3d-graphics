/* This tools read human-readable events on stdin and outputs machine-readable
 * ones on stdout. */

#include <iostream>
#include <cassert>
#include <ion/events.h>

using namespace Ion::Events;

int eventIndexFromName(std::string name) {
  for (int i=0; i<255; i++) {
    Event e(i);
    if (e.name() != nullptr && std::string(e.name()) == name) {
      return i;
    }
  }
  return -1;
}

int main(int argc, char * argv[]) {
  std::string eventName;
  while (std::getline(std::cin, eventName)) {
    int eventIndex = eventIndexFromName(eventName);
    if (eventIndex < 0) {
      std::cerr << "Error: Could not recognize event \"" << eventName << "\"" << std::endl;
      return -1;
    }
    std::cout << (unsigned char)eventIndex;
  }
}
