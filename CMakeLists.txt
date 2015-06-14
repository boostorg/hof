cmake_minimum_required (VERSION 3.0)
project (Fit)
 
# The version number.
set (Fit_VERSION_MAJOR 0)
set (Fit_VERSION_MINOR 1)

include(CheckCXXCompilerFlag)
enable_language(CXX)

if(CMAKE_HOST_APPLE)
    list(APPEND CXX_EXTRA_FLAGS -stdlib=libc++)
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
        list(APPEND CXX_EXTRA_FLAGS ${flag})
        break()
    endif()
endforeach()

install (DIRECTORY fit DESTINATION include)

# Moved down, so everything above is common
if (BIICODE) 
    ADD_BII_TARGETS()
    target_compile_options(${BII_LIB_TARGET} INTERFACE ${CXX_EXTRA_FLAGS})
    return() #finish processing, no need to run cmake below
endif()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -VV -C ${CMAKE_CFG_INTDIR})

macro(add_test_executable TEST_NAME_)
    set(TEST_NAME "${TEST_NAME_}")
    add_executable (${TEST_NAME} EXCLUDE_FROM_ALL test/${TEST_NAME}.cpp ${ARGN})
    target_compile_options(${TEST_NAME} PUBLIC ${CXX_EXTRA_FLAGS})
    if(WIN32)
        add_test(NAME ${TEST_NAME} WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH} COMMAND ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX})
    else()
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endif()
    add_dependencies(check ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
endmacro(add_test_executable)

include(CTest)

include_directories(.)

add_test_executable(always)
add_test_executable(apply)
add_test_executable(apply_eval)
add_test_executable(args)
add_test_executable(by)
add_test_executable(capture)
add_test_executable(compose)
add_test_executable(compress)
add_test_executable(conditional)
add_test_executable(construct)
add_test_executable(fix)
add_test_executable(flip)
add_test_executable(flow)
add_test_executable(function)
add_test_executable(identity)
add_test_executable(implicit)
add_test_executable(indirect)
add_test_executable(infix)
add_test_executable(is_callable)
add_test_executable(issue8)
add_test_executable(lambda)
add_test_executable(lazy)
add_test_executable(match)
add_test_executable(mutable)
add_test_executable(pack)
add_test_executable(partial)
add_test_executable(pipable)
add_test_executable(placeholders)
add_test_executable(result)
add_test_executable(reveal)
add_test_executable(reverse_compress)
add_test_executable(static)
add_test_executable(static_def test/static_def2.cpp)
add_test_executable(tap)
add_test_executable(unpack)
add_test_executable(variadic)
