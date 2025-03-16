cmake_minimum_required(VERSION 3.10)
project(KoAi)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include submodules
add_subdirectory(cpp-driver)

set_target_properties(cassandra PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

add_subdirectory(gui/Crow)

# Find all source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/app/*.cpp"
    "${CMAKE_SOURCE_DIR}/controllers/*.cpp"
    "${CMAKE_SOURCE_DIR}/database/*.cpp"
    "${CMAKE_SOURCE_DIR}/gui/*.cpp"
    "${CMAKE_SOURCE_DIR}/loggers/*.cpp"
    "${CMAKE_SOURCE_DIR}/managers/*.cpp"
    "${CMAKE_SOURCE_DIR}/predicter/*.cpp"
    "${CMAKE_SOURCE_DIR}/requests/*.cpp"
    "${CMAKE_SOURCE_DIR}/scraper/*.cpp"
    "${CMAKE_SOURCE_DIR}/utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/utils/disk/*.cpp"
    "${CMAKE_SOURCE_DIR}/utils/query_builder/*.cpp"
    "${CMAKE_SOURCE_DIR}/utils/string_manipulator/*.cpp"
    "${CMAKE_SOURCE_DIR}/main.cpp"
)

# Find required libraries
find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

# Define the executable
add_executable(koai ${SOURCES})

# Set output directories
set_target_properties(koai PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Add include directories for target
target_include_directories(koai PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/cpp-driver/include
    ${CMAKE_SOURCE_DIR}/gui/Crow/include
)

# Link necessary libraries
target_link_libraries(koai
    CURL::libcurl
    LibXml2::LibXml2
    cassandra
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)