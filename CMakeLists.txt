cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
project(concurra)

########## find local packages #########################################################################################
### Intel Threading Blocks
find_package(TBB REQUIRED)

########## add main library ############################################################################################
add_library(${PROJECT_NAME} SHARED src/Node.cpp src/Pusher.cpp src/PusherClass.cpp src/Processor.cpp src/ src/function_traits_helper.cpp src/Runner.cpp src/RunnerClass.cpp src/RunnerSynchronous.cpp src/RunnerSynchronousClass.cpp src/RunnerSynchronousPair.cpp src/StoppableThread.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC TBB::tbb)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

########## add tests ###################################################################################################
enable_testing()

add_executable(test_runner_synchronous_pair test/test_runner_synchronous_pair.cpp)
target_link_libraries(test_runner_synchronous_pair PUBLIC ${PROJECT_NAME})
target_compile_features(test_runner_synchronous_pair PUBLIC cxx_std_23)

add_executable(test_skip test/test_skip.cpp)
target_link_libraries(test_skip PUBLIC ${PROJECT_NAME})
target_compile_features(test_skip PUBLIC cxx_std_23)

# add_executable(test_skip_once test/test_skip_once.cpp)
# target_link_libraries(test_skip_once PUBLIC ${PROJECT_NAME})
# target_compile_features(test_skip_once PUBLIC cxx_std_23)

add_executable(test_stopping test/test_stopping.cpp)
target_link_libraries(test_stopping PUBLIC ${PROJECT_NAME})
target_compile_features(test_stopping PUBLIC cxx_std_23)

add_executable(test_class_variant test/test_class_variant.cpp)
target_link_libraries(test_class_variant PUBLIC ${PROJECT_NAME})
target_compile_features(test_class_variant PUBLIC cxx_std_23)

add_test(NAME ctest_runner_synchronous_pair COMMAND test_runner_synchronous_pair WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME test_skip COMMAND test_skip WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# add_test(NAME test_skip_once COMMAND test_skip_once WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME test_class_variant COMMAND test_class_variant WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
