include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${SGX_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/config
                    ${ZLIB_SRC_PATH}
                    ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE ENCLAVE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB ENCLAVE_SIGN_PK ${CMAKE_CURRENT_SOURCE_DIR}/${ENCLAVE_NAME}_private.pem)
file(GLOB ENCLAVE_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/${ENCLAVE_NAME}.config.xml)

add_custom_target(${ENCLAVE_NAME}_t)
add_custom_command(
    COMMAND ${SGX_EDGER8R} --trusted ${EXAMPLE_EDL_FILE}
    --search-path ${SGX_INCLUDE_DIR}
    OUTPUT ${ENCLAVE_NAME}_t.c ${ENCLAVE_NAME}_t.h
)

link_directories(${CMAKE_BINARY_DIR}/trusted)
add_library(${ENCLAVE_NAME} SHARED ${ENCLAVE_SRC_FILES} ${ENCLAVE_NAME}_t.c)
add_dependencies(${ENCLAVE_NAME} zlib_sgx_t)

add_custom_command(
    TARGET ${ENCLAVE_NAME}
    POST_BUILD
    COMMAND ${SGX_ENCLAVE_SIGNER} sign -key ${ENCLAVE_SIGN_PK} -enclave $<TARGET_FILE:${ENCLAVE_NAME}> -out ${ENCLAVE_NAME}.signed.so -config ${ENCLAVE_CONFIG}
    BYPRODUCTS ${ENCLAVE_NAME}.signed.so
)

set(LDSCRIPT_FLAG "-Wl,--version-script=${EXAMPLE_LDS_FILE}")

set_target_properties(${ENCLAVE_NAME} PROPERTIES COMPILE_FLAGS ${ENCLAVE_C_FLAGS})
target_link_libraries(${ENCLAVE_NAME} "${SGX_COMMON_CFLAGS} \
            -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L${SGX_LIBRARY_PATH} \
            -Wl,--whole-archive -l${SGX_TRTS_LIB} -Wl,--no-whole-archive \
            -Wl,--whole-archive -lsgx_tcmalloc -Wl,--no-whole-archive \
            -Wl,--start-group -lz_sgx -lsgx_tstdc -lsgx_tcxx -lsgx_tkey_exchange -lsgx_tcrypto -l${SGX_TSVC_LIB} -Wl,--end-group \
            -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
            -Wl,-pie,-eenclave_entry -Wl,--export-dynamic \
            ${LDSCRIPT_FLAG} \
            -Wl,--defsym,__ImageBase=0")

set_target_properties(${ENCLAVE_NAME} PROPERTIES PREFIX "")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${ENCLAVE_NAME}.signed.so
        DESTINATION example/compress_decompress)
