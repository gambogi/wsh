#include <glib.h>
#include <stdlib.h>
#include <string.h>

#ifdef RANGE
# include "range_expansion.h"
#endif
#include "ssh.h"

static gint threads = 0;
static gchar* username = NULL;
static gboolean force = FALSE;
static gint port = 22;
#ifdef RANGE
static gboolean range = FALSE;
#endif

static GOptionEntry entries[] = {
	{ "threads", 't', 0, G_OPTION_ARG_INT, &threads, "Number of threads to spawn off (default: none)", NULL },
	{ "username", 'u', 0, G_OPTION_ARG_STRING, &username, "Username to pass to ssh", NULL },
	{ "force", 'f', 0, G_OPTION_ARG_NONE, &force, "Force add keys, even if they've changed", NULL },
	{ "port", 'p', 0, G_OPTION_ARG_INT, &port, "Port to use, if not 22", NULL },
#ifdef RANGE
	{ "range", 'r', 0, G_OPTION_ARG_NONE, &range, "Use range for hostname expansion", NULL },
#endif
	{ NULL }
};

static gint add_hostkey(const gchar* hostname, gpointer userdata) {
	GError* err;

	g_assert(hostname);

	wsh_ssh_session_t* session = g_slice_new0(wsh_ssh_session_t);
	session->hostname = hostname;
	session->username = username;
	session->port = port;

	if (wsh_ssh_host(session, &err)) {
		g_printerr("Could not add ssh key: %s\n", err->message);
		g_error_free(err);
		g_slice_free(wsh_ssh_session_t, session);
		return EXIT_FAILURE;
	}

	if (wsh_verify_host_key(session, TRUE, force, &err)) {
		g_printerr("Could not add ssh key: %s\n", err->message);
		g_error_free(err);
		g_slice_free(wsh_ssh_session_t, session);
		return EXIT_FAILURE;
	}

	wsh_ssh_disconnect(session);

	g_slice_free(wsh_ssh_session_t, session);

	return EXIT_SUCCESS;
}

gint main(gint argc, gchar** argv) {
	GError* err = NULL;
	GOptionContext* context;
	gint ret = EXIT_SUCCESS;
#if GLIB_CHECK_VERSION( 2, 32, 0 )
#else

	g_thread_init(NULL);
#endif

	wsh_ssh_init();

	context = g_option_context_new("[HOSTS] - automatically add hostkeys to your hostkey file");
	g_option_context_add_main_entries(context, entries, NULL);
	if (! g_option_context_parse(context, &argc, &argv, &err)) {
		g_printerr("Option parsing failed: %s\n", err->message);
		g_error_free(err);
		return EXIT_FAILURE;
	}

	if (username == NULL) {
		username = g_strdup(g_get_user_name());
	}

#ifdef RANGE
	// Really fucking ugly code to resolve range
	if (range) {
		gchar* temp_res = "null,";

		if (wsh_exp_range_init(&err)) {
			g_printerr("%s\n", err->message);
			return EXIT_FAILURE;
		}

		for (gint i = 1; i < argc; i++) {
			gchar** exp_res = NULL;
			if (wsh_exp_range_expand(&exp_res, argv[i], &err)) {
				g_printerr("%s\n", err->message);
				return EXIT_FAILURE;
			}
			gchar* tmp_str = g_strjoinv(",", exp_res);
			gchar* tmp_joined_res = g_strconcat(temp_res, tmp_str, ",", NULL);

			if (i != 1)
				g_free(temp_res);
			g_free(tmp_str);
			g_strfreev(exp_res);

			temp_res = tmp_joined_res;
		}

		argv = g_strsplit(temp_res, ",", 0);
		argc = g_strv_length(argv);
		g_free(temp_res);
		wsh_exp_range_cleanup();
	}

#endif
	if (threads == 0 || argc < 5) {
		gint iret;
		for (gint i = 1; i < argc; i++) {
			if (!strncmp("", argv[i], 1)) continue;
			if ((iret = add_hostkey(argv[i], NULL))) {
				if (iret > ret) ret = iret;
			}
		}
	} else {
		GThreadPool* gtp;
		if ((gtp = g_thread_pool_new((GFunc)add_hostkey, NULL, threads, TRUE, &err)) == NULL) {
			g_printerr("%s\n", err->message);
			g_error_free(err);
			return EXIT_FAILURE;
		}

		for (gint i = 1; i < argc; i++) {
			if (strncmp("", argv[i], 1))
				g_thread_pool_push(gtp, argv[i], NULL);
		}

		g_thread_pool_free(gtp, FALSE, TRUE);
	}

	wsh_ssh_cleanup();
	g_free(username);
	g_option_context_free(context);

#ifdef RANGE
	if (range)
		g_strfreev(argv);
#endif

	return ret;
}

