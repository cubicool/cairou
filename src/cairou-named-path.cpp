#include "cairou.h"

#include <map>
#include <list>
#include <string>
#include <cstring>

struct cairou_named_path_item_t {
	cairou_named_path_item_t(cairo_path_t* _path):
	path(_path) {
	}

	cairo_path_t* path;
	cairo_matrix_t matrix;
};

typedef std::list<cairou_named_path_item_t> cairou_named_path_items_t;
typedef std::map<std::string, cairou_named_path_items_t> cairou_named_path_t;

struct cairou_named_path_private_t {
	cairou_named_path_private_t():
	last(0) {
	}

	cairou_named_path_t paths;
	cairou_named_path_items_t* last;
};

static cairo_user_data_key_t NAMED_PATH_DATA;

static cairou_named_path_private_t* cairou_named_path_private_get(cairo_t* cr) {
	return static_cast<cairou_named_path_private_t*>(cairo_get_user_data(cr, &NAMED_PATH_DATA));
}

static void cairou_named_path_private_destroy(void* data) {
	cairou_named_path_t* named_path = static_cast<cairou_named_path_t*>(data);

	for(cairou_named_path_t::iterator i = named_path->begin(); i != named_path->end(); i++) {
		for(cairou_named_path_items_t::iterator j = i->second.begin(); j != i->second.end(); j++) {
			cairo_path_destroy(j->path);
		}
	}

	delete named_path;
}

static cairo_bool_t cairou_named_path_private_append(
	cairo_t* cr,
	const char* named_path,
	bool new_path=true
) {
	cairou_named_path_items_t* list = 0;
	cairou_named_path_private_t* data = cairou_named_path_private_get(cr);

	if(!data) {
		data = new cairou_named_path_private_t();

		cairo_set_user_data(cr, &NAMED_PATH_DATA, data, cairou_named_path_private_destroy);
	}

	// If the name isn't specified, we're appending to the most recent (if it exists).
	if(!named_path || !std::strlen(named_path)) {
		if(!data->last) return FALSE;

		list = data->last;
	}

	else list = &data->paths[named_path];

	list->push_back(cairo_copy_path(cr));

	cairo_get_matrix(cr, &list->back().matrix);

	if(new_path) cairo_new_path(cr);

	// Set the last member for usage with subsequent NULL/implicit named path setting.
	data->last = list;

	return TRUE;
}

static cairo_bool_t cairou_named_path_private_set(
	cairo_t* cr,
	const char* named_path,
	bool new_path=true
) {
	const cairou_named_path_items_t* list = 0;
	cairou_named_path_private_t* data = cairou_named_path_private_get(cr);

	if(!data) return FALSE;

	if(!named_path || !std::strlen(named_path)) {
		if(!data->last) return FALSE;

		list = data->last;
	}

	else {
		cairou_named_path_t::const_iterator key_value = data->paths.find(named_path);

		if(key_value == data->paths.end()) return FALSE;

		list = &key_value->second;
	}

	if(new_path) cairo_new_path(cr);

	for(
		cairou_named_path_items_t::const_iterator i = list->begin();
		i != list->end();
		i++
	) {
		cairo_matrix_t matrix;

		cairo_get_matrix(cr, &matrix);
		cairo_matrix_multiply(&matrix, &i->matrix, &matrix);
		cairo_set_matrix(cr, &matrix);
		cairo_append_path(cr, i->path);
	}

	return TRUE;
}

extern "C" {

cairo_bool_t cairou_append_named_path(cairo_t* cr, const char* named_path) {
	return cairou_named_path_private_append(cr, named_path);
}

cairo_bool_t cairou_append_named_path_preserve(cairo_t* cr, const char* named_path) {
	return cairou_named_path_private_append(cr, named_path, false);
}

cairo_bool_t cairou_set_named_path(cairo_t* cr, const char* named_path) {
	return cairou_named_path_private_set(cr, named_path);
}

cairo_bool_t cairou_set_named_path_preserve(cairo_t* cr, const char* named_path) {
	return cairou_named_path_private_set(cr, named_path, true);
}

cairo_bool_t cairou_remove_named_path(cairo_t* cr, const char* named_path) {
	cairou_named_path_private_t* data = cairou_named_path_private_get(cr);
	cairou_named_path_t::iterator key_value = data->paths.find(named_path);

	if(key_value == data->paths.end()) return FALSE;

	for(
		cairou_named_path_items_t::const_iterator i = key_value->second.begin();
		i != key_value->second.end();
		i++
	) cairo_path_destroy(i->path);

	data->paths.erase(key_value);

	return TRUE;
}

}

