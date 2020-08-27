cmake_minimum_required (VERSION 3.15)

# Platform

if (WIN32 OR MINGW)
	include ("${PROJECT_SOURCE_DIR}/cmake/platforms/win32.cmake")
elseif (UNIX)
	include ("${PROJECT_SOURCE_DIR}/cmake/platforms/unix.cmake")
else ()
	message (FATAL_ERROR "Undefined platform!")
endif ()

# Libraries

set (GENERAL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

find_package (Angelscript CONFIG REQUIRED)
find_package (EnTT CONFIG REQUIRED)

include ("${PROJECT_SOURCE_DIR}/cmake/modules/FindGLM.cmake")
if (NOT GLM_FOUND)
	message (FATAL_ERROR "Could not find glm!")
endif ()

include ("${PROJECT_SOURCE_DIR}/cmake/modules/FindFMT.cmake")
if (NOT FMT_FOUND)
	message (FATAL_ERROR "Could not find fmt!")
endif ()

include ("${PROJECT_SOURCE_DIR}/cmake/modules/FindTmxlite.cmake")
if (NOT TMXLITE_FOUND)
	message (FATAL_ERROR "Could not find tmxlite!")
endif ()

if (VCPKG_TOOLCHAIN)
	find_package (OpenAL CONFIG REQUIRED)
	set (OPENAL_LIBRARY OpenAL::OpenAL)
	find_package (SDL2 CONFIG REQUIRED)
	if (APPLE)
		set (SDL2_LIBRARIES SDL2::SDL2)
	elseif (MINGW)
		set (SDL2_LIBRARIES "-lmingw32" SDL2::SDL2main SDL2::SDL2 "-mwindows")
	else ()
		set (SDL2_LIBRARIES SDL2::SDL2main SDL2::SDL2)
	endif ()
else ()
	include ("${PROJECT_SOURCE_DIR}/cmake/modules/FindOpenAL.cmake")
	if (NOT OPENAL_FOUND)
		message (FATAL_ERROR "Could not find OpenAL!")
	endif ()
	include ("${PROJECT_SOURCE_DIR}/cmake/modules/FindSDL2.cmake")
	if (NOT SDL2_FOUND)
		message (FATAL_ERROR "Could not find SDL2!")
	elseif (NOT SDL2MAIN_FOUND AND NOT APPLE)
		message (FATAL_ERROR "Could not find SDL2-Main!")
	endif ()
endif ()

# Link/Include

if (NAOMI)
	target_link_libraries (naomi PRIVATE
		Angelscript::angelscript
		EnTT::EnTT
		${TMXLITE_LIBRARY}
		${FMT_LIBRARY}
		${OPENAL_LIBRARY}
		${SDL2_LIBRARIES}
	)
	target_include_directories (naomi PRIVATE
		${GENERAL_INCLUDE_DIR}
		${TMXLITE_INCLUDE_DIR}
		${FMT_INCLUDE_DIR}
		${OPENAL_INCLUDE_DIR}
		${SDL2_INCLUDE_DIR}
	)
endif ()

if (EDITOR)
	target_link_libraries (editor PRIVATE
		${FMT_LIBRARY}
		${SDL2_LIBRARIES}
	)
	target_include_directories (editor PRIVATE
		${GENERAL_INCLUDE_DIR}
		${STB_INCLUDE_DIR}
		${FMT_INCLUDE_DIR}
		${SDL2_INCLUDE_DIR}
	)
endif ()