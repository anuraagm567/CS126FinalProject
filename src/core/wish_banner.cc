#include "core/wish_banner.h"

namespace wishsimulator {

WishBanner::WishBanner(const json& banner_data)
    : kName(banner_data["name"]),
      kImageUrl(banner_data["imageurl"]),
      kFiveStarPity(banner_data["FiveStarPity"]),
      kFiveStarRateUp(WishItem(banner_data["FiveStarRateUp"])),
      random_generator_(std::random_device()()) {
  for (const json& item: banner_data["FourStarRateUps"]) {
    four_star_rate_ups_.push_back(WishItem(item));
  }

  for (const json& item: banner_data["StandardItems"]) {
    WishItem wish_item = WishItem(item);

    switch (wish_item.GetRarity()) {
      case kThreeStarRarity: {
        standard_3_stars_.push_back(wish_item);
        break;
      }
      case kFourStarRarity: {
        standard_4_stars_.push_back(wish_item);
        break;
      }
      case kFiveStarRarity: {
        standard_5_stars_.push_back(wish_item);
        break;
      }
      default: {
        throw invalid_argument("Invalid rarity value");
      }
    }
  }
}

ostream& operator<<(ostream& output_stream, const WishBanner& banner) {
  output_stream << banner.current_4_star_pity_ << endl;
  output_stream << banner.current_5_star_pity_ << endl;
  output_stream << banner.received_standard_4_star_ << endl;
  output_stream << banner.received_standard_5_star_ << endl;

  for (const string& item: banner.inventory_) {
    output_stream << item << endl;
  }

  return output_stream;
}

istream& operator>>(istream& input_stream, WishBanner& banner) {
  string four_star_pity;
  string five_star_pity;
  string received_standard_four_star;
  string received_standard_five_star;
  string inventory_item;

  banner.current_4_star_pity_ = 0;
  banner.current_5_star_pity_ = 0;
  banner.inventory_.clear();

  if (getline(input_stream, four_star_pity) &&
      getline(input_stream, five_star_pity) &&
      getline(input_stream, received_standard_four_star) &&
      getline(input_stream, received_standard_five_star)) {
    stringstream sstream;

    sstream << four_star_pity;
    sstream >> banner.current_4_star_pity_;
    sstream.clear();

    sstream << five_star_pity;
    sstream >> banner.current_5_star_pity_;
    sstream.clear();

    sstream << received_standard_four_star;
    sstream >> banner.received_standard_4_star_;
    sstream.clear();

    sstream << received_standard_five_star;
    sstream >> banner.received_standard_5_star_;
  }

  while (getline(input_stream, inventory_item)) {
    banner.inventory_.push_back(inventory_item);
  }

  return input_stream;
}

vector<WishItem> WishBanner::PerformMultipleWishes(size_t wish_count) {
  vector<WishItem> items_received;

  for (size_t i = 0; i < wish_count; i++) {
    const WishItem& item = PerformSingleWish();
    stringstream sstream;
    sstream << item;

    items_received.push_back(item);
    inventory_.push_back(sstream.str());
  }

  return items_received;
}

const WishItem& WishBanner::PerformSingleWish() {
  current_4_star_pity_++;
  current_5_star_pity_++;

  uniform_int_distribution<int> distribution(0, 100);
  float rand_float = 0.01f * distribution(random_generator_);

  if (rand_float < kBaseFiveStarRate || current_5_star_pity_ == kFiveStarPity) {
    current_4_star_pity_ = 0;
    current_5_star_pity_ = 0;

    if (received_standard_5_star_) {
      received_standard_5_star_ = false;

      return kFiveStarRateUp;
    }

    rand_float = 0.01f * distribution(random_generator_);

    // There is a 50/50 chance of getting the limited time item vs
    // a standard item on a wish
    if (rand_float < 1.0f/2) {
      received_standard_5_star_ = true;

      return RetrieveRandomItem(standard_5_stars_);
    }

    return kFiveStarRateUp;
  } else if (rand_float < kBaseFiveStarRate + kBaseFourStarRate ||
             current_4_star_pity_ == kFourStarPity) {
    current_4_star_pity_ = 0;

    if (received_standard_4_star_) {
      received_standard_4_star_ = false;

      return RetrieveRandomItem(four_star_rate_ups_);
    }

    rand_float = 0.01f * distribution(random_generator_);

    if (rand_float < 1.0f/2) {
      received_standard_4_star_ = true;

      return RetrieveRandomItem(standard_4_stars_);
    }

    return RetrieveRandomItem(four_star_rate_ups_);
  } else {
    return RetrieveRandomItem(standard_3_stars_);
  }
}

const string& WishBanner::GetItemAt(size_t index) const {
  return inventory_.at(index);
}

const WishItem& WishBanner::RetrieveRandomItem(const vector<WishItem>& items) {
  uniform_int_distribution<int> item_distribution(0, items.size() - 1);

  return items.at(item_distribution(random_generator_));
}

const string& WishBanner::GetName() const {
  return kName;
}

const string& WishBanner::GetImageUrl() const {
  return kImageUrl;
}

const vector<string>& WishBanner::GetInventory() const {
  return inventory_;
}

} // namespace wishsimulator