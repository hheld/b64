cmake_minimum_required(VERSION 3.6)

set(CXX_STANDARD 14)

project(b64 C CXX)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME Test COMMAND Test)
add_test(NAME Test_memcheck COMMAND valgrind
    --error-exitcode=1
    --tool=memcheck
    --track-origins=yes
    --show-leak-kinds=all
    --leak-check=yes
    --errors-for-leak-kinds=definite
    ./test/Test)
