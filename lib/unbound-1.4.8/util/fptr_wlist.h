/*
 * util/fptr_wlist.h - function pointer whitelists.
 *
 * Copyright (c) 2007, NLnet Labs. All rights reserved.
 *
 * This software is open source.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * Neither the name of the NLNET LABS nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 *
 * This file contains functions that check function pointers.
 * The functions contain a whitelist of known good callback values.
 * Any other values lead to an error. 
 * 
 * This prevent heap overflow based exploits, where the callback pointer
 * is overwritten by a buffer overflow (apart from this defense, buffer 
 * overflows should be fixed of course).
 *
 * Function pointers are used in
 * 	o network code callbacks.
 * 	o rbtree, lruhash, region data manipulation
 *		in lruhash, the assertions are before the critical regions.
 *		in other places, assertions are before the callback.
 * 	o module operations.
 */

#ifndef UTIL_FPTR_WLIST_H
#define UTIL_FPTR_WLIST_H
#include "util/netevent.h"
#include "util/storage/lruhash.h"
#include "util/module.h"
#include "util/tube.h"
#include "services/mesh.h"

/**
 * Macro to perform an assertion check for fptr wlist checks.
 * Does not get disabled in optimize mode. Check adds security by layers.
 */
#define fptr_ok(x) \
	do { if(!(x)) \
		fatal_exit("%s:%d: %s: pointer whitelist %s failed", \
		__FILE__, __LINE__, __func__, #x); \
	} while(0);

/**
 * Check function pointer whitelist for comm_point callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_point(comm_point_callback_t *fptr);

/**
 * Check function pointer whitelist for raw comm_point callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_point_raw(comm_point_callback_t *fptr);

/**
 * Check function pointer whitelist for comm_timer callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_timer(void (*fptr)(void*));

/**
 * Check function pointer whitelist for comm_signal callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_comm_signal(void (*fptr)(int, void*));

/**
 * Check function pointer whitelist for event structure callback values.
 * This is not called by libevent itself, but checked by netevent.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_event(void (*fptr)(int, short, void *));

/**
 * Check function pointer whitelist for pending udp callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_pending_udp(comm_point_callback_t *fptr);

/**
 * Check function pointer whitelist for pending tcp callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_pending_tcp(comm_point_callback_t *fptr);

/**
 * Check function pointer whitelist for serviced query callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_serviced_query(comm_point_callback_t *fptr);

/**
 * Check function pointer whitelist for rbtree cmp callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_rbtree_cmp(int (*fptr) (const void *, const void *));

/**
 * Check function pointer whitelist for lruhash sizefunc callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_hash_sizefunc(lruhash_sizefunc_t fptr);

/**
 * Check function pointer whitelist for lruhash compfunc callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_hash_compfunc(lruhash_compfunc_t fptr);

/**
 * Check function pointer whitelist for lruhash delkeyfunc callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_hash_delkeyfunc(lruhash_delkeyfunc_t fptr);

/**
 * Check function pointer whitelist for lruhash deldata callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_hash_deldatafunc(lruhash_deldatafunc_t fptr);

/**
 * Check function pointer whitelist for lruhash markdel callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_hash_markdelfunc(lruhash_markdelfunc_t fptr);

/**
 * Check function pointer whitelist for module_env send_query callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_modenv_send_query(struct outbound_entry* (*fptr)(
	uint8_t* qname, size_t qnamelen, uint16_t qtype, uint16_t qclass, 
	uint16_t flags, int dnssec, int want_dnssec,
	struct sockaddr_storage* addr, socklen_t addrlen, 
	struct module_qstate* q));

/**
 * Check function pointer whitelist for module_env detach_subs callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_modenv_detach_subs(void (*fptr)(
	struct module_qstate* qstate));

/**
 * Check function pointer whitelist for module_env attach_sub callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_modenv_attach_sub(int (*fptr)(
	struct module_qstate* qstate, struct query_info* qinfo, 
	uint16_t qflags, int prime, struct module_qstate** newq));

/**
 * Check function pointer whitelist for module_env kill_sub callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_modenv_kill_sub(void (*fptr)(struct module_qstate* newq));

/**
 * Check function pointer whitelist for module_env detect_cycle callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_modenv_detect_cycle(int (*fptr)(
	struct module_qstate* qstate, struct query_info* qinfo, 
	uint16_t flags, int prime));

/**
 * Check function pointer whitelist for module init call values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_mod_init(int (*fptr)(struct module_env* env, int id));

/**
 * Check function pointer whitelist for module deinit call values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_mod_deinit(void (*fptr)(struct module_env* env, int id));

/**
 * Check function pointer whitelist for module operate call values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_mod_operate(void (*fptr)(struct module_qstate* qstate, 
	enum module_ev event, int id, struct outbound_entry* outbound));

/**
 * Check function pointer whitelist for module inform_super call values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_mod_inform_super(void (*fptr)(
	struct module_qstate* qstate, int id, struct module_qstate* super));

/**
 * Check function pointer whitelist for module clear call values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_mod_clear(void (*fptr)(struct module_qstate* qstate, 
	int id));

/**
 * Check function pointer whitelist for module get_mem call values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_mod_get_mem(size_t (*fptr)(struct module_env* env, int id));

/**
 * Check function pointer whitelist for alloc clear on id overflow call values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_alloc_cleanup(void (*fptr)(void*));

/**
 * Check function pointer whitelist for tube listen handler values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_tube_listen(tube_callback_t* fptr);

/**
 * Check function pointer whitelist for mesh state callback values.
 *
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_mesh_cb(mesh_cb_func_t fptr);

/**
 * Check function pointer whitelist for config_get_option func values.
 * @param fptr: function pointer to check.
 * @return false if not in whitelist.
 */
int fptr_whitelist_print_func(void (*fptr)(char*,void*));

/** Due to module breakage by fptr wlist, these test app declarations
 * are presented here */
/** 
 * compare two order_ids from lock-verify test app 
 * @param e1: first order_id
 * @param e2: second order_id
 * @return compare code -1, 0, +1 (like memcmp).
 */
int order_lock_cmp(const void* e1, const void* e2);

/** 
 * compare two codeline structs for rbtree from memstats test app 
 * @param a: codeline
 * @param b: codeline
 * @return compare code -1, 0, +1 (like memcmp).
 */
int codeline_cmp(const void* a, const void* b);

/** compare two replay_vars */
int replay_var_compare(const void* a, const void* b);

#endif /* UTIL_FPTR_WLIST_H */
