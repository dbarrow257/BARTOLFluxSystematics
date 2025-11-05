#================================================================================================
#Project details

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

#KS: Store some handy cmake functions
include(${CMAKE_CURRENT_LIST_DIR}/cmake/Modules/FluxSystematicsEvaluatorUtils.cmake)

set(FLUXSYSTEMATICSEVALUATOR_VERSION 1.4.4)
project(FLUXSYSTEMATICSEVALUATOR VERSION ${FLUXSYSTEMATICSEVALUATOR_VERSION} LANGUAGES CXX)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

#Changes default install path to be a subdirectory of the build dir.
#Can set build dir at configure time with -DCMAKE_INSTALL_PREFIX=/install/path
if(CMAKE_INSTALL_PREFIX STREQUAL "" OR CMAKE_INSTALL_PREFIX STREQUAL
    "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()

#================================================================================================
#Custom variable sanitisation
add_library(FluxSystematicsEvaluatorCompilerOptions INTERFACE)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.0/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

if(NOT DEFINED FluxSystematicsEvaluator_Compiler_Flags)
  set(FluxSystematicsEvaluator_Compiler_Flags "")
endif()

separate_arguments(FluxSystematicsEvaluator_Compiler_Flags)

cmessage(STATUS "Set compiler options: ${FluxSystematicsEvaluator_Compiler_Flags}")
add_compile_options(${FluxSystematicsEvaluator_Compiler_Flags})

#================================================================================================
#Setup Deps
set(YAML_CPP_VERSION 0.8.0)
CPMAddPackage(
    NAME yaml-cpp
    VERSION ${YAML_CPP_VERSION}
    GITHUB_REPOSITORY "jbeder/yaml-cpp"
    GIT_TAG "${YAML_CPP_VERSION}"
    OPTIONS
      "YAML_CPP_INSTALL ON"
      "YAML_CPP_BUILD_TESTS OFF"
      "YAML_CPP_BUILD_CONTRIB OFF"
      "YAML_BUILD_SHARED_LIBS ON"
)

if(NOT TARGET yaml-cpp::yaml-cpp)
  cmessage(FATAL_ERROR "FluxSystematicsEvaluator Expected dependency target: yaml-cpp::yaml-cpp")
endif()

find_package(ROOT 6.18 REQUIRED)

#================================================================================================
#Subdirectories to add

add_subdirectory(Systematics)
add_subdirectory(Apps)

#================================================================================================

set(LIBDEST ${CMAKE_INSTALL_PREFIX}/lib)

configure_file(cmake/Templates/setup.FluxSystematicsEvaluator.sh.in "${PROJECT_BINARY_DIR}/setup.FluxSystematicsEvaluator.sh" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/setup.FluxSystematicsEvaluator.sh" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

set_target_properties(FluxSystematicsEvaluatorCompilerOptions PROPERTIES EXPORT_NAME CompilerOptions)
install(TARGETS FluxSystematicsEvaluatorCompilerOptions
  EXPORT FluxSystematicsEvaluator-targets
  LIBRARY DESTINATION lib/)

#================================================================================================

#This is to export the target properties of FluxSystematicsEvaluator
#Anything that links to "FluxSystematicsEvaluator" will get all of these target properties
add_library(FluxSystematicsEvaluator INTERFACE)
target_link_libraries(FluxSystematicsEvaluator INTERFACE Systematics FluxSystematicsEvaluatorCompilerOptions)
set_target_properties(FluxSystematicsEvaluator PROPERTIES EXPORT_NAME All)

install(TARGETS FluxSystematicsEvaluator
  EXPORT FluxSystematicsEvaluator-targets
  LIBRARY DESTINATION lib/)

install(EXPORT FluxSystematicsEvaluator-targets
  FILE FluxSystematicsEvaluatorTargets.cmake
  NAMESPACE FluxSystematicsEvaluator::
  DESTINATION ${CMAKE_INSTALL_PREFIX}/
)

add_library(FluxSystematicsEvaluator::All ALIAS FluxSystematicsEvaluator)

############################  Install  ####################################
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/Templates/FluxSystematicsEvaluatorConfig.cmake.in ${CMAKE_BINARY_DIR}/FluxSystematicsEvaluatorConfig.cmake
  INSTALL_DESTINATION
  /this/is/ignored/for/some/reason/thanks/kitware
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(${CMAKE_BINARY_DIR}/FluxSystematicsEvaluatorConfigVersion.cmake
  VERSION ${FLUXSYSTEMATICSEVALUATOR_VERSION}
  COMPATIBILITY AnyNewerVersion)

install(FILES
  ${CMAKE_BINARY_DIR}/FluxSystematicsEvaluatorConfig.cmake
  ${CMAKE_BINARY_DIR}/FluxSystematicsEvaluatorConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/FluxSystematicsEvaluator)

include(fluxsystematicsevaluator-config)
