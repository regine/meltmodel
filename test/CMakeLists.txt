# CMakeLists.txt for test/

# Copy json_to_degree.py into the build directory:
configure_file (${PROJECT_SOURCE_DIR}/util/json_input/json_to_degree.py ${PROJECT_BINARY_DIR}/json_to_degree.py COPYONLY)

# Set the location of MeltMod regression test scripts
set(MELTMOD_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# A macro simplifying test creation:
macro(meltmod_test name script)
    add_test("${name}:${script}" ${MELTMOD_TEST_DIR}/${script} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
endmacro()

# Add the tests themselves:
meltmod_test (meltmod_testing_tools test_00.sh)
