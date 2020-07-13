set(DEPENDENCIES)
set(LOADED_DEPENDENCIES)

foreach(BINARY ${Binaries})

	if(EXISTS "${TEST_DIR}/${BINARY}")
	
		add_subdirectory("${TEST_DIR}/${BINARY}" "${BINARY}")

		target_include_directories(${BINARY} PUBLIC "${TEST_DIR}/${BINARY}")

		set_target_properties(${BINARY} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

		list(APPEND DEPENDENCIES ${MODULE_DEPENDENCIES})

	else()

		message("Error: folder ${TEST_DIR}/${BINARY} doesn't exist!")

	endif()

	list(APPEND LOADED_DEPENDENCIES ${BINARY})

endforeach()

list(LENGTH DEPENDENCIES N)

while(${N} GREATER 0)

	list(GET DEPENDENCIES 0 DEPENDENCY)
	list(REMOVE_AT DEPENDENCIES 0)

	list(FIND LOADED_DEPENDENCIES ${DEPENDENCY} N)

	if(${N} EQUAL -1)

		if(EXISTS "${SRC_DIR}/${DEPENDENCY}")

			add_subdirectory("${SRC_DIR}/${DEPENDENCY}" "${DEPENDENCY}")

			target_include_directories(${DEPENDENCY} PUBLIC "${SRC_DIR}/${DEPENDENCY}")

			set_target_properties(${DEPENDENCY} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

			list(APPEND DEPENDENCIES ${MODULE_DEPENDENCIES})

		else()

			message("Error: folder ${SRC_DIR}/${DEPENDENCY} doesn't exist!")

		endif()

		list(APPEND LOADED_DEPENDENCIES ${DEPENDENCY})

	endif()

	list(LENGTH DEPENDENCIES N)

endwhile()

set(INSTALL_GTEST OFF CACHE BOOL "Projects embedding googletest do want to turn this OFF." FORCE)

set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib." FORCE)

add_subdirectory(${TEST_DIR}/googletest EXCLUDE_FROM_ALL)
