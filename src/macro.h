#define __PRINTF printf

#define DBG_PRINT_INT(X,Y) \
  __PRINTF(X,Y);

#define DBG_PRINT_FLOAT(X) \
  __PRINTF("%14.6e\n", X);

#define DBG_PRINT_STRING(X) \
  __PRINTF(X);

#define DBG_AVX_PRINT(X) \
  _mm256_store_ps(dbg, X);						\
  __PRINTF("%14.6e %14.6e %14.6e %14.6e %14.6e %14.6e %14.6e %14.6e\n", dbg[0], dbg[1], dbg[2], dbg[3], dbg[4], dbg[5], dbg[6], dbg[7]);

#define DBG_AVX_PRINT_INT(X) \
  _mm256_store_si256((__m256i *)dbgint, X);  \
  __PRINTF("%d %d %d %d %d %d %d %d\n", dbgint[0], dbgint[1], dbgint[2], dbgint[3], dbgint[4], dbgint[5], dbgint[6], dbgint[7]);

#define DBG_AVX_PRINT_BIT(X) \
  _mm256_store_ps((float *)dbgint, X);						\
  __PRINTF("      %08x       %08x       %08x       %08x       %08x       %08x       %08x       %08x\n", dbgint[0], dbgint[1], dbgint[2], dbgint[3], dbgint[4], dbgint[5], dbgint[6], dbgint[7]);

#define DBG_AVX512_PRINT(X) \
  _mm512_store_ps(dbg, X);						\
  __PRINTF("%14.6e %14.6e %14.6e %14.6e %14.6e %14.6e %14.6e %14.6e\n", dbg[0], dbg[1], dbg[2], dbg[3], dbg[4], dbg[5], dbg[6], dbg[7]); \
  __PRINTF("%14.6e %14.6e %14.6e %14.6e %14.6e %14.6e %14.6e %14.6e\n", dbg[8], dbg[9], dbg[10], dbg[11], dbg[12], dbg[13], dbg[14], dbg[15]);

#undef PRINTF
