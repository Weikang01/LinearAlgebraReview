# Include the main project's include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Locate all test files
file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(_test IN ITEMS ${TEST_FILES})
    get_filename_component(_test_name ${_test} NAME_WE)
    add_executable(${_test_name} ${_test})

    # Add test command
    add_test(NAME ${_test_name} COMMAND $<TARGET_FILE:${_test_name}>)
endforeach()