#Compiler
CPP = g++

CFLAGS = -std=c++17

#Files
FILE_NAME = compression_test.cpp
#Output object
OBJ_NAME = compression_test

#Zstandart libs folder for include
ZSTD_LIB = -isystem zstd
#Zstandart files
ZSTD_FILES = zstd/decompress/huf_decompress_amd64.S
ZSTD_FILES += zstd/decompress/huf_decompress.c
ZSTD_FILES += zstd/decompress/zstd_decompress_block.c
ZSTD_FILES += zstd/decompress/zstd_ddict.c
ZSTD_FILES += zstd/decompress/zstd_decompress.c
ZSTD_FILES += zstd/compress/zstd_fast.c
ZSTD_FILES += zstd/compress/zstd_compress_superblock.c
ZSTD_FILES += zstd/compress/zstd_compress_sequences.c
ZSTD_FILES += zstd/compress/zstd_ldm.c
ZSTD_FILES += zstd/compress/huf_compress.c
ZSTD_FILES += zstd/compress/zstd_lazy.c
ZSTD_FILES += zstd/compress/hist.c
ZSTD_FILES += zstd/compress/zstdmt_compress.c
ZSTD_FILES += zstd/compress/zstd_double_fast.c
ZSTD_FILES += zstd/compress/zstd_compress.c
ZSTD_FILES += zstd/compress/zstd_opt.c
ZSTD_FILES += zstd/compress/zstd_compress_literals.c
ZSTD_FILES += zstd/compress/fse_compress.c
ZSTD_FILES += zstd/dictBuilder/fastcover.c
ZSTD_FILES += zstd/dictBuilder/cover.c
ZSTD_FILES += zstd/dictBuilder/zdict.c
ZSTD_FILES += zstd/dictBuilder/divsufsort.c
ZSTD_FILES += zstd/deprecated/zbuff_decompress.c
ZSTD_FILES += zstd/deprecated/zbuff_common.c
ZSTD_FILES += zstd/deprecated/zbuff_compress.c
ZSTD_FILES += zstd/common/error_private.c
ZSTD_FILES += zstd/common/xxhash.c
ZSTD_FILES += zstd/common/zstd_common.c
ZSTD_FILES += zstd/common/pool.c
ZSTD_FILES += zstd/common/debug.c
ZSTD_FILES += zstd/common/fse_decompress.c
ZSTD_FILES += zstd/common/threading.c
ZSTD_FILES += zstd/common/entropy_common.c

#LZ4 libs folder for include
LZ4_LIB = -isystem lz4
#LZ4 files
LZ4_FILES = lz4/lz4file.c
LZ4_FILES += lz4/xxhash.c
LZ4_FILES += lz4/lz4hc.c
LZ4_FILES += lz4/lz4frame.c
LZ4_FILES += lz4/lz4.c

#Libs union
LIBS = $(ZSTD_LIB) $(LZ4_LIB)

make: $(FILE_NAME)
	$(CPP) $(CFLAGS) $(LIBS) $(LZ4_FILES) $(ZSTD_FILES) $(FILE_NAME) -o $(OBJ_NAME)

clean: clean_build
	rm $(OBJ_NAME)