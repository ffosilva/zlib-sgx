**ZLIB-SGX**

*Data Compression Library inside SGX Enclaves*

This library preserves all the features of zlib, except gz* functions due to Intel(r) SGX limitations (fread, fwrite, ...).

**Building (with examples)**

```bash
git clone https://github.com/ffosilva/zlib-sgx.git --recursive && cd zlib-sgx
mkdir build && cd build
cmake .. -DCOMPILE_EXAMPLES=YES
make -j && make install
```

**License**

zlib-sgx is open source under Apache 2.0. See LICENSE for more details.
