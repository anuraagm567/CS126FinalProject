#pragma once

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

using std::string;
using std::ostream;
using nlohmann::json;

namespace wishsimulator {

/**
 * Represents an item that can be obtained in wish simulator
 */
class WishItem {
  public:
    /**
     * Constructs a WishItem object from a passed in json object
     * representing an item
     * @param item
     */
    WishItem(const json& item);

    /**
     * Outputs the properties of an item retrieved by a wish
     * @param output_stream Output stream used to display item details
     * @param item WishItem object representing an item from a wish
     * @return output stream
     */
    friend ostream& operator<<(ostream& output_stream, const WishItem& item);

    size_t GetRarity() const;

    const string& GetImageUrl() const;

  private:
    const string kName;
    const string kImageUrl;
    const size_t kRarity;
    const string kType;
    const string kCategory;
};

} // namespace wishsimulator