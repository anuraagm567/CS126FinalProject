#include <visualizer/wish_simulator_app.h>

namespace wishsimulator {

namespace visualizer {

WishSimulatorApp::WishSimulatorApp()
    : wish_banner_(json::parse(std::ifstream(kBannerFilePath))),
      current_display_state_(DisplayState::BannerScreen) {
  current_image_ = ci::loadImage(ci::loadUrl(wish_banner_.GetImageUrl()));

  ifstream input_stream(kBannerSaveFile);
  if (input_stream.is_open()) {
    input_stream >> wish_banner_;

    input_stream.close();
  }

  // Add buttons to the map of buttons for each display state
  buttons_[DisplayState::BannerScreen].push_back(
      Button("Wish x1",
             vec2(kMargin,
                  current_image_->getHeight() - kButtonHeight - kMargin),
             kButtonHeight,
             kButtonWidth,
             kButtonColor,
             kLabelColor,
             kLabelFont));

  buttons_[DisplayState::BannerScreen].push_back(
      Button("Wish x10",
             vec2(2 * kMargin + kButtonWidth,
                  current_image_->getHeight() - kButtonHeight - kMargin),
             kButtonHeight,
             kButtonWidth,
             kButtonColor,
             kLabelColor,
             kLabelFont));

  buttons_[DisplayState::BannerScreen].push_back(
      Button("Inventory",
             vec2(kMargin,
                  current_image_->getHeight() - 2 * (kButtonHeight + kMargin)),
             kButtonHeight,
             kButtonWidth,
             kButtonColor,
             kLabelColor,
             kLabelFont));

  buttons_[DisplayState::BannerScreen].push_back(
      Button("Reset",
             vec2(2 * kMargin + kButtonWidth,
                  current_image_->getHeight() - 2 * (kButtonHeight + kMargin)),
             kButtonHeight,
             kButtonWidth,
             kButtonColor,
             kLabelColor,
             kLabelFont));

  buttons_[DisplayState::InventoryScreen].push_back(
      Button("Back to Banner",
             vec2(kMargin, kMargin),
             kButtonHeight,
             kButtonWidth,
             kButtonColor,
             kLabelColor,
             kLabelFont));

  buttons_[DisplayState::InventoryScreen].push_back(
      Button("Inventory",
             vec2((current_image_->getWidth() - kButtonWidth) / 2, kMargin),
             kButtonHeight,
             kButtonWidth,
             kButtonColor,
             kLabelColor,
             kLabelFont));
}

void WishSimulatorApp::draw() {
  if (current_display_state_ == DisplayState::InventoryScreen) {
    ci::gl::clear("white");

    float available_space = static_cast<float>(getWindowHeight()) -
                            4 * kMargin -
                            2 * kButtonHeight;

    size_t item_count;
    if (current_item_page_ * kItemsPerPage >=
        wish_banner_.GetInventory().size()) {
      if (current_item_page_ != 1 &&
          buttons_.at(current_display_state_).size() == 2) {
        buttons_[DisplayState::InventoryScreen].push_back(
            Button("Previous",
                   vec2(kMargin,
                        getWindowHeight() - kButtonHeight - kMargin),
                   kButtonHeight,
                   kButtonWidth,
                   kButtonColor,
                   kLabelColor,
                   kLabelFont));
      }

      if (current_item_page_ * kItemsPerPage >
          wish_banner_.GetInventory().size()) {
        item_count = wish_banner_.GetInventory().size() % kItemsPerPage;
      } else {
        item_count = kItemsPerPage;
      }
    } else {
      if (buttons_.at(current_display_state_).size() == 2) {
        if (current_item_page_ != 1) {
          buttons_[DisplayState::InventoryScreen].push_back(
              Button("Previous",
                     vec2(kMargin,
                          getWindowHeight() - kButtonHeight - kMargin),
                     kButtonHeight,
                     kButtonWidth,
                     kButtonColor,
                     kLabelColor,
                     kLabelFont));
        }

        buttons_[DisplayState::InventoryScreen].push_back(
            Button("Next",
                   vec2(getWindowWidth() - kMargin - kButtonWidth,
                        getWindowHeight() - kButtonHeight - kMargin),
                   kButtonHeight,
                   kButtonWidth,
                   kButtonColor,
                   kLabelColor,
                   kLabelFont));
      }

      item_count = kItemsPerPage;
    }

    float row_height = available_space / static_cast<float>(kItemsPerPage);

    ConfigureInventoryTable(item_count, row_height);

    ConfigureButtons();
  } else {
    if (getWindowWidth() != current_image_->getWidth() ||
        getWindowHeight() != current_image_->getHeight()) {
      setWindowSize(current_image_->getWidth(),
                    current_image_->getHeight());
    }

    ci::gl::draw(ci::gl::Texture2d::create(current_image_));

    ConfigureButtons();

    ci::gl::color(Color("white"));
  }
}

void WishSimulatorApp::mouseDown(ci::app::MouseEvent event) {
  switch (current_display_state_) {
    case DisplayState::BannerScreen: {
      // Performs wishes based on any selected buttons
      if (buttons_.at(current_display_state_).at(0).Contains(event.getPos())) {
        items_to_display_.push_back(wish_banner_.PerformMultipleWishes(1)
                                                .at(0));
      } else if (buttons_.at(current_display_state_)
                         .at(1)
                         .Contains(event.getPos())) {
        for (const WishItem& item: wish_banner_.PerformMultipleWishes(10)) {
          items_to_display_.push_back(item);
        }
      } else if (buttons_.at(current_display_state_)
                         .at(2)
                         .Contains(event.getPos())) {
        current_display_state_ = DisplayState::InventoryScreen;
      } else if (buttons_.at(current_display_state_)
                         .at(3)
                         .Contains(event.getPos())) {
        remove(kBannerSaveFile.c_str());

        ifstream input_stream(kBannerSaveFile);
        input_stream >> wish_banner_;
      }

      if (!items_to_display_.empty()) {
        current_image_ = ci::loadImage(
            ci::loadUrl(items_to_display_.front().GetImageUrl()));
        items_to_display_.pop_front();
        current_display_state_ = DisplayState::WishScreen;
      }

      break;
    }

    case DisplayState::WishScreen: {
      if (items_to_display_.empty()) {
        current_display_state_ = DisplayState::BannerScreen;
        current_image_ = ci::loadImage(ci::loadUrl(wish_banner_.GetImageUrl()));

        ofstream output_stream(kBannerSaveFile);

        if (output_stream.is_open()) {
          output_stream << wish_banner_;

          output_stream.close();
        }
      } else {
        current_image_ = ci::loadImage(
            ci::loadUrl(items_to_display_.front().GetImageUrl()));
        items_to_display_.pop_front();
      }

      break;
    }

    case DisplayState::InventoryScreen: {
      if (buttons_.at(current_display_state_).at(0).Contains(event.getPos())) {
        while (buttons_.at(current_display_state_).size() > 2) {
          buttons_.at(current_display_state_).pop_back();
        }

        current_display_state_ = DisplayState::BannerScreen;
        current_image_ = ci::loadImage(ci::loadUrl(wish_banner_.GetImageUrl()));
        current_item_page_ = 1;
      } else if (buttons_.at(current_display_state_).size() == 3 &&
                 buttons_.at(current_display_state_)
                         .at(2)
                         .Contains(event.getPos())) {
        if (current_item_page_ * kItemsPerPage >=
            wish_banner_.GetInventory().size()) {
          current_item_page_ -= 1;

          while (buttons_.at(current_display_state_).size() > 2) {
            buttons_.at(current_display_state_).pop_back();
          }
        } else {
          current_item_page_ += 1;

          while (buttons_.at(current_display_state_).size() > 2) {
            buttons_.at(current_display_state_).pop_back();
          }
        }
      } else if (buttons_.at(current_display_state_).size() == 4 &&
                 buttons_.at(current_display_state_)
                         .at(2)
                         .Contains(event.getPos())) {
        current_item_page_ -= 1;

        while (buttons_.at(current_display_state_).size() > 2) {
          buttons_.at(current_display_state_).pop_back();
        }
      } else if (buttons_.at(current_display_state_).size() == 4 &&
                 buttons_.at(current_display_state_)
                         .at(3)
                         .Contains(event.getPos())) {
        current_item_page_ += 1;

        while (buttons_.at(current_display_state_).size() > 2) {
          buttons_.at(current_display_state_).pop_back();
        }
      }
    }
  }
}

void WishSimulatorApp::ConfigureInventoryTable(size_t item_count,
                                               float row_height) const {
  for (size_t row = 0; row < item_count; row++) {
    float y_position = 2 * kMargin + kButtonHeight + row * row_height;
    size_t item_index = wish_banner_.GetInventory().size() -
                        (current_item_page_ - 1) * kItemsPerPage -
                        row - 1;
    string item_string = std::to_string(item_index + 1);
    item_string += ". ";
    item_string += wish_banner_.GetItemAt(item_index);

    ci::Rectf item(vec2(kMargin, y_position),
                   vec2(getWindowWidth() - kMargin,
                        y_position + row_height));
    ci::gl::color(kLabelColor);
    ci::gl::drawStrokedRect(item);
    ci::gl::color(kButtonColor);
    ci::gl::drawSolidRect(item);

    ci::gl::drawStringCentered(item_string,
                               vec2(kMargin +
                                    (getWindowWidth() - 2 * kMargin) / 2,
                                    y_position +
                                    row_height / 2.0f -
                                    kLabelFont.getSize() / 2),
                               kLabelColor,
                               kLabelFont);
  }
}

void WishSimulatorApp::ConfigureButtons() const {
  for (const Button& button: buttons_.at(current_display_state_)) {
    button.Draw();
  }
}

}  // namespace visualizer

}  // namespace wishsimulator