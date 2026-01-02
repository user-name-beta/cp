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
	SHELL := cmd.exe
	RM = del /Q
	RMDIR = rmdir /Q /S
	COPY = copy
	EXE_EXT = .exe
	LIB_PREFIX =
	SO_EXT = .dll
	STATIC_EXT = .lib
else
	SHELL := /bin/bash
	RM = rm -rf
	RMDIR = rm -rf
	COPY = cp -r
	EXE_EXT =
	LIB_PREFIX = lib
	SO_EXT = .so
	STATIC_EXT = .a
endif
ifeq ($(OS),Windows_NT)
	global_export = powershell -Command "[Environment]::SetEnvironmentVariable('$(1)', '$(2)', 'User')"
else
	global_export = echo "export $(1)='$(2)'" >> ~/.bashrc
endif

ifeq ($(OS),Windows_NT)
	mkdir = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
else
	mkdir = mkdir -p $(1)
endif

ifeq ($(OS),Windows_NT)
	fix_path = $(subst /,\,$(1))
else
	fix_path = $(1)
endif

# Define CC and CFLAGS

CFLAGS ?=
RELEASE ?= 0

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
	ifeq ($(RELEASE),0)
		CFLAGS += /Od /D_DEBUG
	else
		CFLAGS += /O2
	endif
else ifeq ($(CC),gcc)
	CFLAGS += -std=c99 -Wall -fPIC -fvisibility=hidden
	ifeq ($(RELEASE),0)
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
	MACRO = /D$(1)
else ifeq ($(CC),gcc)
	IMPLIB_EXT = .dll.a
	OUTOBJ_FLAG = -c -o $@
	OUTEXE_FLAG = -o $@
	OUTDLL_FLAG = -o $@
	OUT = -o $@
	OBJ_EXT = .o
	MACRO = -D$(1)
endif

CFLAGS += $(call MACRO,_CP_STATIC_IMPORT_)

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
	ifeq ($(OS),Windows_NT)
		IMPLIB_FLAG = -Wl,--out-implib,$(1)
	else
		IMPLIB_FLAG =
	endif
	LINK_OPT =
	NOEXP =
	ifneq ($(OS),Windows_NT) # Linux
		RPATH_FLAG = -Wl,-rpath,$(1)
	else
		RPATH_FLAG =
	endif
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

# Define Resource compiler (rc or windres) flags

ifeq ($(OS),Windows_NT)
ifeq ($(CC),cl)
	RC = rc
	RCFLAGS = /nologo /fo$@ $<
else ifeq ($(CC),gcc)
	RC = windres
	RCFLAGS = -O coff -o $@ -i $<
endif
else
	RC =
	RCFLAGS =
endif

# Define the output directory

BUILD ?= build
DIST ?= dist
SRC ?= src

# Define installation directory
ifeq ($(origin PREFIX),undefined)
	ifeq ($(origin CPGLOBALHOME),undefined)
		ifeq ($(OS),Windows_NT)
			PREFIX = $(ProgramFiles)/cp
		else
			PREFIX = /usr/local/cp
		endif
	else
		PREFIX ?= $(call fix_path,$(CPGLOBALHOME))
	endif
endif
LOCALPREFIX = $(PREFIX)/$(VERSION)

# Define API headers

API_HEADERS = # No API headers yet.

# Define dependencies of headers and source files

