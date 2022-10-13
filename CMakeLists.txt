cmake_minimum_required(VERSION 3.0.0)
project(scanUtil VERSION 0.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(scanUtil)

set_target_properties(scanUtil PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON

  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release

  AUTOMOC ON
)

set(SRC
  src/main.cpp
  src/AnalyzeTask.cpp
  src/Scanner.cpp
  src/TipParser.cpp
)

if (WIN32)
    set(SRC ${SRC} src/WMappedFile.cpp)

elseif(UNIX)
    set(SRC ${SRC} src/LMappedFile.cpp)

else()
    message( FATAL_ERROR "Only Windows/Unix OS supported" )
endif()

target_include_directories(scanUtil
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(scanUtil PRIVATE ${SRC})

if (CMAKE_BUILD_TYPE MATCHES "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    FILE(COPY resourses/groups.tip DESTINATION "${CMAKE_BINARY_DIR}/bin/debug")
    FILE(COPY resourses/rules.tip DESTINATION "${CMAKE_BINARY_DIR}/bin/debug")

elseif(CMAKE_BUILD_TYPE MATCHES Release)
    FILE(COPY resourses/groups.tip DESTINATION "${CMAKE_BINARY_DIR}/bin/release")
    FILE(COPY resourses/rules.tip DESTINATION "${CMAKE_BINARY_DIR}/bin/release")
    set( CMAKE_CXX_FLAGS "-O2")

endif()
