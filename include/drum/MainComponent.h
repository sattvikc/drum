#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "drum/SequencerEngine.h"
#include "drum/TransportClock.h"

namespace drum {

class MainComponent final : public juce::Component,
                            private juce::Timer {
public:
  MainComponent();

  void paint(juce::Graphics& g) override;
  void resized() override;

private:
  void timerCallback() override;

  juce::TextButton playButton {"Play"};
  juce::TextButton stopButton {"Stop"};
  juce::Slider tempoSlider;
  juce::Slider swingSlider;
  juce::Label statusLabel;

  SequencerEngine sequencer;
  TransportClock clock;
};

} // namespace drum
