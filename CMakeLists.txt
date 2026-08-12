cmake_minimum_required(VERSION 3.31)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    if(APPLE)
        project(yup-ehl-design-module VERSION 0.2.0 LANGUAGES C CXX OBJC OBJCXX)
    else()
        project(yup-ehl-design-module VERSION 0.2.0 LANGUAGES C CXX)
    endif()
    set(EHL_YUP_PLUGIN_UI_IS_TOP_LEVEL ON)
else()
    set(EHL_YUP_PLUGIN_UI_IS_TOP_LEVEL OFF)
endif()

option(EHL_YUP_PLUGIN_UI_BUILD_TESTS "Build the design contract tests" ${EHL_YUP_PLUGIN_UI_IS_TOP_LEVEL})
set(EHL_YUP_PLUGIN_UI_YUP_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../yup" CACHE PATH
    "Path to a local YUP checkout; fetched from upstream for top-level tests when unavailable")

if(NOT TARGET yup::yup_gui AND EHL_YUP_PLUGIN_UI_BUILD_TESTS)
    set(YUP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(YUP_BUILD_PLUGINS OFF CACHE BOOL "" FORCE)
    set(YUP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

    if(EXISTS "${EHL_YUP_PLUGIN_UI_YUP_SOURCE_DIR}/CMakeLists.txt")
        add_subdirectory("${EHL_YUP_PLUGIN_UI_YUP_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/yup")
    else()
        include(FetchContent)
        FetchContent_Declare(yup
            GIT_REPOSITORY https://github.com/kunitoki/yup.git
            GIT_TAG 9a1c9bc699b6a714f6f52486462d98a140c8bf95)
        FetchContent_MakeAvailable(yup)
    endif()
endif()

if(NOT TARGET yup::yup_gui)
    message(FATAL_ERROR "yup::yup_gui must exist before adding yup-ehl-design-module")
endif()

add_library(ehl_yup_plugin_ui INTERFACE)
add_library(ehl::yup_plugin_ui ALIAS ehl_yup_plugin_ui)
target_compile_features(ehl_yup_plugin_ui INTERFACE cxx_std_20)
target_link_libraries(ehl_yup_plugin_ui INTERFACE yup::yup_gui)
target_include_directories(ehl_yup_plugin_ui INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>")

if(EHL_YUP_PLUGIN_UI_BUILD_TESTS)
    include(FetchContent)
    _yup_fetch_sdl()
    include(CTest)
    add_executable(ehl_yup_plugin_ui_contract_tests
        tests/EhlPluginThemeContractTests.cpp)
    target_link_libraries(ehl_yup_plugin_ui_contract_tests PRIVATE ehl::yup_plugin_ui sdl::sdl)
    target_compile_features(ehl_yup_plugin_ui_contract_tests PRIVATE cxx_std_20)
    target_compile_definitions(ehl_yup_plugin_ui_contract_tests PRIVATE
        EHL_YUP_PLUGIN_UI_SOURCE_DIR="${CMAKE_CURRENT_LIST_DIR}")
    add_test(NAME ehl_yup_plugin_ui_contract_tests COMMAND ehl_yup_plugin_ui_contract_tests)
endif()
