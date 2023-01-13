#include <visualizer/button.h>

namespace wishsimulator {

namespace visualizer {

Button::Button(const string& name,
               const vec2& coordinates,
               size_t length,
               size_t width,
               const Color& button_color,
               const Color& label_color,
               const Font& label_font)
    : kName(name),
      kCoordinates(coordinates),
      kLength(length),
      kWidth(width),
      kButtonColor(button_color),
      kLabelColor(label_color),
      kLabelFont(label_font) {}

void Button::Draw() const {
  ci::gl::color(kButtonColor);

  ci::Rectf button(kCoordinates,
                   vec2(kCoordinates.x + kWidth, kCoordinates.y + kLength));
  ci::gl::drawSolidRect(button);
  ci::gl::drawStringCentered(kName,
                             vec2(kCoordinates.x + kWidth / 2.0f,
                                  kCoordinates.y +
                                  kLength / 2.0f -
                                  kLabelFont.getSize() / 2),
                             kLabelColor,
                             kLabelFont);
}

bool Button::Contains(const vec2& mouse_position) const {
  ci::Rectf button(kCoordinates,
                   vec2(kCoordinates.x + kWidth, kCoordinates.y + kLength));

  return button.contains(mouse_position);
}

} // namespace visualizer

} // namespace wishsimulator