dnl
dnl configure.ac - configure script.
dnl Copyright (C) 2025-2026 Huang Jiangyao. All rights reserved.
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <https://www.gnu.org/licenses/>.
dnl

dnl Custom macros for CP.

m4_define([CP_VERSION], [1.0.0])

dnl Version information for libcp.la.
dnl The format is "current:revision:age".

m4_define([LIBCP_VERSION], [0:0:0])

dnl Initialize Autoconf.

AC_PREREQ([2.71])
AC_INIT([CP],[CP_VERSION],[https://github.com/user-name-beta/cp/issues])

AC_CONFIG_SRCDIR([src/version.c])

dnl Initialize Automake.

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

dnl Define the output files and directories.

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])

dnl Version information for libcp.la.

[libcp_la_version_info]=LIBCP_VERSION
AC_SUBST([libcp_la_version_info])

dnl Check for C compiler, archiver, and install program.

AC_PROG_CC
AM_PROG_AR
AC_PROG_INSTALL

dnl Initialize libtool, which should be done
dnl after checking archiver.

LT_INIT

AC_OUTPUT
