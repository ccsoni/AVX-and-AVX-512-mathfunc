CC=gcc

avx2_available = yes
avx512_available = yes

CFLAGS=-O3 -ffast-math -funroll-loops -std=c11
ifeq ($(avx2_available), yes)
CFLAGS+=-mavx2 -mfma 
endif
ifeq ($(avx512_available), yes)
CFLAGS+=-mavx512f -mavx512dq -mavx512cd -mavx512bw -mavx512vl
endif

AVX_MATHFUNC_TEST_OBJ = avx_mathfunc_test.o timing.o 
AVX512_MATHFUNC_TEST_OBJ = avx512_mathfunc_test.o timing.o

AVX_MATHFUNC_TEST_DEP := $(AVX_MATHFUNC_TEST_OBJ) 
AVX512_MATHFUNC_TEST_DEP := $(AVX512_MATHFUNC_TEST_OBJ) 

ifeq ($(avx2_available), yes)
TARGET += avx_mathfunc_test
endif
ifeq ($(avx512_available), yes)
TARGET += avx512_mathfunc_test
endif

all: $(TARGET)

avx_mathfunc_test: $(AVX_MATHFUNC_TEST_DEP)
	$(CC) -o $@ $(AVX_MATHFUNC_TEST_OBJ) -lm

avx512_mathfunc_test: $(AVX512_MATHFUNC_TEST_DEP)
	$(CC) -o $@ $(AVX512_MATHFUNC_TEST_OBJ) -lm

clean:
	-rm -rf *.o

distclean: clean
	-rm -rf $(TARGET)
	-rm -rf *~
