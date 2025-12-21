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
	SHELL := $(COMSPEC)
	RM = del /Q
	RMDIR = rmdir /Q /S
	COPY = copy
	EXE_EXT = .exe
	LIB_PREFIX =
	SO_EXT = .dll
	STATIC_EXT = .lib
else
	SHELL := /bin/bash
	RM = rm -f
	RMDIR = rm -rf
	COPY = cp
	EXE_EXT =
	LIB_PREFIX = lib
	SO_EXT = .so
	STATIC_EXT = .a
endif
ifeq ($(OS),Windows_NT)
	global_export = setx $(1) $(2)
else
	global_export = echo "export $(1)=$(2)" >> ~/.bashrc
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
	OUTOBJ_FLAG = /c /Fo$@
	OUTEXE_FLAG = /Fe$@
	OUTDLL_FLAG = /Fe$@
	OBJ_EXT = .obj
else ifeq ($(CC),gcc)
	IMPLIB_EXT = .dll.a
	OUTOBJ_FLAG = -c -o $@
	OUTEXE_FLAG = -o $@
	OUTDLL_FLAG = -o $@
	OUT = -o $@
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
SRC ?= src

# Define installation directory
ifeq ($(origin GLOBALPREFIX),undefined)
	ifeq ($(origin CPGLOBALHOME),undefined)
		ifeq ($(OS),Windows_NT)
			GLOBALPREFIX = $(ProgramFiles)/cp
		else
			GLOBALPREFIX = /usr/local/cp
		endif
	else
		GLOBALPREFIX = $(CPGLOBALHOME)
	endif
endif

ifneq ($(origin PREFIX),undefined)
	$(error PREFIX is removed, use GLOBALPREFIX instead)
endif
PREFIX = $(GLOBALPREFIX)/$(VERSION)

# Define target all as a default target

TARGET = $(CPIMPLIB) $(DIST)/cp$(EXE_EXT)
all: directories $(TARGET)
.PHONY: all

directories:
	@$(call mkdir,$(DIST))
	@$(call mkdir,$(BUILD))
.PHONY: directories

# Define object files

OBJECTS =
$(BUILD)/main$(OBJ_EXT): $(SRC)/main.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
OBJECTS += $(BUILD)/main$(OBJ_EXT)

$(BUILD)/launch$(OBJ_EXT): $(SRC)/launch.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
#OBJECTS += $(BUILD)/launch$(OBJ_EXT)
# This object file cannot not be linked into the library(but the executable file).

# Define library and executable files and resources(Windows only)

ifeq ($(OS),Windows_NT)
LIBRES = $(BUILD)/lib.res
$(LIBRES): $(SRC)/lib.rc
	rc /nologo /fo$@ $<
EXERES = $(BUILD)/cp.res
$(EXERES): $(SRC)/cp.rc
	rc /nologo /fo$@ $<
else
LIBRES =
EXERES =
endif

$(DIST)/$(LIB_PREFIX)cp$(SO_EXT): $(OBJECTS) $(LIBRES)
	$(CC) $(CFLAGS) $(DLL_FLAG) $(OUTDLL_FLAG) $(OBJECTS) \
	$(call IMPLIB_FLAG,$(DIST)/$(LIB_PREFIX)cp$(IMPLIB_EXT)) $(LINK_OPT) $(NOEXP) $(LIBRES)

ifeq ($(OS),Windows_NT)
CPIMPLIB = $(DIST)/$(LIB_PREFIX)cp$(IMPLIB_EXT)
$(CPIMPLIB): $(DIST)/$(LIB_PREFIX)cp$(SO_EXT)
else
CPIMPLIB = $(DIST)/$(LIB_PREFIX)cp$(SO_EXT)
$(CPIMPLIB):
endif

$(DIST)/cp$(EXE_EXT): $(BUILD)/launch$(OBJ_EXT) $(CPIMPLIB) $(EXERES)
	$(CC) $(CFLAGS) $(OUTEXE_FLAG) $< \
	$(LINK_OPT) $(call LIBPATH_FLAG,$(DIST)) $(call DLIB_FLAG,cp) $(EXERES)

# Define targets

clean-build:
	$(RM) $(call fix_path,$(BUILD)/*)
.PHONY: clean-build

clean: clean-build
	$(RM) $(call fix_path,$(DIST)/*)
.PHONY: clean

install-files-only: all
	$(call mkdir,$(call fix_path,$(GLOBALPREFIX)))
	$(call mkdir,$(call fix_path,$(PREFIX)))
	$(COPY) $(call fix_path,$(DIST)/*) $(call fix_path,$(PREFIX))
.PHONY: install-files-only

install: install-files-only
	$(call global_export,CPGLOBALHOME,$(call fix_path,$(GLOBALPREFIX)))
	$(call global_export,CPLOCALHOME,$(call fix_path,$(PREFIX)))
.PHONY: install

uninstall:
	$(RMDIR) $(PREFIX)
.PHONY: uninstall

help:
	@echo Makefile for cp project
	@echo Available targets:
	@echo no-target-name - Same as target all
	@echo all            - Build the project
	@echo clean          - Clean the build and dist directories
	@echo clean-all      - Clean the build directory only
	@echo install        - Install the project in $(PREFIX)
	@echo uninstall      - Uninstall the project from $(PREFIX)
	@echo help           - Display this help message
	@echo Use MSVC on Windows or GCC on Linux/MacOS as default compiler.
	@echo Now only support building with MSVC or GCC.
	@echo Run in Visual Studio Developer Command Prompt to build with MSVC.
	@echo Run "make help-install" to see verbose help for installing. 
.PHONY: help

help-install:
	@echo Now install the project in $(PREFIX) as default.
	@echo You can change the installation directory by setting some environment variables:
	@echo CPGLOBALHOME - This directory is often setted automatically by the installer.
	@echo GLOBALPREFIX - This directory will be used as the installation directory if CPGLOBALHOME is not setted.
	@echo When you set any of these variables, the installation directory will 
	@echo become a version-numbered subdirectory under the path specified by that variable.
	@echo For example, if CPGLOBALHOME is set to "C:\Program Files\cp", 
	@echo the installation directory will be "C:\Program Files\cp\$(VERSION)".
	@echo When installing, the installer will not only copy the files to the installation directory,
	@echo but also create some environment variables for the program to use at runtime, such as CPGLOBALHOME,
	@echo unless when running "make install-files-only".
.PHONY: help-install