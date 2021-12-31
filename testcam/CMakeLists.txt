project(v4l2testcam)

include_directories(${PROJECT_SOURCE_DIR}/../../TUIIDC/testcam)
file(GLOB sources *.cc)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} TUvV4L2)
