#include "cairocks.h"

#include <map>
#include <list>
#include <string>

typedef std::list<cairo_path_t*>                          cairocks_named_path_list_t;
typedef std::map<std::string, cairocks_named_path_list_t> cairocks_named_path_t;

static cairo_user_data_key_t PATH_DATA;

static cairocks_named_path_t* cairocks_named_path_private_get(cairo_t* cr) {
	return static_cast<cairocks_named_path_t*>(cairo_get_user_data(cr, &PATH_DATA));
}

static void cairocks_named_path_private_destroy(void* data) {
	cairocks_named_path_t* named_path = static_cast<cairocks_named_path_t*>(data);

	for(cairocks_named_path_t::iterator i = named_path->begin(); i != named_path->end(); i++) {
		for(cairocks_named_path_list_t::iterator j = i->second.begin(); j != i->second.end(); j++) {
			cairo_path_destroy(*j);
		}
	}

	delete static_cast<cairocks_named_path_t*>(data);
}

void cairocks_named_path_private_append(cairo_t* cr, const char* named_path, bool new_path=true) {
	cairocks_named_path_t* data = cairocks_named_path_private_get(cr);

	if(!data) {
		data = new cairocks_named_path_t();

		cairo_set_user_data(cr, &PATH_DATA, data, cairocks_named_path_private_destroy);
	}

	(*data)[named_path].push_back(cairo_copy_path(cr));

	if(new_path) cairo_new_path(cr);
}

void cairocks_append_named_path(cairo_t* cr, const char* named_path) {
	cairocks_named_path_private_append(cr, named_path);
}

void cairocks_append_named_path_preserve(cairo_t* cr, const char* named_path) {
	cairocks_named_path_private_append(cr, named_path, false);
}

cairo_bool_t cairocks_set_named_path(cairo_t* cr, const char* named_path) {
	cairocks_named_path_t* data = cairocks_named_path_private_get(cr);

	if(!data) return FALSE;

	cairocks_named_path_t::const_iterator key_value = data->find(named_path);

	if(key_value == data->end()) return FALSE;

	cairo_new_path(cr);

	for(
		cairocks_named_path_list_t::const_iterator i = key_value->second.begin();
		i != key_value->second.end();
		i++
	) cairo_append_path(cr, *i);

	return TRUE;
}

void cairocks_remove_named_path(cairo_t* cr, const char* named_path) {
	cairocks_named_path_t*          data      = cairocks_named_path_private_get(cr);
	cairocks_named_path_t::iterator key_value = data->find(named_path);

	if(key_value == data->end()) return;

	for(
		cairocks_named_path_list_t::const_iterator i = key_value->second.begin();
		i != key_value->second.end();
		i++
	) cairo_path_destroy(*i);

	(*data).erase(key_value);
}

