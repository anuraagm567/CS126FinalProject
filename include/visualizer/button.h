#pragma once

#include <string>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using ci::Color;
using ci::Font;
using std::string;
using glm::vec2;

namespace wishsimulator {

namespace visualizer {

/**
 * Class that stores the display properties of a button in the wish simulator
 * Handles both display setup and user selection depending on mouse location
 */
class Button {
  public:
    /**
     * Instantiates a button with given properties
     * @param name Name of button used in generating label string
     * @param coordinates X and Y positions of button
     * @param length Length of button (vertical)
     * @param width Width of button (horizontal)
     * @param label_color Color to be used for label string
     * @param label_font Font to be used for label string
     */
    Button(const string& name,
           const vec2& coordinates,
           size_t length,
           size_t width,
           const Color& button_color,
           const Color& label_color,
           const Font& label_font);

    /**
     * Method used to display button using stored properties
     */
    void Draw() const;

    /**
     * Determines whether a button contains the current position of the mouse
     * @param mouse_position X and Y coordinates of mouse
     * @return bool corresponding to whether button contains mouse position
     */
    bool Contains(const vec2& mouse_position) const;

  private:
    const string kName;
    const vec2 kCoordinates;
    const size_t kLength;
    const size_t kWidth;
    const Color kButtonColor;
    const Color kLabelColor;
    const Font kLabelFont;
};

} // namespace visualizer

} // namespace wishsimulator