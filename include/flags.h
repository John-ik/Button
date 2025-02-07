#pragma once

#include "stdint.h"


typedef uint16_t Flags;

#define flag_mask(flags, x) (flags & x)

// inline Flags flag_mask(Flags* flags, const Flags* x) __attribute__((always_inline)) {
//     return *flags & *x;
// }

#define flag_set(flags, x) (flags |= x) 
// inline void flag_set(Flags* flags, const Flags* x) __attribute__((always_inline)) {
//     *flags |= *x;
// }

#define flag_clear(flags, x) (flags &= ~(x))
// inline void flag_clear(Flags* flags, const Flags* x) __attribute__((always_inline)) {
//     *flags &= ~(*x);
// }

#define flag_read(flags, x) (flags & x) 
// inline bool flag_read(Flags* flags, const Flags* x) __attribute__((always_inline)) {
//     return *flags & *x;
// }

#define flag_write(flags, x, v) ( v ? flag_set(flags, x) : flag_clear(flags, x) )
// inline void flag_write(Flags* flags, const Flags* x, const bool v) __attribute__((always_inline)) {
//     if (v) flag_set(flags, x);
//     else flag_clear(flags, x);
// }

#define flag_eq(flags, x, y) ((flags & x) == y)
// inline bool flag_eq(Flags* flags, const Flags* x, const Flags* y) __attribute__((always_inline)) {
//     return (*flags & *x) == *y;
// }

