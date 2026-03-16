#include "drum/MainComponent.h"

#include <algorithm>

namespace drum {

MainComponent::MainComponent() {
  setSize(1024, 520);

  addAndMakeVisible(playButton);
  addAndMakeVisible(stopButton);
  playButton.onClick = [this] { clock.start(); };
  stopButton.onClick = [this] { clock.stop(); };

  tempoSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  tempoSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
  tempoSlider.setRange(40.0, 240.0, 1.0);
  tempoSlider.setValue(120.0);
  tempoSlider.onValueChange = [this] { clock.setTempo(tempoSlider.getValue()); };
  addAndMakeVisible(tempoSlider);

  swingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  swingSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
  swingSlider.setRange(0.0, 0.75, 0.01);
  swingSlider.setValue(0.0);
  swingSlider.onValueChange = [this] { clock.setSwing(swingSlider.getValue()); };
  addAndMakeVisible(swingSlider);

  statusLabel.setText("Stopped • Step 1", juce::dontSendNotification);
  addAndMakeVisible(statusLabel);

  for (int lane = 0; lane < SequencerEngine::numLanes; ++lane) {
    for (int step = 0; step < SequencerEngine::numSteps; ++step) {
      const bool isEnabled = (step % 4 == lane % 4);
      sequencer.setStepEnabled(lane, step, isEnabled);
      if (isEnabled) {
        sequencer.setStepVelocity(lane, step, 100);
      }
    }
  }

  startTimerHz(60);
}

void MainComponent::paint(juce::Graphics& g) {
  g.fillAll(juce::Colour::fromRGB(16, 16, 24));

  auto titleArea = getLocalBounds().removeFromTop(30);
  g.setColour(juce::Colours::white.withAlpha(0.9f));
  g.setFont(16.0f);
  g.drawText("DATO Drum • JUCE Prototype", titleArea, juce::Justification::centred);

  auto gridArea = getGridArea();
  const int cellW = std::max(1, gridArea.getWidth() / SequencerEngine::numSteps);
  const int cellH = std::max(1, gridArea.getHeight() / SequencerEngine::numLanes);

  for (int lane = 0; lane < SequencerEngine::numLanes; ++lane) {
    for (int step = 0; step < SequencerEngine::numSteps; ++step) {
      auto cell = juce::Rectangle<int>(gridArea.getX() + step * cellW,
                                       gridArea.getY() + lane * cellH,
                                       cellW - 2,
                                       cellH - 2);

      const bool isCurrentStep = (clock.getCurrentStep() == step);
      const bool enabled = sequencer.isStepEnabled(lane, step);

      const auto baseColour = enabled
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

void MainComponent::mouseDown(const juce::MouseEvent& event) {
  int lane = 0;
  int step = 0;
  if (!tryMapPositionToCell(event.getPosition(), lane, step)) {
    return;
  }

  const bool enabled = sequencer.isStepEnabled(lane, step);
  sequencer.setStepEnabled(lane, step, !enabled);
  if (!enabled) {
    sequencer.setStepVelocity(lane, step, 100);
  }

  repaint();
}

juce::Rectangle<int> MainComponent::getGridArea() const {
  auto area = getLocalBounds().reduced(20, 20);
  area.removeFromTop(90);
  return area;
}

bool MainComponent::tryMapPositionToCell(juce::Point<int> position, int& laneOut, int& stepOut) const {
  const auto gridArea = getGridArea();
  if (!gridArea.contains(position)) {
    return false;
  }

  const auto relativeX = position.x - gridArea.getX();
  const auto relativeY = position.y - gridArea.getY();

  const int cellW = std::max(1, gridArea.getWidth() / SequencerEngine::numSteps);
  const int cellH = std::max(1, gridArea.getHeight() / SequencerEngine::numLanes);

  stepOut = std::clamp(relativeX / cellW, 0, SequencerEngine::numSteps - 1);
  laneOut = std::clamp(relativeY / cellH, 0, SequencerEngine::numLanes - 1);
  return true;
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

} // namespace drum
