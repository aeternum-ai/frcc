add_subdirectory(${FRCC_ROOT}/cmake/external/googletest EXCLUDE_FROM_ALL)

enable_testing()

set(FRCC_TEST_ROOT ${FRCC_ROOT}/test)

macro(AddTest TARGET SOURCES)
    add_executable(${TARGET} ${SOURCES} )
    target_link_libraries(${TARGET} GTest::gtest_main frcc protobuf absl_log_internal_message absl_log_internal_check_op)
    add_test(${TARGET} ${TARGET})
endmacro()

AddTest(
        frcc_compile_main_test
        ${FRCC_TEST_ROOT}/vanilla_test.cc
)

AddTest(
        frcc_vanilla_remove_identity_pass_test
        ${FRCC_TEST_ROOT}/vanilla_remove_identity_pass_test.cc
)