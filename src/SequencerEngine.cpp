#include "drum/SequencerEngine.h"

#include <algorithm>
#include <cstddef>

namespace drum {

void SequencerEngine::setStepEnabled(int lane, int step, bool enabled) noexcept {
  if (!isValidCoordinate(lane, step)) {
    return;
  }

  enabledSteps[static_cast<std::size_t>(lane)][static_cast<std::size_t>(step)] = enabled;
}

bool SequencerEngine::isStepEnabled(int lane, int step) const noexcept {
  if (!isValidCoordinate(lane, step)) {
    return false;
  }

  return enabledSteps[static_cast<std::size_t>(lane)][static_cast<std::size_t>(step)];
}

void SequencerEngine::setStepVelocity(int lane, int step, std::uint8_t velocity) noexcept {
  if (!isValidCoordinate(lane, step)) {
    return;
  }

  stepVelocities[static_cast<std::size_t>(lane)][static_cast<std::size_t>(step)] =
    static_cast<std::uint8_t>(std::clamp<int>(velocity, 1, 127));
}

std::uint8_t SequencerEngine::getStepVelocity(int lane, int step) const noexcept {
  if (!isValidCoordinate(lane, step)) {
    return 0;
  }

  return stepVelocities[static_cast<std::size_t>(lane)][static_cast<std::size_t>(step)];
}

bool SequencerEngine::isValidCoordinate(int lane, int step) noexcept {
  return lane >= 0 && lane < numLanes && step >= 0 && step < numSteps;
}

} // namespace drum
