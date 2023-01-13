#include <visualizer/wish_simulator_app.h>

using wishsimulator::visualizer::WishSimulatorApp;

void prepareSettings(WishSimulatorApp::Settings* settings) {
  settings->setResizable(false);
}

CINDER_APP(WishSimulatorApp, ci::app::RendererGl, prepareSettings);