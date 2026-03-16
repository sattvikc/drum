#include "drum/SequencerEngine.h"
#include "drum/TransportClock.h"

#include <cstdint>
#include <iostream>

namespace {

bool testTransportClockStepAdvance() {
  drum::TransportClock clock;
  clock.setTempo(120.0);
  clock.start();

  // 120 BPM => 2 beats/sec, so a 16th note is 0.125 sec.
  // 6000 samples @ 48k = 0.125 sec => exactly one step.
  clock.advance(6000, 48000.0);
  if (clock.getCurrentStep() != 1) {
    std::cerr << "Expected step 1 after one 16th, got " << clock.getCurrentStep() << '\n';
    return false;
  }

  clock.stop();
  if (clock.getCurrentStep() != 0) {
    std::cerr << "Expected step reset to 0 on stop\n";
    return false;
  }

  return true;
}

bool testSwingDelaysOffbeatStep() {
  drum::TransportClock straightClock;
  straightClock.setTempo(120.0);
  straightClock.setSwing(0.0);
  straightClock.start();

  drum::TransportClock swingClock;
  swingClock.setTempo(120.0);
  swingClock.setSwing(0.5);
  swingClock.start();

  // 0.26 beats is just after straight's first 16th (0.25 beat),
  // but before swung first step boundary (0.375 beat).
  straightClock.advance(6240, 48000.0);
  swingClock.advance(6240, 48000.0);

  if (straightClock.getCurrentStep() != 1) {
    std::cerr << "Expected straight step 1 at 0.26 beats\n";
    return false;
  }

  if (swingClock.getCurrentStep() != 0) {
    std::cerr << "Expected swung clock to remain on step 0 at 0.26 beats\n";
    return false;
  }

  return true;
}

bool testSequencerCoordinatesAndVelocity() {
  drum::SequencerEngine sequencer;

  sequencer.setStepEnabled(2, 5, true);
  if (!sequencer.isStepEnabled(2, 5)) {
    std::cerr << "Expected lane 2 step 5 enabled\n";
    return false;
  }

  sequencer.setStepVelocity(2, 5, static_cast<std::uint8_t>(127));
  if (sequencer.getStepVelocity(2, 5) != 127) {
    std::cerr << "Expected velocity 127\n";
    return false;
  }

  // Invalid accesses should be safely ignored / defaulted.
  sequencer.setStepEnabled(-1, 0, true);
  sequencer.setStepVelocity(99, 0, static_cast<std::uint8_t>(100));
  if (sequencer.isStepEnabled(-1, 0)) {
    std::cerr << "Invalid coordinate unexpectedly enabled\n";
    return false;
  }

  if (sequencer.getStepVelocity(99, 0) != 0) {
    std::cerr << "Invalid coordinate unexpectedly has velocity\n";
    return false;
  }

  return true;
}

} // namespace

int main() {
  const bool ok = testTransportClockStepAdvance() &&
                  testSwingDelaysOffbeatStep() &&
                  testSequencerCoordinatesAndVelocity();

  if (!ok) {
    return 1;
  }

  std::cout << "All transport/sequencer tests passed.\n";
  return 0;
}
