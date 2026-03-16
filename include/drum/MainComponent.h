#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "drum/TransportClock.h"

namespace drum {

class MainComponent final : public juce::Component,
                            private juce::Timer,
                            private juce::Button::Listener,
                            private juce::Slider::Listener {
public:
  MainComponent();

  void paint(juce::Graphics& g) override;
  void resized() override;

private:
  void timerCallback() override;
  void buttonClicked(juce::Button* button) override;
  void sliderValueChanged(juce::Slider* slider) override;

  static constexpr int numRows {8};
  static constexpr int numColumns {16};

  juce::TextButton playButton {"Play"};
  juce::TextButton stopButton {"Stop"};
  juce::Slider tempoSlider;
  juce::Slider swingSlider;
  juce::Label statusLabel;

  bool grid[numRows][numColumns] {};

  TransportClock clock;
};

} // namespace drum
