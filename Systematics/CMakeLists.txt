set(HEADERS BARTOLSystematicBase.h BARTOLSystematic_SolarActivity.h BARTOLSystematicsHandler.h BARTOLSystematic_CosmicNormalisation.h)
add_library(BARTOLSystematics SHARED BARTOLSystematicBase.cpp BARTOLSystematic_SolarActivity.cpp BARTOLSystematicsHandler.cpp BARTOLSystematic_CosmicNormalisation.cpp)
set_target_properties(BARTOLSystematics PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
target_link_libraries(BARTOLSystematics BARTOLFluxSystematicsEvaluatorCompilerOptions ROOT::Core ROOT::Hist ROOT::Tree)

target_include_directories(BARTOLSystematics PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(BARTOLSystematics PROPERTIES
  PUBLIC_HEADER "${HEADERS}"
  EXPORT_NAME BARTOLSystematics)

install(TARGETS BARTOLSystematics
  EXPORT BARTOLFluxSystematicsEvaluator-targets
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/Systematics)

add_library(BARTOLSystematics::BARTOLSystematics ALIAS BARTOLSystematics)