$(BUILD)/depends.d: $(wildcard $(SRC)/*.*) | directories
ifeq ($(OS),Windows_NT)
	powershell -ExecutionPolicy Bypass -File ./depends.ps1 $(SRC) $@
else
	@chmod +x ./depends.sh
	./depends.sh $(SRC) $@
endif

ifeq ($(filter clean clean-build help help-install install install-files-only uninstall uninstall-all $(BUILD)/depends.d,$(MAKECMDGOALS)),)
-include $(BUILD)/depends.d
endif

# Define object files

OBJECTS =
$(BUILD)/main$(OBJ_EXT): $(SRC)/main.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
OBJECTS += $(BUILD)/main$(OBJ_EXT)

$(BUILD)/path$(OBJ_EXT): $(SRC)/path.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
OBJECTS += $(BUILD)/path$(OBJ_EXT)

$(BUILD)/report_error$(OBJ_EXT): $(SRC)/report_error.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
OBJECTS += $(BUILD)/report_error$(OBJ_EXT)

$(BUILD)/parsearg$(OBJ_EXT): $(SRC)/parsearg.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
OBJECTS += $(BUILD)/parsearg$(OBJ_EXT)

$(BUILD)/version$(OBJ_EXT): $(SRC)/version.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
OBJECTS += $(BUILD)/version$(OBJ_EXT)

$(BUILD)/launch$(OBJ_EXT): $(SRC)/launch.c
	$(CC) $(CFLAGS) $(OUTOBJ_FLAG) $<
#OBJECTS += $(BUILD)/launch$(OBJ_EXT)
# This object file cannot not be linked into the library(but the executable file).

# Define library and executable files and resources(Windows only)

ifeq ($(OS),Windows_NT)
LIBRES = $(BUILD)/lib.res
$(LIBRES): $(SRC)/lib.rc
	$(RC) $(RCFLAGS)
EXERES = $(BUILD)/cp.res
$(EXERES): $(SRC)/cp.rc
	$(RC) $(RCFLAGS)
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
	$(LINK_OPT) $(call LIBPATH_FLAG,$(DIST)) $(call DLIB_FLAG,cp) $(EXERES) \
	$(call RPATH_FLAG,\$$ORIGIN)

# Define targets

# Define target all as a default target

TARGET = directories $(CPIMPLIB) $(DIST)/cp$(EXE_EXT) api_headers
all: $(TARGET)
.PHONY: all
.DEFAULT_GOAL := all

api_headers: $(API_HEADERS)
	@$(call mkdir,"$(call fix_path,$(DIST)/include)")
#	$(COPY) $(API_HEADERS) "$(call fix_path,$(DIST)/include/)"
# No API headers yet. This command will cause an error.
.PHONY: api_headers

directories:
	@$(call mkdir,$(DIST))
	@$(call mkdir,$(BUILD))
.PHONY: directories

clean-build:
	$(RM) $(call fix_path,$(BUILD)/*)
.PHONY: clean-build

clean: clean-build
	$(RM) $(call fix_path,$(DIST)/*)
.PHONY: clean

install-files-only: all
	$(call mkdir,"$(call fix_path,$(PREFIX))")
	$(call mkdir,"$(call fix_path,$(LOCALPREFIX))")
	$(COPY) $(call fix_path,$(DIST)/*) "$(call fix_path,$(LOCALPREFIX))"
.PHONY: install-files-only

install: install-files-only
	$(call global_export,CPGLOBALHOME,$(call fix_path,$(PREFIX)))
	$(call global_export,CPLOCALHOME,$(call fix_path,$(LOCALPREFIX)))
	@echo Installed cp $(VERSION) in $(LOCALPREFIX)
	@echo Please remove the old path and add $(call fix_path,$(LOCALPREFIX)) to PATH to use cp $(VERSION).
	@echo Please also add $(call fix_path,$(LOCALPREFIX)) to LD_LIBRARY_PATH
	@echo on Unix-like systems to use the library.
.PHONY: install

uninstall-all: uninstall
	$(RMDIR) "$(call fix_path,$(PREFIX))"
	$(call global_export,CPGLOBALHOME,)
	@echo Uninstalled all versions of cp from $(PREFIX)
.PHONY: uninstall-all

uninstall:
	$(RM) "$(call fix_path,$(LOCALPREFIX)/*)"
	$(call global_export,CPLOCALHOME,)
	@echo Uninstalled cp $(VERSION) from $(LOCALPREFIX)
	@echo Please remove $(call fix_path,$(LOCALPREFIX)) from PATH and LD_LIBRARY_PATH.
.PHONY: uninstall

help:
	@echo Makefile for cp project
	@echo Available targets:
	@echo no-target-name - Same as target all
	@echo all            - Build the project
	@echo clean          - Clean the build and dist directories
	@echo clean-all      - Clean the build directory only
	@echo install        - Install the project in $(LOCALPREFIX)
	@echo uninstall      - Uninstall the project from $(LOCALPREFIX)
	@echo uninstall-all  - Uninstall all versions of the project from $(PREFIX)
	@echo help           - Display this help message
	@echo Use MSVC on Windows or GCC on Linux/MacOS as default compiler.
	@echo Now only support building with MSVC or GCC.
	@echo Run in Visual Studio Developer Command Prompt to build with MSVC.
	@echo Run "make help-install" to see verbose help for installing. 
.PHONY: help

help-install:
	@echo Now install the project in $(LOCALPREFIX) as default.
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
