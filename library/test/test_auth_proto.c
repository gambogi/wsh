#include <glib.h>

#include "auth.pb-c.h"

static const guint8 ai_encoded[12] = { 0x0a, 0x04, 0x77, 0x69, 0x6c, 0x6c, 0x12, 0x04, 0x74, 0x65, 0x73, 0x74 };
static const gsize ai_encoded_len = 12;
static gchar* ai_username = "will";
static gchar* ai_password = "test";

static void test_packing_auth_proto(void) {
	AuthInfo ai = AUTH_INFO__INIT;
	guint8* buf;
	gsize len;

	ai.username = ai_username;
	ai.password = ai_password;

	len = auth_info__get_packed_size(&ai);

	buf = g_slice_alloc0(len);
	auth_info__pack(&ai, buf);

	g_assert(len == ai_encoded_len);

	for (gint i = 0; i < len; i++) {
		g_assert(buf[i] == ai_encoded[i]);
	}

	g_slice_free1(len, buf);
}

static void test_unpacking_auth_proto(void) {
	AuthInfo* ai = NULL;

	ai = auth_info__unpack(NULL, ai_encoded_len, ai_encoded);
	
	g_assert(ai != NULL);
	g_assert_cmpstr(ai->username, ==, ai_username);
	g_assert_cmpstr(ai->password, ==, ai_password);

	auth_info__free_unpacked(ai, NULL);
}

int main(int argc, char** argv) {
	g_test_init(&argc, &argv, NULL);

	g_test_add_func("/Library/Protocol/PackAuthInfo", test_packing_auth_proto);
	g_test_add_func("/Library/Protocol/UnpackAuthInfo", test_unpacking_auth_proto);

	return g_test_run();
}

