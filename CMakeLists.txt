cmake_minimum_required(VERSION 3.21)
project(OpenCutStudio VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Performance: build leve para notebooks modestos
if(MSVC)
  add_compile_options(/O1 /Os /EHsc)
else()
  add_compile_options(-Os -ffunction-sections -fdata-sections)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Concurrent)

set(SOURCES
  src/main.cpp
  src/core/Project.cpp
  src/core/FFmpegRunner.cpp
  src/ui/MainWindow.cpp
  src/ui/TimelineWidget.cpp
  src/ui/PreviewWidget.cpp
  src/ui/InspectorWidget.cpp
  src/ui/MediaBinWidget.cpp
  src/ui/ColorPanel.cpp
  src/ui/EffectsPanel.cpp
  src/ui/AudioMixer.cpp
  src/ui/DeliverPanel.cpp
  src/effects/EffectChain.cpp
)

set(HEADERS
  src/core/Project.h
  src/core/FFmpegRunner.h
  src/ui/MainWindow.h
  src/ui/TimelineWidget.h
  src/ui/PreviewWidget.h
  src/ui/InspectorWidget.h
  src/ui/MediaBinWidget.h
  src/ui/ColorPanel.h
  src/ui/EffectsPanel.h
  src/ui/AudioMixer.h
  src/ui/DeliverPanel.h
  src/ui/Theme.h
  src/effects/EffectChain.h
)

add_executable(OpenCutStudio WIN32 ${SOURCES} ${HEADERS})

target_include_directories(OpenCutStudio PRIVATE src)
target_link_libraries(OpenCutStudio PRIVATE
  Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets Qt6::Concurrent
)

# Pasta de saída
set_target_properties(OpenCutStudio PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

install(TARGETS OpenCutStudio RUNTIME DESTINATION .)
