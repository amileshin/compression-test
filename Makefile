#Compiler
CPP = g++

#Files
FILE_NAME = compression_test.cpp
#Output object
OBJ_NAME = compression_test

#Zstandart libs include
ZSTD_LIB = -lzstd
#LZ4 libs include
LZ4_LIB = lib/lz4.c lib/lz4hc.c -Ilib

all: $(FILE_NAME) measure.hpp
	$(CPP) $(FILE_NAME) $(ZSTD_LIB) $(LZ4_LIB) -o $(OBJ_NAME)