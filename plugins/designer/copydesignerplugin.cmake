if(NOT BUILD_CONFIGURATION STREQUAL "Release"
   AND NOT BUILD_CONFIGURATION STREQUAL "RelWithDebInfo")
    return()
endif()

if(NOT EXISTS "${PLUGIN_FILE}")
    message(FATAL_ERROR "Designer plugin does not exist: ${PLUGIN_FILE}")
endif()

file(MAKE_DIRECTORY "${DESTINATION_DIR}")
execute_process(
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PLUGIN_FILE}"
        "${DESTINATION_DIR}"
    RESULT_VARIABLE copy_result
)
if(NOT copy_result EQUAL 0)
    message(FATAL_ERROR "Failed to copy Designer plugin to ${DESTINATION_DIR}")
endif()

message(STATUS "Copied Designer plugin to ${DESTINATION_DIR}")
