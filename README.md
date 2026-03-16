# DATO Drum (JUCE)

A JUCE-based software prototype inspired by the DATO Drum concept.

## Current status

This repository now contains a runnable JUCE standalone application skeleton with:

- A transport section (`Play`, `Stop`, `Tempo`, `Swing`)
- A simple `TransportClock` implementation
- A visual 8x16 drum-step grid with current-step highlighting
- A modular structure to evolve toward a real sequencer and drum engine

## Build (standalone app)

### Requirements

- CMake 3.22+
- C++20 compiler
- Internet access on first configure (JUCE fetched from GitHub)

### Commands

```bash
cmake -S . -B build
cmake --build build -j
```

## Project layout

- `CMakeLists.txt` – JUCE app target and build config
- `src/Main.cpp` – app and window bootstrap
- `include/drum/MainComponent.h` + `src/MainComponent.cpp` – prototype UI
- `include/drum/TransportClock.h` + `src/TransportClock.cpp` – clock state and step progression

## Next implementation steps

1. Move timing advancement from timer simulation into the real audio callback.
2. Add `SequencerEngine` for lane/step state and lock-free trigger events.
3. Add `DrumEngine` for sample voice playback and per-lane gain/pan.
4. Replace static pattern drawing with editable step toggles.
5. Add project save/load for kits, patterns, tempo, and swing.
