#add here custom compiler flags for this module

set(THIRD_PARTY_DEPENDENCIES
	pthread)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2 /Oi /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS")
	set(CMAKE_CXX_FLAGS_RELEASE_DEBUG "${CMAKE_CXX_FLAGS_RELEASE_DEBUG} /MD /Zi /Od /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D DDK_DEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Od /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D DDK_DEBUG")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++11 -D__LINUX__")
	set(CMAKE_CXX_FLAGS_RELEASE_DEBUG "${CMAKE_CXX_FLAGS_RELEASE_DEBUG} -g -Wall -std=c++11 -D__LINUX__ -DDDK_DEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -std=c++11 -D__LINUX__ -DDDK_DEBUG")

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

	set(THIRD_PARTY_DEPENDENCIES "")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE_DEBUG "${CMAKE_CXX_FLAGS_RELEASE_DEBUG} -std=c++11 -g -DDDK_DEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -std=c++11 -g -DDDK_DEBUG")

endif()
