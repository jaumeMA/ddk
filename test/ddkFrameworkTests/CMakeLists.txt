cmake_minimum_required(VERSION 3.13.0)

include(WhiteList.cmake)
include(CompilerFlags.cmake)

project(ddkFrameworkTests)

add_executable(ddkFrameworkTests ${SOURCES} ${HEADERS})

#internal dependencies
set(LOCAL_MODULE_DEPENDENCIES ddkFramework)

set(MODULE_DEPENDENCIES ${LOCAL_MODULE_DEPENDENCIES} PARENT_SCOPE)

target_include_directories(ddkFrameworkTests PUBLIC
	"${SRC_DIR}/ddkFramework")

target_link_libraries(ddkFrameworkTests
	${THIRD_PARTY_DEPENDENCIES}
	gmock_main
	${LOCAL_MODULE_DEPENDENCIES})

set_target_properties(ddkFrameworkTests PROPERTIES FOLDER "1.Tests")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

  set_target_properties(ddkFrameworkTests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}/$<CONFIG>")
  add_custom_command(TARGET ddkFrameworkTests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,copy_if_different,echo_append> ${THIRD_PARTY_PDBS} $<$<CONFIG:Debug>:"${OUTPUT_DIR}/Debug">)
  add_custom_command(TARGET ddkFrameworkTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${THIRD_PARTY_BINARIES} "${OUTPUT_DIR}/$<CONFIG>")

endif()
