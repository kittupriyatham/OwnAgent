cmake_minimum_required(VERSION 3.20)
project(OwnAgent CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch nlohmann_json
FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_library(asl_parser src/tokenizer.cpp src/parser.cpp src/command_model.cpp)
target_include_directories(asl_parser PUBLIC include)
target_link_libraries(asl_parser PUBLIC nlohmann_json::nlohmann_json)

add_executable(ownagent src/main.cpp)
target_link_libraries(ownagent PRIVATE asl_parser)

enable_testing()
add_executable(asl_tests test/test_parser.cpp)
target_link_libraries(asl_tests PRIVATE asl_parser GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(asl_tests)
