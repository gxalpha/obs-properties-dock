cmake_minimum_required(VERSION 3.16...3.21)

project(obs-properties-dock VERSION 0.1.0)
add_library(${CMAKE_PROJECT_NAME} MODULE)
set(PLUGIN_AUTHOR "Sebastian Beckmann")
set(MACOS_BUNDLEID "net.sebastianbeckmann.obs-plugins.${CMAKE_PROJECT_NAME}")
set(LINUX_MAINTAINER_EMAIL "redacted@example.com")

target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE src/plugin-main.cpp
          src/properties-dock.cpp
          src/properties-dock.hpp
          src/transform-dock.cpp
          src/transform-dock.cpp
          src/transform-view.cpp
          src/transform-view.hpp
          src/obs-classes-helper-functions.hpp
          src/obs-classes/double-slider.cpp
          src/obs-classes/double-slider.hpp
          src/obs-classes/mute-checkbox.hpp
          src/obs-classes/properties-view.cpp
          src/obs-classes/properties-view.hpp
          src/obs-classes/properties-view.moc.hpp
          src/obs-classes/qt-wrappers.cpp
          src/obs-classes/qt-wrappers.hpp
          src/obs-classes/slider-absoluteset-style.cpp
          src/obs-classes/slider-absoluteset-style.hpp
          src/obs-classes/slider-ignorewheel.cpp
          src/obs-classes/slider-ignorewheel.hpp
          src/obs-classes/spinbox-ignorewheel.cpp
          src/obs-classes/spinbox-ignorewheel.hpp
          src/obs-classes/vertical-scroll-area.cpp
          src/obs-classes/vertical-scroll-area.hpp
          src/obs-classes/obs-app.hpp)

find_package(libobs REQUIRED)
find_package(obs-frontend-api REQUIRED)
include(cmake/ObsPluginHelpers.cmake)
find_qt(COMPONENTS Widgets Core)

configure_file(src/plugin-macros.h.in
               ${CMAKE_SOURCE_DIR}/src/plugin-macros.generated.h)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE src/plugin-macros.generated.h)

# --- Platform-independent build settings ---

target_include_directories(${CMAKE_PROJECT_NAME}
                           PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
  ${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs OBS::obs-frontend-api Qt::Core
                                Qt::Widgets)

if(OS_POSIX)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt::GuiPrivate)
endif()

set_target_properties(
  ${CMAKE_PROJECT_NAME}
  PROPERTIES AUTOMOC ON
             AUTOUIC ON
             AUTOUIC_SEARCH_PATHS "src/forms"
             AUTORCC ON)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_17)

# --- End of section ---

# --- Windows-specific build settings and tasks ---
if(OS_WINDOWS)
  configure_file(cmake/bundle/windows/installer-Windows.iss.in
                 ${CMAKE_BINARY_DIR}/installer-Windows.generated.iss)

  if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /MP /d2FH4-)
  endif()
  # --- End of section ---

  # -- macOS specific build settings and tasks --
elseif(OS_MACOS)
  configure_file(cmake/bundle/macos/installer-macos.pkgproj.in
                 ${CMAKE_BINARY_DIR}/installer-macos.generated.pkgproj)

  set(MACOSX_PLUGIN_GUI_IDENTIFIER "${MACOS_BUNDLEID}")
  set(MACOSX_PLUGIN_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}")
  set(MACOSX_PLUGIN_SHORT_VERSION_STRING "1")

  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE -Wall -Wextra -Werror-implicit-function-declaration -stdlib=libc++
            -fvisibility=default)

  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "")
  # --- End of section ---

  # --- Linux-specific build settings and tasks ---
else()
  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra)

  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "")
endif()
# --- End of section ---

setup_plugin_target(${CMAKE_PROJECT_NAME})
