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
  positionInBeats = 0.0;
  currentStep = 0;
}

bool TransportClock::isRunning() const noexcept {
  return running;
}

void TransportClock::advance(int numSamples, double sampleRate) noexcept {
  if (!running || sampleRate <= 0.0 || numSamples <= 0) {
    return;
  }

  const auto beatsPerSecond = bpm / 60.0;
  positionInBeats += static_cast<double>(numSamples) * (beatsPerSecond / sampleRate);

  constexpr double barLengthInBeats = 4.0;
  constexpr double pairLengthInBeats = 0.5;
  const auto barPos = std::fmod(positionInBeats, barLengthInBeats);
  const auto safeBarPos = barPos < 0.0 ? barPos + barLengthInBeats : barPos;

  const int pairIndex = static_cast<int>(safeBarPos / pairLengthInBeats);
  const double pairPhase = safeBarPos - static_cast<double>(pairIndex) * pairLengthInBeats;

  const double firstStepLength = 0.25 * (1.0 + swing);
  const int stepInPair = (pairPhase < firstStepLength) ? 0 : 1;

  currentStep = pairIndex * 2 + stepInPair;
}

int TransportClock::getCurrentStep() const noexcept {
  return currentStep;
}

} // namespace drum
