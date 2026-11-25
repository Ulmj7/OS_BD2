#ifndef KSHIM_ERRNO_H
#define KSHIM_ERRNO_H
/*
 * User-space shim for <linux/errno.h>. glibc's <bits/errno.h> itself includes <linux/errno.h> to get the E*
 * numbers, and -Isrc/shim makes this file shadow it, so forward to the system header with #include_next
 * before pulling in the C library's <errno.h>.
 */
#include_next <linux/errno.h>
#include <errno.h>
#endif
