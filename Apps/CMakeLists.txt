add_custom_target(FluxSystematicsEvaluatorApps)

foreach(app
    TestBed
  )

  add_executable(${app} ${app}.cpp)
  add_dependencies(FluxSystematicsEvaluatorApps ${app})
  target_link_libraries(${app} FluxSystematicsEvaluator::All)
  install(TARGETS ${app} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach(app)
