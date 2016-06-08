#ifndef CAIROU_PRIVATE_H
#define CAIROU_PRIVATE_H

#include "cairou.h"

#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)) && \
	(defined(__ELF__) || defined(__APPLE__)) && \
	!defined(__sun)
#define cairo_private __attribute__((__visibility__("hidden")))
#else
#define cairo_private
#endif

cairo_private
void cairou_set_status(cairo_t* cr, cairou_status_t status);

#endif

