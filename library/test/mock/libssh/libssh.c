#include "libssh/libssh.h"

#include <glib.h>

gint ssh_server_is_known_ret;
gint ssh_connect_ret;
gint ssh_write_knownhost_ret;
gint ssh_userauth_list_ret;
gint ssh_userauth_autopubkey_ret;
gint ssh_userauth_kbdint_ret;
gint ssh_userauth_kbdint_setanswer_ret;
gint ssh_userauth_password_ret;
gint ssh_channel_open_session_ret;
gint ssh_channel_request_exec_ret;

void set_ssh_connect_res(gint ret) {
	ssh_connect_ret = ret;
}

gint ssh_connect() {
	return ssh_connect_ret;
}

void set_ssh_is_server_known_res(gint ret) {
	ssh_server_is_known_ret = ret;
}

gint ssh_is_server_known() {
	return ssh_server_is_known_ret;
}

void ssh_disconnect() {
}

void ssh_free(ssh_session* session) {
	g_free(session);
}

const char* ssh_get_error() {
	return NULL;
}

ssh_session ssh_new() {
	return g_malloc0(sizeof(ssh_session));
}

void set_ssh_write_knownhost_res(gint ret) {
	ssh_write_knownhost_ret = ret;
}

gint ssh_write_knownhost() {
	return ssh_write_knownhost_ret;
}

gint ssh_options_set() {
	return 0;
}

void ssh_options_parse_config() {
}

void set_ssh_userauth_list_ret(gint ret) {
	ssh_userauth_list_ret = ret;
}

gint ssh_userauth_list() {
	return ssh_userauth_list_ret;
}

void set_ssh_userauth_autopubkey(gint ret) {
	ssh_userauth_autopubkey_ret = ret;
}

gint ssh_userauth_autopubkey() {
	return ssh_userauth_autopubkey_ret;
}

void set_ssh_userauth_kbdint_ret(gint ret) {
	ssh_userauth_kbdint_ret = ret;
}

gint ssh_userauth_kbdint() {
	return ssh_userauth_kbdint_ret;
}

void set_ssh_userauth_kbdint_setanswer_ret(gint ret) {
	ssh_userauth_kbdint_setanswer_ret = ret;
}

gint ssh_userauth_kbdint_setanswer() {
	return ssh_userauth_kbdint_setanswer_ret;
}

void set_ssh_userauth_password_ret(gint ret) {
	ssh_userauth_password_ret = ret;
}

gint ssh_userauth_password() {
	return ssh_userauth_password_ret;
}

ssh_channel ssh_channel_new() {
	return g_malloc0(sizeof(ssh_channel));
}

void set_ssh_channel_open_session_ret(gint ret) {
	ssh_channel_open_session_ret = ret;
}

gint ssh_channel_open_session() {
	return ssh_channel_open_session_ret;
}

void set_ssh_channel_request_exec_ret(gint ret) {
	ssh_channel_request_exec_ret = ret;
}

gint ssh_channel_request_exec() {
	return ssh_channel_request_exec_ret;
}

void ssh_channel_close() {
}

void ssh_channel_free(ssh_channel chan) {
	g_free(chan);
}

