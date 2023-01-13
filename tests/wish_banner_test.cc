#include <fstream>
#include <sstream>

#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>
#include <core/wish_banner.h>
#include <core/wish_item.h>

using nlohmann::json;
using std::stringstream;
using std::ifstream;
using wishsimulator::WishBanner;
using wishsimulator::WishItem;

TEST_CASE("Banner Follows Pity Rules") {
  json banner_info;

  ifstream banner_input("data/albedobannerinfo.json");

  banner_input >> banner_info;

  SECTION("Four Star or Five Star Received Within 10 Wishes") {
    // Repeats 100 times to account for randomness in wishing system
    for (size_t i = 0; i < 100; i++) {
      WishBanner banner(banner_info);
      size_t above_four_star_count = 0;

      for (const WishItem& item: banner.PerformMultipleWishes(10)) {
        if (item.GetRarity() >= 4) {
          above_four_star_count++;
        }
      }

      REQUIRE(above_four_star_count > 0);
    }
  }

  SECTION("Five Star Received Within 90 Wishes") {
    for (size_t i = 0; i < 100; i++) {
      WishBanner banner(banner_info);
      size_t five_star_count = 0;

      for (const WishItem& item: banner.PerformMultipleWishes(90)) {
        if (item.GetRarity() == 5) {
          five_star_count++;
        }
      }

      REQUIRE(five_star_count > 0);
    }
  }

  SECTION("Featured Five Star Received Within 180 Wishes") {
    for (size_t i = 0; i < 100; i++) {
      WishBanner banner(banner_info);
      size_t featured_five_star_count = 0;

      for (const WishItem& item: banner.PerformMultipleWishes(180)) {
        if (item.GetRarity() == 5) {
          stringstream ss;
          ss << item;

          if ("Albedo: 5 Star Character (Geo)" == ss.str()) {
            featured_five_star_count++;
          }
        }
      }

      REQUIRE(featured_five_star_count > 0);
    }
  }
}

TEST_CASE("Banner Saving with << Operator") {
  json banner_info;

  ifstream banner_input("data/albedobannerinfo.json");

  banner_input >> banner_info;

  SECTION("Zero Wishes") {
    WishBanner banner(banner_info);
    stringstream ss;
    ss << banner;

    REQUIRE("0\n0\n0\n0\n" == ss.str());
  }

  SECTION("Nonzero Wishes") {
    WishBanner banner(banner_info);
    stringstream ss_actual;
    stringstream ss_expected;
    vector<WishItem> items = banner.PerformMultipleWishes(90);
    ss_actual << banner;

    size_t four_star_pity = 0;
    size_t five_star_pity = 0;
    bool received_standard_four_star;
    bool received_standard_five_star;
    string item_list;

    for (const WishItem& item: items) {
      stringstream item_stream;
      item_stream << item;

      if (item.GetRarity() == 3) {
        four_star_pity++;
        five_star_pity++;
      } else if (item.GetRarity() == 4) {
        received_standard_four_star = item_stream.str() !=
                                      "Bennett: 4 Star Character (Pyro)" &&
                                      item_stream.str() !=
                                      "Sucrose: 4 Star Character (Anemo)" &&
                                      item_stream.str() !=
                                      "Fischl: 4 Star Character (Electro)";

        four_star_pity = 0;
        five_star_pity++;
      } else {
        received_standard_five_star = item_stream.str() !=
                                      "Albedo: 5 Star Character (Geo)";
        four_star_pity = 0;
        five_star_pity = 0;
      }

      item_list += item_stream.str() + "\n";
    }

    ss_expected << four_star_pity << "\n";
    ss_expected << five_star_pity << "\n";
    ss_expected << received_standard_four_star << "\n";
    ss_expected << received_standard_five_star << "\n";
    ss_expected << item_list;

    REQUIRE(ss_expected.str() == ss_actual.str());
  }
}

TEST_CASE("Banner Loading with >> Operator") {
  json banner_info;

  ifstream banner_input("data/albedobannerinfo.json");

  banner_input >> banner_info;

  SECTION("Empty Inventory After Loading Zero Wishes") {
    WishBanner banner(banner_info);
    ifstream zero_wishes("data/testloadingzerowishes.txt");
    zero_wishes >> banner;

    REQUIRE(banner.GetInventory().empty());
  }

  SECTION("Updated Inventory After Loading Nonzero Wishes") {
    WishBanner banner(banner_info);
    ifstream wishes("data/testlimitedfourstarafterloading.txt");
    wishes >> banner;

    vector<string> expected_inventory = {"Amber: 4 Star Character (Pyro)"};

    REQUIRE(expected_inventory == banner.GetInventory());
  }

  SECTION("Limited 4 Star After Loading Wishes") {
    // Going through 100 iterations to account for random probabilities
    // Ensures that next 4 star after receiving a standard 4 star is a limited
    // 4 star
    for (size_t i = 0; i < 100; i++) {
      WishBanner banner(banner_info);
      ifstream wishes("data/testlimitedfourstarafterloading.txt");
      wishes >> banner;

      bool received_four_star = false;
      while (!received_four_star) {
        WishItem item = banner.PerformMultipleWishes(1).at(0);

        if (item.GetRarity() == 4) {
          received_four_star = true;
          stringstream sstream;
          sstream << item;

          REQUIRE(("Bennett: 4 Star Character (Pyro)" == sstream.str() ||
                   "Sucrose: 4 Star Character (Anemo)" == sstream.str() ||
                   "Fischl: 4 Star Character (Electro)" == sstream.str()));
        }
      }
    }
  }

  SECTION("Limited 5 Star After Loading Wishes") {
    // Ensures that next 5 star after receiving a standard 5 star is a limited
    // 5 star
    for (size_t i = 0; i < 100; i++) {
      WishBanner banner(banner_info);
      ifstream wishes("data/testlimitedfivestarafterloading.txt");
      wishes >> banner;

      bool received_five_star = false;
      while (!received_five_star) {
        WishItem item = banner.PerformMultipleWishes(1).at(0);

        if (item.GetRarity() == 5) {
          received_five_star = true;
          stringstream sstream;
          sstream << item;

          REQUIRE("Albedo: 5 Star Character (Geo)" == sstream.str());
        }
      }
    }
  }
}