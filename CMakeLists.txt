#================================================================================================
#Project details

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

#KS: Store some handy cmake functions
include(${CMAKE_CURRENT_LIST_DIR}/cmake/Modules/BARTOLFluxSystematicsEvaluatorUtils.cmake)

set(BARTOLFLUXSYSTEMATICSEVALUATOR_VERSION 1.0.0)
project(BARTOLFLUXSYSTEMATICSEVALUATOR VERSION ${BARTOLFLUXSYSTEMATICSEVALUATOR_VERSION} LANGUAGES CXX)

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
add_library(BARTOLFluxSystematicsEvaluatorCompilerOptions INTERFACE)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.0/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

if(NOT DEFINED BARTOLFluxSystematicsEvaluator_Compiler_Flags)
  set(BARTOLFluxSystematicsEvaluator_Compiler_Flags "")
endif()

separate_arguments(BARTOLFluxSystematicsEvaluator_Compiler_Flags)

cmessage(STATUS "Set compiler options: ${BARTOLFluxSystematicsEvaluator_Compiler_Flags}")
add_compile_options(${BARTOLFluxSystematicsEvaluator_Compiler_Flags})

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
  cmessage(FATAL_ERROR "BARTOLFluxSystematicsEvaluator Expected dependency target: yaml-cpp::yaml-cpp")
endif()

find_package(ROOT 6.18 REQUIRED)

#================================================================================================
#Subdirectories to add

add_subdirectory(Systematics)
add_subdirectory(Apps)

#================================================================================================

set(LIBDEST ${CMAKE_INSTALL_PREFIX}/lib)

configure_file(cmake/Templates/setup.BARTOLFluxSystematicsEvaluator.sh.in "${PROJECT_BINARY_DIR}/setup.BARTOLFluxSystematicsEvaluator.sh" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/setup.BARTOLFluxSystematicsEvaluator.sh" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

set_target_properties(BARTOLFluxSystematicsEvaluatorCompilerOptions PROPERTIES EXPORT_NAME CompilerOptions)
install(TARGETS BARTOLFluxSystematicsEvaluatorCompilerOptions
  EXPORT BARTOLFluxSystematicsEvaluator-targets
  LIBRARY DESTINATION lib/)

#================================================================================================

#This is to export the target properties of BARTOLFluxSystematicsEvaluator
#Anything that links to "BARTOLFluxSystematicsEvaluator" will get all of these target properties
add_library(BARTOLFluxSystematicsEvaluator INTERFACE)
target_link_libraries(BARTOLFluxSystematicsEvaluator INTERFACE BARTOLSystematics BARTOLFluxSystematicsEvaluatorCompilerOptions)
set_target_properties(BARTOLFluxSystematicsEvaluator PROPERTIES EXPORT_NAME All)

install(TARGETS BARTOLFluxSystematicsEvaluator
  EXPORT BARTOLFluxSystematicsEvaluator-targets
  LIBRARY DESTINATION lib/)

install(EXPORT BARTOLFluxSystematicsEvaluator-targets
  FILE BARTOLFluxSystematicsEvaluatorTargets.cmake
  NAMESPACE BARTOLFluxSystematicsEvaluator::
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/BARTOLFluxSystematicsEvaluator
)

add_library(BARTOLFluxSystematicsEvaluator::All ALIAS BARTOLFluxSystematicsEvaluator)

############################  Install  ####################################
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/Templates/BARTOLFluxSystematicsEvaluatorConfig.cmake.in ${CMAKE_BINARY_DIR}/BARTOLFluxSystematicsEvaluatorConfig.cmake
  INSTALL_DESTINATION
  /this/is/ignored/for/some/reason/thanks/kitware
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(${CMAKE_BINARY_DIR}/BARTOLFluxSystematicsEvaluatorConfigVersion.cmake
  VERSION ${BARTOLFLUXSYSTEMATICSEVALUATOR_VERSION}
  COMPATIBILITY AnyNewerVersion)

install(FILES
  ${CMAKE_BINARY_DIR}/BARTOLFluxSystematicsEvaluatorConfig.cmake
  ${CMAKE_BINARY_DIR}/BARTOLFluxSystematicsEvaluatorConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/BARTOLFluxSystematicsEvaluator)

include(bartolfluxsystematicsevaluator-config)
