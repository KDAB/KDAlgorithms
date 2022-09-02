cmake_minimum_required(VERSION 3.9)
project(algorithms)

# This library support C++14 and above.
if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

add_compile_options(-Wall)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wno-deprecated -Wextra -Woverloaded-virtual -Winit-self -Wmissing-include-dirs -Wunused -Wno-div-by-zero -Werror=undef -Wpointer-arith -Wmissing-noreturn -Werror=return-type -Wsuggest-override")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-parameter")
endif()

if(MSVC) # Check if we are using the Visual Studio compiler
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus") # Make __cplusplus match the actual C++ version used
endif()


find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Test)
set(CMAKE_AUTOMOC TRUE)

enable_testing()
add_executable(tst_KDAlgorithms
    src/KDAlgorithms.h

    src/bits/copy_or_move_iterators.h
    src/bits/filter.h
    src/bits/has_reserve_trait.h
    src/bits/return_type_trait.h
    src/bits/shared.h
    src/bits/transform.h

    tests/tst_KDAlgorithms.cpp
    tests/tst_constraints.cpp
    tests/CopyObserver.cpp
)

add_test(NAME tst_KDAlgorithms COMMAND tst_KDAlgorithms)
target_link_libraries(tst_KDAlgorithms Qt${QT_MAJOR_VERISION}::Test)

add_executable(tst_return_type_traits tests/tst_return_type_traits.cpp)

# Make it show up in Qt Creator
add_custom_target(additional_files SOURCES README.md run TODO Notes)
