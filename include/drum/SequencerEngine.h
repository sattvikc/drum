#pragma once

#include <array>
#include <cstdint>

namespace drum {

class SequencerEngine {
public:
  static constexpr int numLanes {8};
  static constexpr int numSteps {16};

  void setStepEnabled(int lane, int step, bool enabled) noexcept;
  [[nodiscard]] bool isStepEnabled(int lane, int step) const noexcept;

  void setStepVelocity(int lane, int step, std::uint8_t velocity) noexcept;
  [[nodiscard]] std::uint8_t getStepVelocity(int lane, int step) const noexcept;

private:
  static bool isValidCoordinate(int lane, int step) noexcept;

  std::array<std::array<bool, numSteps>, numLanes> enabledSteps {};
  std::array<std::array<std::uint8_t, numSteps>, numLanes> stepVelocities {};
};

} // namespace drum
