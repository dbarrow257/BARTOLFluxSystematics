add_custom_target(BARTOLFluxSystematicsEvaluatorApps)

foreach(app
    TestBed
  )

  add_executable(${app} ${app}.cpp)
  add_dependencies(BARTOLFluxSystematicsEvaluatorApps ${app})
  target_link_libraries(${app} BARTOLFluxSystematicsEvaluator::All)
  install(TARGETS ${app} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach(app)
