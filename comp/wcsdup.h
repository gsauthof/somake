// 2016, Georg Sauthoff <mail@georg.so>, CDDL
#ifndef COMPAT_WCSDUP_H
#define COMPAT_WCSDUP_H

#include <wchar.h>

#include "config.h"

#ifndef HAVE_WCSDUP
  #if defined(__cplusplus)
  extern "C" {
  #endif
    wchar_t * wcsdup (const wchar_t *s);
  #if defined(__cplusplus)
  }
  #endif
#endif

#endif
