add_executable(Example ${PROJECT_SOURCE_DIR}/example/src/example.c)

target_include_directories(Example PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(Example NikiScript)