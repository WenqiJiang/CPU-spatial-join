/* mem_align.h
 * Created on: Sep 5, 2012
 *
 * Darius Sidlauskas
 * Copyright (c) 2012 MADALGO, Aarhus University
 *
 * Provides aligned memory allocation for several OSs.
 */

#ifndef SRC_UTIL_MEM_ALIGN_H_
#define SRC_UTIL_MEM_ALIGN_H_

#if defined (__SVR4) && defined (__sun)
/* For Solaris */
#include <mtmalloc.h>
#endif

#include <cstdio>
#include <cstring>
#include <cstdlib>

#define CACHE_LINE_SIZE 64  // size in BYTES (not bits)

#if defined (__SVR4) && defined (__sun)
/* For Solaris */

static inline void* aligned_malloc(size_t size) {
  void *p = memalign(CACHE_LINE_SIZE, size);
  if ( p == NULL ) {
    printf("Posix Memalign Error:\n");
    exit(0);
  }
  return p;
}

static inline void aligned_free(void *mem_ptr) {
  free(mem_ptr);
}

static inline void* aligned_calloc(size_t size) {
  void *ret = aligned_malloc(size);
  if (ret != NULL) memset(ret, 0, size);
  return ret;
}

#else

/* For Linux */

static inline void* aligned_malloc( size_t size ) {
  void *p;

  int err = posix_memalign( &p, CACHE_LINE_SIZE, size );
  if ( err != 0 ) {
    printf( "Posix Memalign Error: %d\n", err );
    exit( 0 );
  }
  return p;
}

static inline void aligned_free( void *mem_ptr ) {
  free( mem_ptr );
}

static inline void* aligned_calloc( size_t size ) {
  void *ret = aligned_malloc( size );
  if ( ret != NULL )
    memset( ret, 0, size );
  return ret;
}

#endif  // defined (__SVR4) && defined (__sun)
#endif  // SRC_UTIL_MEM_ALIGN_H_
