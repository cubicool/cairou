#include "cairou-private.h"

static cairo_user_data_key_t CAIROU_STATUS;

void cairou_set_status(cairo_t* cr, cairou_status_t status) {
	cairo_set_user_data(cr, &CAIROU_STATUS, (void*)(status), NULL);
}

cairou_status_t cairou_status(cairo_t* cr) {
	return (cairou_status_t)(cairo_get_user_data(cr, &CAIROU_STATUS));
}

