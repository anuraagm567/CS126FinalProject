#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "visualizer/button.h"
#include "core/wish_item.h"
#include "core/wish_banner.h"

using ci::Color;
using ci::Font;
using glm::vec2;
using std::string;
using std::unordered_map;
using std::vector;
using std::ofstream;
using std::ifstream;

namespace wishsimulator {

namespace visualizer {

/**
 * Specifies possible display states of the simulator, which are used to
 * update the display based on user actions
 */
enum class DisplayState {
  BannerScreen,
  WishScreen,
  InventoryScreen
};

/**
 * Struct that handles hash values for enum classes to enable use in
 * unordered maps
 */
struct EnumClassHash {
  // Code below derived from:
  // https://stackoverflow.com/questions/18837857/
  // cant-use-enum-class-as-unordered-map-key/24847480#24847480
  template <typename T>
  size_t operator()(T t) const
  {
    return static_cast<size_t>(t);
  }
};

/**
 * Allows a user to simulate wishes with a visual display using Cinder
 */
class WishSimulatorApp : public ci::app::App {
public:
  WishSimulatorApp();

  void draw() override;
  void mouseDown(ci::app::MouseEvent event) override;

private:
  static constexpr size_t kMargin = 35;

  static constexpr size_t kButtonWidth = 150;
  static constexpr size_t kButtonHeight = 50;
  const Color kButtonColor = Color(36.0f/255, 60.0f/255, 102.0f/255);

  const Color kLabelColor = Color(229.0f/255, 209.0f/255, 142.0f/255);
  const Font kLabelFont = Font("arial", 20);

  static constexpr size_t kItemsPerPage = 4;
  size_t current_item_page_ = 1;

  unordered_map<DisplayState, vector<Button>, EnumClassHash> buttons_ = {
      {DisplayState::BannerScreen, {}},
      {DisplayState::WishScreen, {}},
      {DisplayState::InventoryScreen, {}}
  };

  const string kBannerFilePath = "data/albedobannerinfo.json";
  const string kBannerSaveFile = "data/bannersavedata.txt";
  WishBanner wish_banner_;
  std::list<WishItem> items_to_display_;
  ci::ImageSourceRef current_image_;
  DisplayState current_display_state_;

  /**
   * Sets up the table view for player inventory display
   * @param item_count Number of inventory items on current page
   * @param row_height Height of each row element in table
   */
  void ConfigureInventoryTable(size_t item_count, float row_height) const;

  /**
   * Sets up button display for simulator based on current display state
   */
  void ConfigureButtons() const;
};

}  // namespace visualizer

}  // namespace wishsimulator
