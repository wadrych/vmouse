set(HEADER_LIST "${vmouse_client_SOURCE_DIR}/include/vmouse_client/Mouse.hpp")

add_executable(${PROJECT_NAME} Client.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../include )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_sources(${PROJECT_NAME} PUBLIC
                ${SOURCE_DIR}/Device.cpp
                ${SOURCE_DIR}/Mouse.cpp
                ${SOURCE_DIR}/Registry.cpp)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
