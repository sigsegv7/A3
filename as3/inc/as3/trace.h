/*
 * Copyright (c) 2026, Ian Moffett.
 * Provided under the BSD-3 clause.
 */

#ifndef AS3_TRACE_H
#define AS3_TRACE_H 1

#include <stdio.h>

#define trace_warn(fmt, ...) \
    printf("[warn]: " fmt, ##__VA_ARGS__)
#define trace_error(state, fmt, ...) \
    printf("[error]: " fmt, ##__VA_ARGS__)

#endif  /* !AS3_TRACE_H */
