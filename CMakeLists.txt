cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
project(allocator CXX)


set(SOURCES
"allocator.cpp"
"main.cpp"
)

set(HEADERS
    "allocator.hpp"
)

add_library(allocator_lib ${SOURCES} ${HEADERS})

# target_include_directories(allocator_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(allocator_lib
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
$<INSTALL_INTERFACE:include>
)

# prepare:
# git submodule update --init --> skrypt ktory sprawdza czy Catch2 do pusty folder, jesli tak to wykonaj init
# ./install_env --> sha pliku install_env zapisue go do .installed. --> skrypt ktory porownuje sha z tym co jest w installed, jesli NEQ uruchmo install_env

add_custom_target(prepare
    COMMAND chmod +x prepare.sh ./prepare.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running Prepare"
    VERBATIM
)
add_executable(allocator main.cpp)
add_dependencies(allocator prepare)
target_link_libraries(allocator PRIVATE allocator_lib)

add_subdirectory(Catch2)

set(T_SOURCES
"allocator.cpp"
"test.cpp"
)

set(T_HEADERS
"allocator.hpp"
)

add_library(tests_lib ${T_SOURCES} ${T_HEADERS})
target_include_directories(tests_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests_lib PRIVATE Catch2::Catch2WithMain)
add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE tests_lib)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests_lib PRIVATE Catch2::Catch2WithMain)



#Valgrind
#Run from build/ with: cmake --build . --target valgrind
find_program(VALGRIND_EXECUTABLE valgrind)
if (VALGRIND_EXECUTABLE)
    add_custom_target(valgrind
        COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --error-exitcode=1 $<TARGET_FILE:tests>
        DEPENDS tests
        COMMENT "Running Valgrind"
    )
endif()

#ScanBuild
#Run from build/ with: cmake --build . --target scan_build
add_custom_target(scan_build
    COMMAND scan-build --status-bugs --keep-cc --show-description ninja
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running ScanBuild"
    VERBATIM
)
#Xanalyzer
#Run from build/ with: cmake --build . --target xanalyzer
#Or with ninja -Cbuild xanalyzer
add_custom_target(xanalyzer
    COMMAND clang --analyze -Xanalyzer -analyzer-output=text ${CMAKE_CURRENT_SOURCE_DIR}/allocator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Runnig Xanalyzer"
    VERBATIM
)

#Clang-Tidy
#Run with: ninja -Cbuild run-clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

set_target_properties(allocator PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
set_target_properties(tests PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

add_custom_target(run-clang-tidy
    COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running Clang-Tidy"
    SOURCES ${SOURCES} ${HEADERS} ${T_SOURCES} ${T_HEADERS}
)

#Clang-Sanitizer
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")

#All analyses and tests
add_custom_target(regression
    COMMAND ${CMAKE_COMMAND} --build . --target valgrind  # Run Valgrind
    COMMAND ${CMAKE_COMMAND} --build . --target xanalyzer  # Run Xanalyzer
    COMMAND ${CMAKE_COMMAND} --build . --target scan_build  # Run ScanBuild
    COMMAND ${CMAKE_COMMAND} --build . --target run-clang-tidy  # Run Clang-Tidy
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Regression (Tests and Analyses)"
)


# Target 'install' do instalacji biblioteki
install(TARGETS allocator_lib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

# Install the executable
install(TARGETS allocator
        DESTINATION bin)
