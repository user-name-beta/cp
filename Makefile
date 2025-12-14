# Makefile
#
# Copyright (c) 2025 user-name-beta. All rights reserved.
# Licensed under the MIT license.
# See LICENSE file in the project root for full license information.
#
# This file is used to build the project.

# Version

VERSION := p0.0.0

# Define tools

ifeq ($(OS),Windows_NT)
	RM = del /Q
	RMDIR = rmdir /Q /S
	CP = copy
	EXE_EXT = .exe
	LIB_PREFIX =
	SO_EXT = .dll
	STATIC_EXT = .lib
else
	RM = rm -f
	RMDIR = rm -rf
	CP = cp
	EXE_EXT =
	LIB_PREFIX = lib
	SO_EXT = .so
	STATIC_EXT = .a
endif

ifeq ($(OS),Windows_NT)
	mkdir = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
else
	mkdir = mkdir -p $(1)
endif

ifeq ($(OS),Windows_NT)
	fix_path = "$(subst /,\,$(1))"
else
	fix_path = "$(1)"
endif

# Define CC and CFLAGS

CFLAGS ?=
NDEBUG ?= 0

ifeq ($(filter $(origin CC),undefined default),$(origin CC))
	ifeq ($(OS),Windows_NT)
		CC = cl
	else
		CC = gcc
	endif
endif

ifeq ($(CC),cl)
	ifeq ($(origin VisualStudioVersion),undefined)
$(error Build with MSVC but not in Visual Studio Command Prompt)
	endif
endif

ifneq ($(CC),cl)
ifneq ($(CC),gcc)
$(error Unsupported compiler $(CC))
endif
endif

ifeq ($(CC),cl)
	CFLAGS += /nologo /DYNAMICBASE
	ifeq ($(NDEBUG),0)
		CFLAGS += /Od /D_DEBUG
	else
		CFLAGS += /O2
	endif
else ifeq ($(CC),gcc)
	CFLAGS += -std=c99 -Wall -fPIC -fvisibility=hidden
	ifeq ($(NDEBUG),0)
		CFLAGS += -O0 -DDEBUG -g
	else
		CFLAGS += -O2 -DNDEBUG
	endif
endif

ifeq ($(CC),cl)
	IMPLIB_EXT = .lib
	OUTOBJ_FLAG = /Fo$@
	OUTEXE_FLAG = /Fe$@
	OUTDLL_FLAG = /Fe$@
	OBJ = /c
	OBJ_EXT = .obj
else ifeq ($(CC),gcc)
	IMPLIB_EXT = .dll.a
	OUTOBJ_FLAG = -o $@
	OUTEXE_FLAG = -o $@
	OUTDLL_FLAG = -o $@
	OUT = -o $@
	OBJ = -c
	OBJ_EXT = .o
endif

# Define variables about linking

ifeq ($(CC),cl)
	DLIB_FLAG = $(1)$(IMPLIB_EXT)
	SLIB_FLAG = $(1)$(STATIC_EXT)
	LIBPATH_FLAG = /LIBPATH:$(1)
	DLL_FLAG = /LD
	IMPLIB_FLAG = /IMPLIB:$(1)
	LINK_OPT = /link
	NOEXP = /NOEXP
else ifeq ($(CC),gcc)
	DLIB_FLAG = -l$(1)
	SLIB_FLAG = -l$(1)
	LIBPATH_FLAG = -L$(1)
	DLL_FLAG = -shared
	IMPLIB_FLAG = -Wl,--out-implib,$(1)
	LINK_OPT =
	NOEXP =
endif

# Define AR and ARFLAGS

ifeq ($(CC),cl)
	AR = lib
else ifeq ($(CC),gcc)
	AR = ar
endif
ifeq ($(AR),ar)
	ARFLAGS = rcs $(1)
else ifeq ($(AR),lib)
	ARFLAGS = /nologo /OUT:$(1)
endif

# Define the output directory

BUILD ?= build
DIST ?= dist
SRC ?= .
ifeq ($(origin CPGLOBALHOME),undefined)
	ifeq ($(OS),Windows_NT)
		PREFIX ?= $(call fix_path,$(ProgramFiles)/cp/$(VERSION))
	else
		PREFIX ?= $(call fix_path,/usr/local/cp/$(VERSION))
	endif
else
	PREFIX ?= $(call fix_path,$(CPGLOBALHOME)/$(VERSION))
endif

# Define target all as a default target

all: directories $(DIST)/$(LIB_PREFIX)cp$(SO_EXT) $(DIST)/cp$(EXE_EXT)
.PHONY: all

directories:
	$(call mkdir,$(DIST))
	$(call mkdir,$(BUILD))
.PHONY: directories

# Define object files

$(BUILD)/main$(OBJ_EXT): $(SRC)/main.c
	$(CC) $(CFLAGS) $(OBJ) $(OUTOBJ_FLAG) $<
$(BUILD)/launch$(OBJ_EXT): $(SRC)/launch.c
	$(CC) $(CFLAGS) $(OBJ) $(OUTOBJ_FLAG) $<

# Define library and executable files

$(DIST)/$(LIB_PREFIX)cp$(SO_EXT): $(BUILD)/main$(OBJ_EXT)
	$(CC) $(CFLAGS) $(DLL_FLAG) $(OUTDLL_FLAG) $< \
	$(call IMPLIB_FLAG,$(DIST)/$(LIB_PREFIX)cp$(IMPLIB_EXT)) $(LINK_OPT) $(NOEXP)

ifeq ($(OS),Windows_NT)
$(DIST)/$(LIB_PREFIX)cp$(IMPLIB_EXT): $(DIST)/$(LIB_PREFIX)cp$(SO_EXT)
CPIMPLIB = $(DIST)/$(LIB_PREFIX)cp$(IMPLIB_EXT)
else
$(DIST)/$(LIB_PREFIX)cp$(IMPLIB_EXT):
CPIMPLIB = $(DIST)/$(LIB_PREFIX)cp$(SO_EXT)
endif

$(DIST)/cp$(EXE_EXT): $(BUILD)/launch$(OBJ_EXT) $(CPIMPLIB)
	$(CC) $(CFLAGS) $(OUTEXE_FLAG) $< \
	$(LINK_OPT) $(call LIBPATH_FLAG,$(DIST)) $(call DLIB_FLAG,cp)

# Define targets

clean:
	$(RM) $(call fix_path,$(BUILD)/*)
.PHONY: clean

clean-all: clean
	$(RM) $(call fix_path,$(DIST)/*)
.PHONY: clean-all

install: all
	$(call mkdir,$(PREFIX)/..)
	$(call mkdir,$(PREFIX))
	$(CP) $(call fix_path,$(DIST)/*) $(PREFIX)
.PHONY: install

uninstall:
	$(RMDIR) $(PREFIX)
.PHONY: uninstall

help:
	@echo Makefile for cp project
	@echo Available targets:
	@echo no-target-name - Same as target all
	@echo all            - Build the project
	@echo clean          - Clean the build directory
	@echo clean-all      - Clean the build and distribution directories
	@echo install        - Install the project in $(PREFIX) or PREFIX environment variable
	@echo uninstall      - Uninstall the project from $(PREFIX) or PREFIX environment variable
	@echo help           - Display this help message
	@echo If you make on Windows, not to use MSYS2 bash.
	@echo Use MSVC on Windows or GCC on Linux/MacOS as default compiler.
	@echo Run in Visual Studio Developer Command Prompt to build with MSVC.
	@echo Setting CPGLOBALHOME environment variable can specify the installation directory.
.PHONY: help