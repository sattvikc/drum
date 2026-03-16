cmake_minimum_required(VERSION 3.22)

project(DatoDrum VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DATODRUM_ENABLE_APP "Build JUCE standalone app" ON)
option(DATODRUM_ENABLE_TESTS "Build non-JUCE core tests" ON)

if (DATODRUM_ENABLE_APP)
  include(FetchContent)

  FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.2
  )

  FetchContent_MakeAvailable(JUCE)

  juce_add_gui_app(DatoDrum
    PRODUCT_NAME "Dato Drum"
    COMPANY_NAME "OpenAI"
  )

  target_sources(DatoDrum
    PRIVATE
      src/Main.cpp
      src/MainComponent.cpp
      src/TransportClock.cpp
      src/SequencerEngine.cpp
  )

  target_include_directories(DatoDrum PRIVATE include)

  target_compile_definitions(DatoDrum
    PRIVATE
      JUCE_WEB_BROWSER=0
      JUCE_USE_CURL=0
  )

  target_link_libraries(DatoDrum
    PRIVATE
      juce::juce_gui_extra
    PUBLIC
      juce::juce_recommended_config_flags
      juce::juce_recommended_lto_flags
      juce::juce_recommended_warning_flags
  )
endif()

if (DATODRUM_ENABLE_TESTS)
  enable_testing()

  add_executable(dato_core_tests
    tests/test_transport_and_sequencer.cpp
    src/TransportClock.cpp
    src/SequencerEngine.cpp
  )

  target_include_directories(dato_core_tests PRIVATE include)

  add_test(NAME dato_core_tests COMMAND dato_core_tests)
endif()
