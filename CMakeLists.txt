cmake_minimum_required(VERSION 3.14)

project(bookshelf)
set(CMAKE_C_COMPILER clang)
set(C_STANDARD 11)

include_directories(utils include)

add_library(utils SHARED
  src/utils/linked_list.c
  src/utils/hash.c
  src/utils/hashtable.c
  src/utils/clargs.c
)
add_library(cutest SHARED third_party/cutest/CuTest.c third_party/cutest/CuTest.h)

add_executable(test
  test/main.c
  test/utils/linked_list_test.c
  test/utils/hashtable_test.c
  test/utils/clargs_test.c
)

target_link_libraries(test cutest utils)
target_include_directories(test PRIVATE test third_party)

add_executable(demo
  demo/demo.c
)

target_link_libraries(demo utils)
