#
# Makefile.am - the structure of Makefile
# Copyright (C) 2026 Huang Jiangyao. All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# Flags

AM_CFLAGS = -Wall -Wextra -Werror -std=c99
LDADD = @windows_libs@ libcp.la

# Main programs

bin_PROGRAMS = \
	cpc
cpc_SOURCES = cpc_src/launch.c cpc_src/main.h

# Libraries

lib_LTLIBRARIES = libcp.la
# Please put new source files in alphabetical order.
libcp_la_SOURCES = \
	commandline.c \
	commandline.h \
	cpassert.h \
	cpc_src/main.c \
	cpc_src/main.h \
	cptypes.h \
	exports.h \
	parsearg.c \
	parsearg.h \
	path.c \
	path.h \
	platform/mmap.c \
	platform/mmap.h \
	report_error.c \
	report_error.h \
	safe_string.c \
	safe_string.h \
	version.c \
	version.h

libcp_la_LDFLAGS = -version-info @libcp_la_version_info@

# Test programs

check_PROGRAMS = \
	test_mmap

test_mmap_SOURCES = \
	Test/platform/mmap.c

# Check headers
# XXX This replaces .github/workflows/header_syntax.yml.

# When .github/workflows/build.yml run `check` target
# the following target will be run.

check-headers:
	@failed=; \
	for h in $$(find . -name "*.h" -not -path "./.git/*"); \
	do \
	printf "%s\n" "Check header: $$h"; \
	# Use `-include config.h` since we are \
	# compiling a single header file \
	# which does not include config.h \
	# automatically. (this is usually done \
	# by C source files at the beginning). \
	$(COMPILE) -fsyntax-only -include config.h $$h || failed=y; \
	done; \
	if test $$failed; \
	then \
	echo "Check does not pass.   Stop."; \
	exit 1; \
	else \
	echo "Check passed."; \
	fi

.PHONY: check-headers

check: check-headers
