# DATO Drum (JUCE)

A JUCE-based software prototype inspired by the DATO Drum concept.

## Current status

This repository contains:

- A JUCE standalone UI prototype (`Play`, `Stop`, `Tempo`, `Swing`)
- A `TransportClock` core class with step progression and basic swing behavior
- A `SequencerEngine` core class for 8x16 lane/step state + velocity
- A visual 8x16 grid with current-step highlighting
- Core logic tests that can run without JUCE/network access

## Build

### 1) Build the core tests only (no JUCE fetch, works offline)

```bash
cmake -S . -B build-core -DDATODRUM_ENABLE_APP=OFF
cmake --build build-core -j
ctest --test-dir build-core --output-on-failure
```

### 2) Build the JUCE standalone app

```bash
cmake -S . -B build
cmake --build build -j
```

> The first app configure requires internet access because JUCE is fetched via `FetchContent`.

## Project layout

- `CMakeLists.txt` – build options for app and core tests
- `src/Main.cpp` – app/window bootstrap
- `include/drum/MainComponent.h` + `src/MainComponent.cpp` – prototype UI
- `include/drum/TransportClock.h` + `src/TransportClock.cpp` – tempo/swing/playhead clocking
- `include/drum/SequencerEngine.h` + `src/SequencerEngine.cpp` – lane/step sequencing state
- `tests/test_transport_and_sequencer.cpp` – core behavior checks

## Next implementation steps

1. Move clock advancement into a real audio callback path.
2. Emit sequencer trigger events from `TransportClock` + `SequencerEngine` integration.
3. Add `DrumEngine` sample voice playback and per-lane gain/pan.
4. Make grid cells editable with mouse interaction.
5. Add project save/load for kits, patterns, tempo, and swing.
