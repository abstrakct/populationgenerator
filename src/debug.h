/*
 * Debugging
 *
 * Copyright (C) 2013
 *
 */

#pragma once

#include <stdio.h>

// the following was stolen from http://stackoverflow.com/questions/4248372/custom-debug-printf-used-at-runtime
#ifdef DEBUG_ON
#define dbg(fmt, ...) printf(("  DEBUG: [%s:%d] %s: " fmt "\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#else
#define dbg(...)
#endif

// vim: fdm=syntax
