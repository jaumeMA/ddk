#add here custom compiler flags for this module

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2 /Oi /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D QT_CORE_LIB /D QT_SQL_LIB /D QT_XML_LIB /D QT_DLL /D QT_NO_DEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE_DEBUG "${CMAKE_CXX_FLAGS_RELEASE_DEBUG} /MD /Zi /Od /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D QT_CORE_LIB /D QT_SQL_LIB /D QT_XML_LIB /D QT_DLL /D QT_NO_DEBUG /D EWAS_DEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Od /D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS /D QT_CORE_LIB /D QT_SQL_LIB /D QT_XML_LIB /D QT_DLL /D QT_DEBUG /D EWAS_DEBUG")

	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/NOLOGO /INCREMENTAL:NO")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE_DEBUG "/NOLOGO /DEBUG /ASSEMBLYDEBUG")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/NOLOGO /DEBUG /ASSEMBLYDEBUG /NODEFAULTLIB:\"msvcrt.lib\"")

endif()
