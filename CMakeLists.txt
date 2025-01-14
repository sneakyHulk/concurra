cmake_minimum_required(VERSION 3.28)

project(concurra)

########## find local packages #########################################################################################
### Intel Threading Blocks
find_package(TBB REQUIRED)

add_library(${PROJECT_NAME} SHARED src/Node.cpp src/Pusher.cpp src/Processor.cpp src/ src/Runner.cpp src/RunnerSynchronous.cpp src/RunnerSynchronousPair.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC TBB::tbb)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

add_executable(test_runner_synchronous_pair test/test_runner_synchronous_pair.cpp)
target_link_libraries(test_runner_synchronous_pair PUBLIC ${PROJECT_NAME})
target_compile_features(test_runner_synchronous_pair PUBLIC cxx_std_23)