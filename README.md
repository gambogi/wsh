# wsh

wsh is a shell-like program made for sysadmins that need to administer
several machines at once. With wsh, you can `ssh` into multiple
machines and run one-off commands very simply

## What wsh isn't

* configuration management 
* a shell 
* an `ssh` replacement

## Dependencies

* `glib2`
* `libssh` (**NOT** libssh2) 
* `protobuf-c`
* `sudo`

### Build deps

* `cmake`
* c compiler (`gcc`/`clang`) 
* `make`/`ninja`

## Architecture

`wshd` is a program that resides on remote hosts. `wshc` ssh's into
them, honoring your ssh config and exec's `wshd`. Over the secure ssh
pipe, `wshc` issues instructions to `wshd`, and `wshd` executes them on
the remote host. If privileges need to be raised, `wshc` will prompt
you for creds prior to ssh'ing, and submit them once the ssh
connection has been established.

## Advantages

* configuration-less

wsh honors your existing `ssh` and `sudo` configuration files, leaving
all of the work of authorization and authentication to `ssh` and `sudo`

* sudo-ready

Unlike `dsh`, wsh has `sudo` support built in, so that you don't need
to use some kind of hack to get your `sudo` creds over the network.

* logging

wsh logs everything to syslog

