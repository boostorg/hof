include(CMakeParseArguments)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

function(bcm_write_package_template_header NAME)
    file(WRITE ${NAME} "

@PACKAGE_INIT@

include(CMakeFindDependencyMacro OPTIONAL RESULT_VARIABLE _CMakeFindDependencyMacro_FOUND)
if (NOT _CMakeFindDependencyMacro_FOUND)
  macro(find_dependency dep)
    if (NOT \${dep}_FOUND)
      set(cmake_fd_version)
      if (\${ARGC} GREATER 1)
        set(cmake_fd_version \${ARGV1})
      endif()
      set(cmake_fd_exact_arg)
      if(\${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION_EXACT)
        set(cmake_fd_exact_arg EXACT)
      endif()
      set(cmake_fd_quiet_arg)
      if(\${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
        set(cmake_fd_quiet_arg QUIET)
      endif()
      set(cmake_fd_required_arg)
      if(\${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
        set(cmake_fd_required_arg REQUIRED)
      endif()
      find_package(\${dep} \${cmake_fd_version}
          \${cmake_fd_exact_arg}
          \${cmake_fd_quiet_arg}
          \${cmake_fd_required_arg}
      )
      string(TOUPPER \${dep} cmake_dep_upper)
      if (NOT \${dep}_FOUND AND NOT \${cmake_dep_upper}_FOUND)
        set(\${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE \"\${CMAKE_FIND_PACKAGE_NAME} could not be found because dependency \${dep} could not be found.\")
        set(\${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
        return()
      endif()
      set(cmake_fd_version)
      set(cmake_fd_required_arg)
      set(cmake_fd_quiet_arg)
      set(cmake_fd_exact_arg)
    endif()
  endmacro()
endif()

")
endfunction()

function(bcm_write_package_template_function FILENAME NAME)
    string(REPLACE ";" " " ARGS "${ARGN}")
    file(APPEND ${FILENAME}
"
${NAME}(${ARGS})
")
endfunction()

function(bcm_install_targets)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs TARGETS INCLUDE DEPENDS)

    cmake_parse_arguments(PARSE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(LIB_TARGETS)
    set(EXE_TARGETS)

    foreach(TARGET ${PARSE_TARGETS})
        foreach(INCLUDE ${PARSE_INCLUDE})
            target_include_directories(${TARGET} INTERFACE $<BUILD_INTERFACE:${INCLUDE}>)
        endforeach()
        get_target_property(TARGET_TYPE ${TARGET} TYPE)
        if(${TARGET_TYPE} MATCHES "LIBRARY")
            list(APPEND LIB_TARGETS ${TARGET})
        else()
            list(APPEND EXE_TARGETS ${TARGET})
        endif()
    endforeach()

    string(TOUPPER ${PROJECT_NAME} PROJECT_UPPER)
    string(TOLOWER ${PROJECT_NAME} PROJECT_LOWER)

    set(TARGET_FILE ${PROJECT_LOWER}-targets)
    set(CONFIG_NAME ${PROJECT_LOWER}-config)
    set(TARGET_VERSION ${PROJECT}_VERSION)

    set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
    set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
    set(CONFIG_PACKAGE_INSTALL_DIR ${LIB_INSTALL_DIR}/cmake/${PROJECT_LOWER})

    set(CONFIG_TEMPLATE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_LOWER}-config.cmake.in")

    bcm_write_package_template_header(${CONFIG_TEMPLATE})

    foreach(DEPEND ${PARSE_DEPENDS})
        bcm_write_package_template_function(${CONFIG_TEMPLATE} find_dependency ${DEPEND})
    endforeach()

    bcm_write_package_template_function(${CONFIG_TEMPLATE} include "\${CMAKE_CURRENT_LIST_DIR}/${TARGET_FILE}.cmake")
    foreach(NAME ${PROJECT_NAME} ${PROJECT_UPPER} ${PROJECT_LOWER})
        bcm_write_package_template_function(${CONFIG_TEMPLATE} set_and_check ${NAME}_INCLUDE_DIR "@PACKAGE_INCLUDE_INSTALL_DIR@")
        bcm_write_package_template_function(${CONFIG_TEMPLATE} set_and_check ${NAME}_INCLUDE_DIRS "@PACKAGE_INCLUDE_INSTALL_DIR@")
        bcm_write_package_template_function(${CONFIG_TEMPLATE} set ${NAME}_LIBRARIES ${LIB_TARGETS})
    endforeach()

    configure_package_config_file(
        ${CONFIG_TEMPLATE}
        ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_NAME}.cmake
        INSTALL_DESTINATION ${CONFIG_PACKAGE_INSTALL_DIR}
        PATH_VARS LIB_INSTALL_DIR INCLUDE_INSTALL_DIR
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_NAME}-version.cmake
        VERSION ${TARGET_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    foreach(INCLUDE ${PARSE_INCLUDE})
        install(DIRECTORY ${INCLUDE}/ DESTINATION include)
    endforeach()

    install(TARGETS ${LIB_TARGETS} EXPORT ${TARGET_FILE}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    )

    install( EXPORT ${TARGET_FILE}
        DESTINATION
        ${CONFIG_PACKAGE_INSTALL_DIR}
    )

    install(TARGETS ${EXE_TARGETS}
        DESTINATION bin
    )

    install( FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_NAME}.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_NAME}-version.cmake
        DESTINATION
        ${CONFIG_PACKAGE_INSTALL_DIR})

endfunction()

