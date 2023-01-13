#pragma once

#include <vector>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <random>
#include <cstddef>

#include <nlohmann/json.hpp>

#include "core/wish_item.h"

using std::vector;
using std::invalid_argument;
using std::stringstream;
using std::uniform_int_distribution;
using std::endl;
using std::ostream;
using std::istream;
using std::getline;
using nlohmann::json;

namespace wishsimulator {

/**
 * Object denoting a banner that players are able to wish on to receive items
 */
class WishBanner {
  public:
    /**
     * Constructs a wish banner from stored json data
     * @param banner_data Json object representing a banner for wishing
     */
    explicit WishBanner(const json& banner_data);

    /**
     * Outputs wishing progress to file to save game state
     * @param output_stream Output stream used to save wishing progress
     * @param banner WishBanner instance storing game state
     * @return output stream that was passed in
     */
    friend ostream& operator<<(ostream& output_stream,
                               const WishBanner& banner);

    /**
     * Updates banner state using saved wish data
     * @param input_stream Input stream used to load in saved wish data
     * @param banner WishBanner instance to be updated with saved data
     * @return input stream that was passed in
     */
    friend istream& operator>>(istream& input_stream, WishBanner& banner);

    /**
     * Performs a given number of wishes
     * @param wish_count Number of wishes to be performed
     * @return vector containing the items received
     */
    vector<WishItem> PerformMultipleWishes(size_t wish_count);

    /**
     * Retrieves inventory item at given index
     * @param index Index corresponding to inventory item
     * @return string representation of inventory item
     */
    const string& GetItemAt(size_t index) const;

    const string& GetName() const;

    const string& GetImageUrl() const;

    const vector<string>& GetInventory() const;

  private:
    static constexpr size_t kThreeStarRarity = 3;
    static constexpr size_t kFourStarRarity = 4;
    static constexpr size_t kFiveStarRarity = 5;

    static constexpr float kBaseFiveStarRate = 0.006f;
    static constexpr float kBaseFourStarRate = 0.051f;

    bool received_standard_4_star_ = false;
    bool received_standard_5_star_ = false;

    const string kName;
    const string kImageUrl;

    static constexpr size_t kFourStarPity = 10;
    const size_t kFiveStarPity;
    size_t current_5_star_pity_ = 0;
    size_t current_4_star_pity_ = 0;
    vector<string> inventory_;

    const WishItem kFiveStarRateUp;
    vector<WishItem> four_star_rate_ups_;

    vector<WishItem> standard_3_stars_;
    vector<WishItem> standard_4_stars_;
    vector<WishItem> standard_5_stars_;

    std::mt19937 random_generator_;

    /**
     * Simulates a single wish and updates banner state accordingly
     */
    const WishItem& PerformSingleWish();

    /**
     * Retrieves a random item from a given vector of items
     * @param items Vector of WishItem objects
     * @return random item from the given vector
     */
    const WishItem& RetrieveRandomItem(const vector<WishItem>& items);
};

} // namespace wishsimulator