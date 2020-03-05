include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${SGX_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(${ENCLAVE_NAME}_u)
add_custom_command(
    COMMAND ${SGX_EDGER8R} --untrusted ${EXAMPLE_EDL_FILE}
    --search-path ${SGX_INCLUDE_DIR}
    OUTPUT ${ENCLAVE_NAME}_u.c ${ENCLAVE_NAME}_u.h
)

add_executable(app_compress_decompress ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp ${ENCLAVE_NAME}_u.c)
set_target_properties(app_compress_decompress PROPERTIES COMPILE_FLAGS ${APP_CXX_FLAGS})

target_link_libraries(app_compress_decompress "${SGX_COMMON_CFLAGS} \
                                         -L${SGX_LIBRARY_PATH} \
                                         -l${SGX_URTS_LIB} \
                                         -l${SGX_USVC_LIB} \
                                         -lpthread \
                                         -lsgx_ukey_exchange")

install(TARGETS app_compress_decompress
        DESTINATION example/compress_decompress)
