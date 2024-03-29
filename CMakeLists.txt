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
include(cmake/ObsPluginHelpers.cmake)

# Uncomment these lines if you want to use the OBS Frontend API in your plugin
find_package(obs-frontend-api REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)

# Uncomment those lines if you want to use Qt in your plugin
find_qt(COMPONENTS Widgets Core)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Qt::GuiPrivate)
set_target_properties(
  ${CMAKE_PROJECT_NAME}
  PROPERTIES AUTOMOC ON
             AUTOUIC ON
             AUTOUIC_SEARCH_PATHS "src/forms"
             AUTORCC ON)

configure_file(src/plugin-macros.h.in ${CMAKE_SOURCE_DIR}/src/plugin-macros.generated.h)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE src/plugin-macros.generated.h)

# /!\ TAKE NOTE: No need to edit things past this point /!\

# --- Platform-independent build settings ---

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

# --- End of section ---

# --- Windows-specific build settings and tasks ---
if(OS_WINDOWS)
  configure_file(cmake/bundle/windows/installer-Windows.iss.in
                 ${CMAKE_BINARY_DIR}/installer-Windows.generated.iss)

  if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4)
  endif()
  # --- End of section ---

  # -- macOS specific build settings and tasks --
elseif(OS_MACOS)
  configure_file(cmake/bundle/macos/installer-macos.pkgproj.in
                 ${CMAKE_BINARY_DIR}/installer-macos.generated.pkgproj)

  set(MACOSX_PLUGIN_GUI_IDENTIFIER "${MACOS_BUNDLEID}")
  set(MACOSX_PLUGIN_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}")
  set(MACOSX_PLUGIN_SHORT_VERSION_STRING "1")

  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall)
  # --- End of section ---

  # --- Linux-specific build settings and tasks ---
else()
  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall)
endif()
# --- End of section ---

setup_plugin_target(${CMAKE_PROJECT_NAME})
