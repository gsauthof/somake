// 2016, Georg Sauthoff <mail@georg.so>, CDDL
#ifndef COMP_TIMESTRUC_T_H
#define COMP_TIMESTRUC_T_H

#if defined(sun) || defined(__sun)
#else
typedef struct timespec timestruc_t;
#endif

#endif
