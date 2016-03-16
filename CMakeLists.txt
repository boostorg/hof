cmake_minimum_required (VERSION 2.8)
project (Fit)
 
# The version number.
set (Fit_VERSION_MAJOR 0)
set (Fit_VERSION_MINOR 4)

include(CheckCXXCompilerFlag)
enable_language(CXX)

if(CMAKE_HOST_APPLE)
  check_cxx_compiler_flag("-stdlib=libc++" COMPILER_HAS_CXX_FLAG_libcxx)
  if(COMPILER_HAS_CXX_FLAG_libcxx)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
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

install (DIRECTORY include/fit DESTINATION include)
configure_file(fit.pc.in fit.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fit.pc DESTINATION lib/pkgconfig)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -VV -C ${CMAKE_CFG_INTDIR})

macro(add_test_executable TEST_NAME)
    add_executable (${TEST_NAME} EXCLUDE_FROM_ALL ${ARGN})
    if(WIN32)
        add_test(NAME ${TEST_NAME} WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH} COMMAND ${TEST_NAME}${CMAKE_EXECUTABLE_SUFFIX})
    else()
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endif()
    add_dependencies(check ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
endmacro(add_test_executable)

macro(add_test_header TEST_NAME HEADER)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/header-include-${TEST_NAME}.cpp "#include <${HEADER}>\nint main() {}")
    add_test_executable(header-include-${TEST_NAME} ${CMAKE_CURRENT_BINARY_DIR}/header-include-${TEST_NAME}.cpp)
endmacro(add_test_header)

include(CTest)

include_directories(include)

file(GLOB TESTS test/*.cpp)
foreach(TEST ${TESTS})
    get_filename_component(BASE_NAME ${TEST} NAME_WE)
    add_test_executable(${BASE_NAME} ${TEST})
endforeach()
add_test_executable(static_def test/static_def/static_def.cpp test/static_def/static_def2.cpp)

file(GLOB HEADERS include/fit/*.hpp)
foreach(HEADER ${HEADERS})
    get_filename_component(BASE_NAME ${HEADER} NAME_WE)
    add_test_header(${BASE_NAME} fit/${BASE_NAME}.hpp)
endforeach()

set(BUILD_EXAMPLES on)
if (CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
        set(BUILD_EXAMPLES off)
    endif()
endif()

if (BUILD_EXAMPLES)
    file(GLOB EXAMPLES example/*.cpp)
    foreach(EXAMPLE ${EXAMPLES})
        get_filename_component(BASE_NAME ${EXAMPLE} NAME_WE)
        add_test_executable(example-${BASE_NAME} ${EXAMPLE})
    endforeach()
endif()
