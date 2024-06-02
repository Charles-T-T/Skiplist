cmake_minimum_required(VERSION 3.15)    

project(Skiplist)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(src MENU_SRC)

aux_source_directory(stress-test STRESS_SRC)

include_directories(include)

add_executable(menu ${MENU_SRC})

add_executable(stress_test ${STRESS_SRC})
