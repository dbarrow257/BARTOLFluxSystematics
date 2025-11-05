set(HEADERS SystematicBase.h Systematic_SolarActivity.h SystematicsHandler.h)

add_library(Systematics SHARED SystematicBase.cpp Systematic_SolarActivity.cpp SystematicsHandler.cpp)

target_link_libraries(Systematics yaml-cpp FluxSystematicsEvaluatorCompilerOptions ROOT::Core ROOT::Hist ROOT::Tree)

target_include_directories(Systematics PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS yaml-cpp
  EXPORT FluxSystematicsEvaluator-targets
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/yaml-cpp
)

install(DIRECTORY ${yaml-cpp_SOURCE_DIR}/include/
  DESTINATION include/
)

set_target_properties(Systematics PROPERTIES
  PUBLIC_HEADER "${HEADERS}"
  EXPORT_NAME Systematics)

install(TARGETS Systematics
  EXPORT FluxSystematicsEvaluator-targets
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/Systematics)

add_library(Systematics::Systematics ALIAS Systematics)
