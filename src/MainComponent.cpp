#include "drum/MainComponent.h"

#include <algorithm>

namespace drum {

MainComponent::MainComponent() {
  setSize(1024, 520);

  addAndMakeVisible(playButton);
  addAndMakeVisible(stopButton);
  playButton.addListener(this);
  stopButton.addListener(this);

  tempoSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  tempoSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
  tempoSlider.setRange(40.0, 240.0, 1.0);
  tempoSlider.setValue(120.0);
  tempoSlider.addListener(this);
  addAndMakeVisible(tempoSlider);

  swingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  swingSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
  swingSlider.setRange(0.0, 0.75, 0.01);
  swingSlider.setValue(0.0);
  swingSlider.addListener(this);
  addAndMakeVisible(swingSlider);

  statusLabel.setText("Stopped • Step 1", juce::dontSendNotification);
  addAndMakeVisible(statusLabel);

  // Seed a simple static pattern for visual feedback.
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numColumns; ++col) {
      grid[row][col] = (col % 4 == row % 4);
    }
  }

  startTimerHz(60);
}

void MainComponent::paint(juce::Graphics& g) {
  g.fillAll(juce::Colour::fromRGB(16, 16, 24));

  const auto bounds = getLocalBounds();
  auto gridArea = bounds.reduced(20, 20);
  gridArea.removeFromTop(90);

  g.setColour(juce::Colours::white.withAlpha(0.9f));
  g.setFont(16.0f);
  g.drawText("DATO Drum • JUCE Prototype", bounds.removeFromTop(30), juce::Justification::centred);

  const int cellW = std::max(1, gridArea.getWidth() / numColumns);
  const int cellH = std::max(1, gridArea.getHeight() / numRows);

  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numColumns; ++col) {
      auto cell = juce::Rectangle<int>(gridArea.getX() + col * cellW,
                                       gridArea.getY() + row * cellH,
                                       cellW - 2,
                                       cellH - 2);

      const bool isCurrentStep = (clock.getCurrentStep() == col);
      const auto baseColour = grid[row][col]
                                ? juce::Colour::fromRGB(0, 208, 160)
                                : juce::Colour::fromRGB(56, 60, 75);
      const auto drawColour = isCurrentStep ? baseColour.brighter(0.4f) : baseColour;

      g.setColour(drawColour);
      g.fillRoundedRectangle(cell.toFloat(), 4.0f);
    }
  }
}

void MainComponent::resized() {
  auto area = getLocalBounds().reduced(20);
  area.removeFromTop(30);

  auto controls = area.removeFromTop(60);
  playButton.setBounds(controls.removeFromLeft(90).reduced(4));
  stopButton.setBounds(controls.removeFromLeft(90).reduced(4));

  tempoSlider.setBounds(controls.removeFromLeft(320).reduced(4));
  swingSlider.setBounds(controls.removeFromLeft(320).reduced(4));
  statusLabel.setBounds(controls.reduced(4));
}

void MainComponent::timerCallback() {
  constexpr int simulatedBlockSize = 256;
  constexpr double simulatedSampleRate = 48000.0;
  clock.advance(simulatedBlockSize, simulatedSampleRate);

  const auto status = juce::String(clock.isRunning() ? "Playing" : "Stopped") +
                      " • Step " + juce::String(clock.getCurrentStep() + 1) +
                      " • Tempo " + juce::String(clock.getTempo(), 0) +
                      " BPM";

  statusLabel.setText(status, juce::dontSendNotification);
  repaint();
}

void MainComponent::buttonClicked(juce::Button* button) {
  if (button == &playButton) {
    clock.start();
  }

  if (button == &stopButton) {
    clock.stop();
  }
}

void MainComponent::sliderValueChanged(juce::Slider* slider) {
  if (slider == &tempoSlider) {
    clock.setTempo(tempoSlider.getValue());
  }

  if (slider == &swingSlider) {
    clock.setSwing(swingSlider.getValue());
  }
}

} // namespace drum
