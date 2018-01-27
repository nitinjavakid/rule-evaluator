cmake_minimum_required(VERSION 3.7)

find_package(BISON)

BISON_TARGET(RuleEvaluator evaluator.y++ ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(rule_evaluator evaluator.cpp ${BISON_RuleEvaluator_OUTPUTS})
target_link_libraries(rule_evaluator -static-libgcc -static-libstdc++ -static)
