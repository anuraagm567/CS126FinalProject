#include "core/wish_item.h"

namespace wishsimulator {

WishItem::WishItem(const json& item): kName(item["name"]),
                                      kImageUrl(item["imageurl"]),
                                      kRarity(item["rarity"]),
                                      kType(item["type"]),
                                      kCategory(item["category"]) {}

size_t WishItem::GetRarity() const {
  return kRarity;
}

const string& WishItem::GetImageUrl() const {
  return kImageUrl;
}

ostream& operator<<(ostream& output_stream, const WishItem& item) {
  output_stream << item.kName + ": ";
  output_stream << item.kRarity;
  output_stream << " Star " + item.kCategory;
  output_stream << " (" + item.kType + ")";

  return output_stream;
}

} // namespace wishsimulator
