#include "range_expansion.h"

#include <apr_pools.h>
#include <libcrange.h>

const gsize MAX_APR_ERR_MSG_SIZE = 1024;

static apr_pool_t* pool;

gint wsh_exp_range_init(GError** err) {
	g_assert_no_error(*err);

	apr_initialize();
	apr_status_t stat = apr_pool_create(&pool, NULL);

	WSH_EXP_ERROR = g_quark_from_static_string("wsh_exp_error");

	if (stat != APR_SUCCESS) {
		char* err_message = g_slice_alloc0(MAX_APR_ERR_MSG_SIZE);
		apr_strerror(stat, err_message, MAX_APR_ERR_MSG_SIZE);
		*err = g_error_new(WSH_EXP_ERROR, WSH_EXP_POOL_ALLOC_ERR,
			"%s", err_message);
		// I hope g_error_new makes a copy of that string!
		g_slice_free1(MAX_APR_ERR_MSG_SIZE, err_message);
		return WSH_EXP_POOL_ALLOC_ERR;
	}

	return 0;
}

void wsh_exp_range_cleanup(void) {
	apr_pool_clear(pool);
	apr_pool_destroy(pool);
	apr_terminate();
}

gint wsh_exp_range_expand(gchar*** host_list, const gchar* string, GError** err) {
	g_assert_no_error(*err);
	g_assert(*host_list == NULL);
	g_assert(string != NULL);

	apr_initialize();

	gint ret = 0;

	libcrange* lr = libcrange_new(pool, NULL);
	if (lr == NULL) {
		*err = g_error_new(WSH_EXP_ERROR, WSH_EXP_RANGE_ALLOC_ERR,
			"Could not allocate a libcrange object");
		ret = WSH_EXP_RANGE_ALLOC_ERR;
		goto wsh_exp_range_expand_err;
	}

	range_request* rr = range_expand(lr, pool, string);
	if (range_request_has_warnings(rr)) {
		*err = g_error_new(WSH_EXP_ERROR, WSH_EXP_RANGE_EXPANSION_ERR,
			"%s", range_request_warnings(rr));
		ret = WSH_EXP_RANGE_EXPANSION_ERR;
		goto wsh_exp_range_expand_err;
	}

	*host_list = g_strdupv((gchar**)range_request_nodes(rr));
	if (host_list == NULL) {
		*err = g_error_new(WSH_EXP_ERROR, WSH_EXP_NO_HOSTS_ERR,
			"%s expanded to 0 hosts", string);
		ret = WSH_EXP_NO_HOSTS_ERR;
	}

wsh_exp_range_expand_err:
	return ret;
}

