# Copyright (c) 2022 Status Research & Development GmbH. Licensed under
# either of:
# - Apache License, version 2.0
# - MIT license
# at your option. This file may not be copied, modified, or distributed except
# according to those terms.

###########################
## nat-libs (nimbledeps) ##
###########################
# Builds miniupnpc and libnatpmp from the package installed by nimble under
# nimbledeps/pkgs2/. Invoked via $(NIMBLEDEPS_STAMP) / build-deps.
#
# NAT_TRAVERSAL_NIMBLEDEPS_DIR is evaluated at parse time, so targets that
# depend on it must be invoked via a recursive $(MAKE) call so the sub-make
# re-evaluates the variable after nimble setup has populated nimbledeps/.
#
# `ls -dt` (sort by modification time, newest first) is used to pick the
# latest installed version and is portable across Linux, macOS, and
# Windows (MSYS/MinGW).

NAT_TRAVERSAL_NIMBLEDEPS_DIR := $(shell ls -dt $(CURDIR)/nimbledeps/pkgs2/nat_traversal-* 2>/dev/null | head -1)

NAT_UNAME_M := $(shell uname -m)
ifeq ($(NAT_UNAME_M),x86_64)
  PORTABLE_NAT_MARCH := -mssse3
else
  PORTABLE_NAT_MARCH :=
endif

NAT_MINIUPNPC_REQUIRED_MEMBERS := \
	miniwget.o minixml.o igd_desc_parse.o minisoap.o minissdpc.o \
	miniupnpc.o upnpreplyparse.o upnpcommands.o upnperrors.o \
	connecthostport.o portlistingparse.o receivedata.o upnpdev.o addr_is_reserved.o
NAT_NATPMP_REQUIRED_MEMBERS := natpmp.o getgateway.o

define verify_nat_archive_members
@for member in $(2); do \
	$(AR) t "$(1)" | grep -Fqx "$$member" || { \
		echo "incomplete NAT archive $(1): missing $$member" >&2; \
		exit 1; \
	}; \
done
endef

.PHONY: rebuild-nat-libs-nimbledeps

rebuild-nat-libs-nimbledeps:
ifeq ($(NAT_TRAVERSAL_NIMBLEDEPS_DIR),)
	$(error No nat_traversal package found under nimbledeps/pkgs2/ — run 'make build-deps' first)
endif
	@echo "Rebuilding nat-libs from $(NAT_TRAVERSAL_NIMBLEDEPS_DIR)"
ifeq ($(OS), Windows_NT)
	+ "$(MAKE)" -C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/miniupnp/miniupnpc" \
		-f Makefile.mingw CC=$(CC) RM="rm -f" clean $(HANDLE_OUTPUT)
	+ PATH=".;$${PATH}" "$(MAKE)" -C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/miniupnp/miniupnpc" \
		-f Makefile.mingw CC=$(CC) CFLAGS="-Os -fPIC" libminiupnpc.a $(HANDLE_OUTPUT)
	$(call verify_nat_archive_members,$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/miniupnp/miniupnpc/libminiupnpc.a,$(NAT_MINIUPNPC_REQUIRED_MEMBERS))
	+ "$(MAKE)" -C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/libnatpmp-upstream" \
		OS=mingw CC=$(CC) clean $(HANDLE_OUTPUT)
	+ "$(MAKE)" -C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/libnatpmp-upstream" \
		OS=mingw CC=$(CC) \
		CFLAGS="-Wall -Wno-cpp -Os -fPIC -DWIN32 -DNATPMP_STATICLIB -DENABLE_STRNATPMPERR -DNATPMP_MAX_RETRIES=4 $(CFLAGS)" \
		libnatpmp.a $(HANDLE_OUTPUT)
	$(call verify_nat_archive_members,$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/libnatpmp-upstream/libnatpmp.a,$(NAT_NATPMP_REQUIRED_MEMBERS))
else
	+ "$(MAKE)" -C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/miniupnp/miniupnpc" \
		CC=$(CC) clean $(HANDLE_OUTPUT)
	+ "$(MAKE)" -C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/miniupnp/miniupnpc" \
		CC=$(CC) CFLAGS="-Os -fPIC $(PORTABLE_NAT_MARCH)" build/libminiupnpc.a $(HANDLE_OUTPUT)
	$(call verify_nat_archive_members,$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/miniupnp/miniupnpc/build/libminiupnpc.a,$(NAT_MINIUPNPC_REQUIRED_MEMBERS))
	+ "$(MAKE)" -C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/libnatpmp-upstream" \
		CC=$(CC) clean $(HANDLE_OUTPUT)
	+ "$(MAKE)" CFLAGS="-Wall -Wno-cpp -Os -fPIC $(PORTABLE_NAT_MARCH) -DENABLE_STRNATPMPERR -DNATPMP_MAX_RETRIES=4 $(CFLAGS)" \
		-C "$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/libnatpmp-upstream" \
		CC=$(CC) libnatpmp.a $(HANDLE_OUTPUT)
	$(call verify_nat_archive_members,$(NAT_TRAVERSAL_NIMBLEDEPS_DIR)/vendor/libnatpmp-upstream/libnatpmp.a,$(NAT_NATPMP_REQUIRED_MEMBERS))
endif
