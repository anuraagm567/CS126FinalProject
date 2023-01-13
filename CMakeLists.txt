cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(final-project)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wpedantic -Werror)
endif()

include(FetchContent)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.11.1
)

FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_library(catch2 INTERFACE)
    target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
endif()

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.9.1
)

# Code below derived from: https://campuswire.com/c/G161768D4/feed/2240
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_library(json_lib INTERFACE)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR})
endif()

target_link_libraries(json_lib INTERFACE nlohmann_json::nlohmann_json)

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

list(APPEND CORE_SOURCE_FILES src/core/wish_item.cc
                              src/core/wish_banner.cc)

list(APPEND SOURCE_FILES    ${CORE_SOURCE_FILES}
                            src/visualizer/wish_simulator_app.cc
                            src/visualizer/button.cc)

list(APPEND TEST_FILES tests/wish_banner_test.cc)

add_executable(console-simulator apps/console_simulator_main.cc ${CORE_SOURCE_FILES})
target_link_libraries(console-simulator json_lib)
target_include_directories(console-simulator PRIVATE include)

ci_make_app(
        APP_NAME        wish-simulator
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         apps/cinder_simulator_main.cc ${SOURCE_FILES}
        INCLUDES        include
        LIBRARIES       catch2 json_lib
)

ci_make_app(
        APP_NAME        wish-simulator-test
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         tests/test_main.cc ${SOURCE_FILES} ${TEST_FILES}
        INCLUDES        include
        LIBRARIES       catch2
                        catch2 json_lib
)

if(MSVC)
    set_property(TARGET wish-simulator-test APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:CONSOLE")
endif()