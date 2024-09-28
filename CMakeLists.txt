cmake_minimum_required(VERSION 3.10)
project(BlockchainApp)

set(CMAKE_CXX_STANDARD 17)

# Include Crow
include_directories(${CMAKE_SOURCE_DIR}/lib/Crow)

add_executable(blockchain_app core/api/routes.h)
