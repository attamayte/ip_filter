cmake_minimum_required(VERSION 3.0)

project(ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

#set(Boost_USE_STATIC_LIBS           ON) 
#set(Boost_USE_MULTITHREADED         ON)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(ip_filter main.cpp)

add_library(ip_filter_lib lib.cpp)

add_executable(ip_filter_test test.cpp)

set_target_properties(ip_filter ip_filter_lib ip_filter_test PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(ip_filter_test PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(ip_filter
    ip_filter_lib
    Threads::Threads
)

target_link_libraries(ip_filter_test
    ${Boost_LIBRARIES}
    ip_filter_lib
)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT attamayte@gmail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(ip_filter_tests ip_filter_test)