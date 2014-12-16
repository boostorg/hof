cmake_minimum_required (VERSION 2.6)
project (Fit)
 
# The version number.
set (Fit_VERSION_MAJOR 0)
set (Fit_VERSION_MINOR 1)

include(CheckCXXCompilerFlag)
enable_language(CXX)

if(CMAKE_HOST_APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(ENABLE_CXXFLAGS_TO_CHECK 
    -std=gnu++1y 
    -std=c++1y
    -std=gnu++11 
    -std=c++11
    -std=gnu++0x 
    -std=c++0x)

foreach(flag ${ENABLE_CXXFLAGS_TO_CHECK})
    string(REPLACE "-std=" "_" flag_var ${flag})
    string(REPLACE "+" "x" flag_var ${flag_var})
    check_cxx_compiler_flag("${flag}" COMPILER_HAS_CXX_FLAG${flag_var})
    if(COMPILER_HAS_CXX_FLAG${flag_var})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
        break()
    endif()
endforeach()

include(CTest)

include_directories(.)

install (DIRECTORY fit DESTINATION include)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

macro(add_test_executable TEST_NAME)
    add_executable (${TEST_NAME} EXCLUDE_FROM_ALL test/${TEST_NAME}.cpp)
    add_test(${TEST_NAME} ${TEST_NAME})
    add_dependencies(check ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
endmacro(add_test_executable)

add_test_executable(always)
add_test_executable(args)
add_test_executable(conditional)
add_test_executable(compose)
add_test_executable(fix)
add_test_executable(function)
add_test_executable(fuse)
add_test_executable(identity)
add_test_executable(implicit)
add_test_executable(indirect)
add_test_executable(infix)
add_test_executable(invoke)
add_test_executable(is_callable)
add_test_executable(lambda)
add_test_executable(lazy)
add_test_executable(match)
add_test_executable(mutable)
add_test_executable(by)
add_test_executable(pack)
add_test_executable(partial)
add_test_executable(placeholders)
add_test_executable(pipable)
add_test_executable(result)
add_test_executable(reveal)
add_test_executable(static)
add_test_executable(tap)
add_test_executable(variadic)
