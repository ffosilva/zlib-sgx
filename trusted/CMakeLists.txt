set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -nostdinc -fvisibility=hidden -fpie -fstack-protector")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -std=c++03 -nostdinc++")

set(ZLIB_SRCS
    ${ZLIB_SRC_PATH}/adler32.c
    ${ZLIB_SRC_PATH}/compress.c
    ${ZLIB_SRC_PATH}/crc32.c
    ${ZLIB_SRC_PATH}/deflate.c
    ${ZLIB_SRC_PATH}/inflate.c
    ${ZLIB_SRC_PATH}/infback.c
    ${ZLIB_SRC_PATH}/inftrees.c
    ${ZLIB_SRC_PATH}/inffast.c
    ${ZLIB_SRC_PATH}/trees.c
    ${ZLIB_SRC_PATH}/uncompr.c
    ${ZLIB_SRC_PATH}/zutil.c
)

set(ZLIB_PUBLIC_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zconf.h
    ${ZLIB_SRC_PATH}/zlib.h
)

add_library(${PROJECT_NAME}_t STATIC ${ZLIB_SRCS})
set_target_properties(${PROJECT_NAME}_t PROPERTIES OUTPUT_NAME z_sgx)

#add_definitions(-DZ_SOLO)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ZLIB_SRC_PATH}
    ${SGX_PATH}/include
    ${SGX_PATH}/include/tlibc
    ${SGX_PATH}/include/stlport
)

target_link_libraries(${PROJECT_NAME}_t "${SGX_COMMON_CFLAGS} \
    -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L${SGX_LIBRARY_PATH} \
    -Wl,--whole-archive -l${SGX_TRTS_LIB} -Wl,--no-whole-archive \
    -Wl,--start-group -lsgx_tstdc -lsgx_tstdcxx -lsgx_tcrypto -l${SGX_TSVC_LIB} -Wl,--end-group \
    -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
    -Wl,-pie,-eenclave_entry -Wl,--export-dynamic \
    -Wl,--defsym,__ImageBase=0")

install(FILES ${ZLIB_PUBLIC_HDRS}
    DESTINATION include
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

install(TARGETS ${PROJECT_NAME}_t
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)