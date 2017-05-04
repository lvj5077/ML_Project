
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src )
LINK_LIBRARIES(${PROJECT_NAME})
IF(OPENCV_VERSION_3)


find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(/home/jin/Desktop/ML_Project/utils)
add_library( myImage SHARED myImage.cpp )

add_executable(demo demo.cpp)
target_link_libraries( demo myImage ${Boost_LIBRARIES} )

# add_executable(testF testF.cpp)
# target_link_libraries( testF myImage ${Boost_LIBRARIES} )

# ADD_EXECUTABLE(3_test_demo_general 3_test_demo_general.cpp)
# ADD_EXECUTABLE(2_test_demo_general 2_test_demo_general.cpp)
# ADD_EXECUTABLE(1_train_demo_general 1_train_demo_general.cpp)
# ADD_EXECUTABLE(demo_general demo_general.cpp)
# ADD_EXECUTABLE(create_voc_step0 create_voc_step0.cpp)
# ADD_EXECUTABLE(create_voc_step1 create_voc_step1.cpp)
# INSTALL(TARGETS  demo_general  create_voc_step0  create_voc_step1  RUNTIME DESTINATION bin)
ENDIF()
