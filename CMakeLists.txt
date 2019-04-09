cmake_minimum_required(VERSION 3.13)
project(iq_loyalty_system_rkeeper)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
else(MSVC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
endif(MSVC)


find_package(cpprestsdk CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS log_setup log date_time)

add_library(iqloyalty SHARED lib.cpp)
target_link_libraries(iqloyalty PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal ${Boost_LIBRARIES})

include (GenerateExportHeader)
GENERATE_EXPORT_HEADER(iqloyalty)
target_include_directories(iqloyalty PUBLIC ${CMAKE_BINARY_DIR})

add_executable(test main.cpp)
target_link_libraries(test PRIVATE iqloyalty)
