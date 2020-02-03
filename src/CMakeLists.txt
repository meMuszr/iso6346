file(GLOB SOURCES "*.c")

add_executable(iso6346-bin ${SOURCES})

target_link_libraries(iso6346-bin LINK_PUBLIC iso6346)
target_include_directories(iso6346-bin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(iso6346-bin PROPERTIES OUTPUT_NAME iso6346)

