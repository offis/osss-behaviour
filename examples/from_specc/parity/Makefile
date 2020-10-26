# Copyright (c) 2008 OFFIS e.V., Oldenburg, Germany
# Some rights reserved.
# -------------------------------------------------------------------
# Example Makefile for OSSS simulation library
# -------------------------------------------------------------------
#
# This file is part of the OSSS simulation library.
# Copyright (c) 2008 OFFIS e.V. Institute for Information Technology
#
# See the files COPYRIGHT and LICENSE included with the OSSS library
# for copyright and license details. If you have not received these
# files, please contact OFFIS e.V. for further information.
#
# OFFIS e.V.
# Institute for Information Technology
#   Escherweg 2
#   D-26121 Oldenburg
#   Germany
# www  : http://offis.de
# phone: +49 (441) 9722-0
# fax  : +49 (441) 9722-102
# -------------------------------------------------------------------
#
# Author(s): Philipp A. Hartmann, Kim Gruettner
#
# -------------------------------------------------------------------

#
# This is an example Makefile for an OSSS application.
# It uses the generic OSSS Makefile, named Makefile.osss
# in the top-level example directory.
#

#
# Configuration section of this example/benchmark application
# (if a variable is omitted here, a default value is used)
#

# Name of the application (required!)
MODULE:= parity

# Default parameters to pass to application, if invoked by
# "simulate" target (optional).
#ARGS:=42

# Target architecture
#TARGET_ARCH := $(shell uname -s | tr '[:upper:]' '[:lower:]' )

# SystemC installation (required)
#
# The variable SYSTEMC_HOME is required to determine the
# location of the SystemC installation. If set in the
# environment, this setting is used. If there is a configuration
# file config.mk in the OSSS library directory, the setting
# is inherited from there.
#SYSTEMC_HOME := /path/to/systemc/directory

# The variable SYSTEMC_LIB is required to locate the static
# library file libsystemc.a, matching the current commpiler
# and SystemC headers.
#SYSTEMC_LIB := $(SYSTEMC_HOME)/lib-$(TARGET_ARCH)

#
# Source files of the current application
#

# source file extension
#
# Set the extension of C++ source files.
# Defaults to "<name>.cpp"
SRCEXT := cpp

# The actual source code files can be specified via a list of
# source directories (in the variable SRCDIRS) OR as a list of
# .cpp-files (in the variable SRCS). If set, the variable SRCS takes
# precedence, otherwise, all .cpp-files in SRCDIRS are taken as
# input files (see also: $(SRCEXT) above).
#
# Source-Directories
SRCDIRS := . ../../../src
# Source files
#SRCS := example1.cpp example2.cpp

#
# Various settings
#

# Verbose compilation output? (yes,no)
#VERBOSE=no

# Include debug symbols? (yes,no)
DEBUG=yes

# Include profiling information (yes,no)
#PROFILE=no

# Use different OSSS library flavour to link against
# Only required, if you use a specific flavoured
# version of the OSSS library.
#OSSS_LIBNAME := osss

# set additional defines
#
# This variable can hold additional command-line defines, that
# should be passed to the compiler e.g "-DSC_INCLUDE_FX=1".
#EXTRA_DEFINES :=

# additional include directories
EXTRA_INCLUDES := -I. -I../../../src

# additional library directories and link directives
#EXTRA_LIBDIRS :=
#EXTRA_LIBS    :=

# Compiler to use
#
# Inherits setting of simulation library config, if possible.
# Otherwise defaults to g++.
#OSSS_CXX := $(CXX)

#
# Define the minimum version of GNU C++ compiler, if necessary.
# Defaults to OSSS minimum requirement of GNU C++ 3.4.4
#
# Specify version as triplet of <MAJOR><MINOR><PATCHLEVEL>,
# each as two digits. Requiring at least g++ 4.0 means
#OSSS_MIN_CXX_VERSION := 040000
#

# Compiler settings
#
#
# The following variables are respected during the compilation
# process. Defaults are below.
# Note:
#    CXXFLAGS_DEBUG is only added, iff $(DEBUG)  = yes
#    CXXFLAGS_OPT   is only added, iff $(DEBUG) != yes
#
#OSSS_CXXFLAGS       := -Wall
#
#OSSS_CXXFLAGS_OPT   := -O3
#OSSS_CXXFLAGS_DEBUG := -g -DDEBUG=1 -DTRACE=1

# additional target to clean up current application
#
# This target is automatically invoked, if 'clean'
# is called, see below.
EXTRA_CLEAN=extra-clean

# look for common build rules, if not set
ifeq (,$(strip $(OSSS_BUILD_RULES)))
OSSS_BUILD_RULES := ../../../build/Makefile.osss
endif
# include generic OSSS Makefile
ifneq (,$(wildcard $(OSSS_BUILD_RULES)))
include $(OSSS_BUILD_RULES)
else
$(error Common OSSS Makefile '$(OSSS_BUILD_RULES)' not found! Bailing out)
endif

# -----------------------------------------------------------------------
#    local targets below
# -----------------------------------------------------------------------

# see $(EXTRA_CLEAN)
extra-clean:
	$(call cmd-delete,$(wildcard *.awif *.vcd *.dot))

# TAF!
