#pragma once

namespace drum {

class TransportClock {
public:
  void setTempo(double newBpm) noexcept;
  [[nodiscard]] double getTempo() const noexcept;

  void setSwing(double newSwing) noexcept;
  [[nodiscard]] double getSwing() const noexcept;

  void start() noexcept;
  void stop() noexcept;
  [[nodiscard]] bool isRunning() const noexcept;

  void advance(int numSamples, double sampleRate) noexcept;
  [[nodiscard]] int getCurrentStep() const noexcept;

private:
  double bpm {120.0};
  double swing {0.0};
  bool running {false};
  double positionInBeats {0.0};
  int currentStep {0};
};

} // namespace drum
