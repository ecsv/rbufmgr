#!/usr/bin/make -f
# SPDX-License-Identifier: MIT
# -*- makefile -*-
#
# Non-linear ring buffer block management test
#
# SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>

TESTS_CXX_COMPATIBLE = \
 rbufmgr_init \
 rbufmgr_insert_linear \
 rbufmgr_read_linear \
 rbufmgr_insert_random \
 rbufmgr_read_random \
 rbufmgr_insert_random_block \
 rbufmgr_read_random_block \
 rbufmgr_insert_random_old \
 rbufmgr_read_random_old \
 rbufmgr_insert_random_outdated \
 rbufmgr_read_random_outdated \

TESTS_C_ONLY = \

TESTS_ALL = $(TESTS_CXX_COMPATIBLE) $(TESTS_C_ONLY)

# tests flags and options
CFLAGS += -g3 -pedantic -Wall -W -Werror -MD -MP
ifeq ("$(BUILD_CXX)", "1")
	CFLAGS += -std=c++98
	TESTS = $(TESTS_CXX_COMPATIBLE)
	COMPILER_NAME=$(CXX)
else
	CFLAGS += -std=c99
	TESTS += $(TESTS_ALL)
	COMPILER_NAME=$(CC)
endif

# disable verbose output
ifneq ($(findstring $(MAKEFLAGS),s),s)
ifndef V
	Q_CC = @echo '    $(COMPILER_NAME)' $@;
	Q_LD = @echo '    $(COMPILER_NAME)' $@;
	export Q_CC
	export Q_LD
endif
endif

# standard build tools
CC ?= gcc
RM ?= rm -f
ifeq ("$(BUILD_CXX)", "1")
COMPILE.c = $(Q_CC)$(CXX) -x c++ $(CFLAGS) $(CXXLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CXX) $(CFLAGS) $(CXXLAGS) $(LDFLAGS) $(TARGET_ARCH)
else
COMPILE.c = $(Q_CC)$(CC) -x c $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
LINK.o = $(Q_LD)$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_ARCH)
endif

TESTS_OK = $(TESTS:=.ok)

# default target
all: $(TESTS_OK)

$(TESTS_OK): %.ok: %
	@echo "T:  $(COMPILER_NAME) $(TESTRUN_NAME) $<"
	@$(TESTRUN_WRAPPER) ./$<
	@touch $@

# standard build rules
.SUFFIXES: .o .c
.c.o:
	$(COMPILE.c) -o $@ $<

simple_buffer.o: simple_buffer.c
ring_buffer.o: ring_buffer.c

$(TESTS): %: %.o simple_buffer.o ring_buffer.o
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	@$(RM) $(TESTS_ALL) $(DEP) $(TESTS_OK) $(TESTS:=.o) $(TESTS:=.d)
	@$(RM) simple_buffer.o ring_buffer.o
	@$(RM) simple_buffer.d ring_buffer.d

# load dependencies
DEP = $(TESTS:=.d) simple_buffer.d ring_buffer.d
-include $(DEP)

.PHONY: all clean
