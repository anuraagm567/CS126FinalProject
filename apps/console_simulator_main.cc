#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/wish_item.h>
#include <core/wish_banner.h>

using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::stringstream;
using nlohmann::json;
using wishsimulator::WishItem;
using wishsimulator::WishBanner;

int main() {
  json banner_info;

  std::ifstream banner_input("data/albedobannerinfo.json");

  banner_input >> banner_info;

  WishBanner banner(banner_info);

  bool has_ended = false;

  while (!has_ended) {
    cout << "How many times would you like to wish on " + banner.GetName() + "."
         << endl;
    cout << "If you want to stop wishing, please enter \"stop\"." << endl;

    string response;
    cin >> response;
    
    if (response == "stop") {
      has_ended = true;
    } else {
      stringstream response_stream(response);
      size_t wish_count;

      response_stream >> wish_count;
      size_t wish_number = 1;

      for (const WishItem& item: banner.PerformMultipleWishes(wish_count)) {
        cout << wish_number;
        cout << ". ";
        cout << item << endl;

        wish_number++;
      }
    }
  }
}

