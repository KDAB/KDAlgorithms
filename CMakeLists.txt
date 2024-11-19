cmake_minimum_required(VERSION 3.10)
project(kdalgorithms
    LANGUAGES CXX
)

if(POLICY CMP0077) # Yes, let the parent project set BUILD_TESTING before we include CTest.cmake below
  cmake_policy(SET CMP0077 NEW)
endif()

add_library(kdalgorithms INTERFACE)
add_library(KDAB::KDAlgorithms ALIAS kdalgorithms)
target_include_directories(kdalgorithms INTERFACE src/)

if(MSVC)
    target_compile_options(kdalgorithms INTERFACE /Zc:__cplusplus)
endif()

include(CTest)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(MAIN_PROJECT ON)
endif()

option(KDALGORITHMS_BUILD_TEST "Build the kdalgorithms unit tests when BUILD_TESTING is enabled." ${MAIN_PROJECT})

if(BUILD_TESTING AND ${KDALGORITHMS_BUILD_TEST})

    # This library support C++14 and above.
    if (NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 20)
    endif()

    add_compile_options(-Wall)

    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wno-deprecated -Wextra -Woverloaded-virtual -Winit-self -Wmissing-include-dirs -Wunused -Wno-div-by-zero -Werror=undef -Wpointer-arith -Wmissing-noreturn -Werror=return-type")
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-parameter")
    endif()

    if(MSVC) # Check if we are using the Visual Studio compiler
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus") # Make __cplusplus match the actual C++ version used
    endif()

    # Needed for the unit tests
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Core Test)
    set(CMAKE_AUTOMOC TRUE)

    enable_testing()
    add_executable(tst_kdalgorithms
        src/kdalgorithms.h

        src/kdalgorithms_bits/read_iterator_wrapper.h
        src/kdalgorithms_bits/find_if.h
        src/kdalgorithms_bits/filter.h
        src/kdalgorithms_bits/generate.h
        src/kdalgorithms_bits/insert_wrapper.h
        src/kdalgorithms_bits/is_const_method.h
        src/kdalgorithms_bits/is_detected.h
        src/kdalgorithms_bits/method_tests.h
        src/kdalgorithms_bits/operators.h
        src/kdalgorithms_bits/reserve_helper.h
        src/kdalgorithms_bits/return_type_trait.h
        src/kdalgorithms_bits/shared.h
        src/kdalgorithms_bits/to_function_object.h
        src/kdalgorithms_bits/transform.h
        src/kdalgorithms_bits/zip.h
        src/kdalgorithms_bits/tuple_utils.h
        src/kdalgorithms_bits/invoke.h
        src/kdalgorithms_bits/cartesian_product.h

        tests/tst_kdalgorithms.cpp
        tests/tst_constraints.cpp
        tests/copy_observer.h
        tests/copy_observer.cpp
    )

    add_test(NAME tst_kdalgorithms COMMAND tst_kdalgorithms)
    target_link_libraries(tst_kdalgorithms Qt${QT_VERSION_MAJOR}::Test)

    add_executable(tst_return_type_traits tests/tst_return_type_traits.cpp)

    # Make it show up in Qt Creator
    add_custom_target(additional_files SOURCES
        README.md
        run
        Documentation/algorithms.md
        Documentation/deploying.md
        Documentation/inspiration.md
        Documentation/ChangeLog.md
    )

    add_subdirectory(Inspiration)
endif()
