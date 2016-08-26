// 2016, Georg Sauthoff <mail@georg.so>, CDDL and WTFPL
#include "wcsdup.h"

#ifndef HAVE_WCSDUP

  #include <stdlib.h>

  // this function is part of POSIX.2008
  wchar_t * wcsdup(const wchar_t *s)
  {
    size_t   n = wcslen(s) + 1;
    wchar_t *r = malloc(n * sizeof(wchar_t));
    return   r ? wmemcpy(r, s, n) : 0;
}

#endif
