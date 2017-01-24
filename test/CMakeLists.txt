find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${b64_SOURCE_DIR}/src
                    ${Boost_INCLUDE_DIRS}
)

add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(Test Test.cpp)

target_link_libraries(Test
                      b64
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
