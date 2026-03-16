#include "drum/TransportClock.h"

#include <algorithm>
#include <cmath>

namespace drum {

void TransportClock::setTempo(double newBpm) noexcept {
  bpm = std::clamp(newBpm, 40.0, 240.0);
}

double TransportClock::getTempo() const noexcept {
  return bpm;
}

void TransportClock::setSwing(double newSwing) noexcept {
  swing = std::clamp(newSwing, 0.0, 0.75);
}

double TransportClock::getSwing() const noexcept {
  return swing;
}

void TransportClock::start() noexcept {
  running = true;
}

void TransportClock::stop() noexcept {
  running = false;
}

bool TransportClock::isRunning() const noexcept {
  return running;
}

void TransportClock::advance(int numSamples, double sampleRate) noexcept {
  if (!running || sampleRate <= 0.0) {
    return;
  }

  const auto beatsPerSecond = bpm / 60.0;
  const auto beatIncrement = static_cast<double>(numSamples) * (beatsPerSecond / sampleRate);
  positionInBeats += beatIncrement;

  // 16-step sequencer over one 4/4 bar -> 4 steps per beat.
  const auto rawStep = static_cast<int>(std::floor(positionInBeats * 4.0));
  const auto step = ((rawStep % 16) + 16) % 16;
  currentStep = step;
}

int TransportClock::getCurrentStep() const noexcept {
  return currentStep;
}

} // namespace drum
