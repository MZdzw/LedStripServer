cmake_minimum_required(VERSION 3.22)

enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS_DEBUG   "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3")

if(ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -coverage -fno-exceptions")
endif()

set(CXX_WARNINGS "-Wall -Wextra")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/6910c9d9165801d8827d628cb72eb7ea9dd538c5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_SRCS    ${CMAKE_CURRENT_SOURCE_DIR}/Test_TcpIp.cpp)
set(PROD_INCS    ${CMAKE_SOURCE_DIR}/Src/Communication/
                 ${CMAKE_SOURCE_DIR}/Src/Common/
                 ${CMAKE_SOURCE_DIR}/Src/Interface/)
set(PROD_SRCS    ${CMAKE_SOURCE_DIR}/Src/Communication/TcpIp.cpp)
set(MOCK_INCS    ${CMAKE_SOURCE_DIR}/Tests/Mocks/)

add_executable(Test_TcpIp ${TEST_SRCS} ${PROD_SRCS})

target_compile_definitions(Test_TcpIp PRIVATE TESTING=1)
target_include_directories(Test_TcpIp PRIVATE ${PROD_INCS} ${MOCK_INCS})

target_link_libraries(Test_TcpIp PRIVATE
                      GTest::gmock
                      GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Test_TcpIp)