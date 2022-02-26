cmake_minimum_required(VERSION 3.22)

project(
  vmouse_client
  VERSION 0.1
  DESCRIPTION "Virtual mouse client"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

find_path(WINHID_INCLUDE_DIR
		NAMES
		hidsdi.h
		PATHS
		"${WINHID_ROOT_DIR}"
		${MINGWSEARCH_INCLUDE_DIRS}
		/mingw
		PATH_SUFFIXES
		include/w32api/ddk
		include/ddk
		ddk)



