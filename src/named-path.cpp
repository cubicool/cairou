#include "cairocks.h"

#include <map>
#include <list>
#include <string>

typedef std::map<std::string, std::list<cairo_path_t*>> cairocks_named_path_t;

static cairo_user_data_key_t PATH_DATA;

static cairocks_named_path_t* cairocks_named_path_private_get(cairo_t* cr) {
	return static_cast<cairocks_named_path_t*>(cairo_get_user_data(cr, &PATH_DATA));
}

static void cairocks_named_path_private_destroy(void* data) {
	auto named_path = static_cast<cairocks_named_path_t*>(data);

	for(const auto& keys_values : *named_path) {
		for(auto path : keys_values.second) {
			cairo_path_destroy(path);
		}
	}

	delete static_cast<cairocks_named_path_t*>(data);
}

extern "C" {

void cairocks_append_named_path(cairo_t* cr, const char* named_path) {
	auto data = cairocks_named_path_private_get(cr);

	if(!data) {
		data = new cairocks_named_path_t();

		cairo_set_user_data(cr, &PATH_DATA, data, cairocks_named_path_private_destroy);
	}

	(*data)[named_path].push_back(cairo_copy_path(cr));

	cairo_new_path(cr);
}

cairo_bool_t cairocks_set_named_path(cairo_t* cr, const char* named_path) {
	auto data = cairocks_named_path_private_get(cr);

	if(!data) return FALSE;
	
	const auto key_value = data->find(named_path);
	
	if(key_value == data->end()) return FALSE;

	cairo_new_path(cr);

	for(const auto p : key_value->second) cairo_append_path(cr, p);

	return TRUE;
}

void cairocks_named_path_destroy(cairo_t* cr, const char* named_path) {
	auto data      = cairocks_named_path_private_get(cr);
	auto key_value = data->find(named_path);

	if(key_value == data->end()) return;

	(*data).erase(key_value);
}

}

