# Copyright (c) 2008 OFFIS e.V., Oldenburg, Germany
# Some rights reserved.
# -------------------------------------------------------------------
# Top-level Makefile for the OSSS library designs
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
# Author(s): Philipp A. Hartmann
#
# -------------------------------------------------------------------

#
# This Makefile can be used to build all designs recursively.
# Additionally, every sub-directory contains a similar Makefile,
# that can be used to build the designs in this particular
# sub-directory only.
#
# The available targets are:
#
# build : compile all designs (default)
# sim   : compile and simulate all designs
# clean : clean all designs
#
# To exclude a certain (set of) designs, you can put
# a list of sub-directories, that should be excluded
# from the traversal into a file named "exclude.dirs"
# into any of the designs' sub-directories. The listed
# directories must be below the directory in which the
# exclude file is located in.
#
# Invoking gmake(1) in an excluded directory builds the
# requested target anyway. Only the descending into these
# directories from a higher level is suppressed.
#
# Empty lines, or lines starting with "#" are ignored, any
# other line in these files is treated as a directory name,
# that should be excluded. Wildcards are not allowed.
#
# Example of an exclude file:
# # --- begin of exclude.dirs ---
#
# # exclude amba_bus, when calling make from the top-level
# # Note: Not having an exclude.dirs in ./osss_channel_examples
# #       would build "amba_bus", when calling make(1) in this
# #       directory directly
# osss_channel_examples/amba_bus
#
# # --- end of exclude.dirs ---
#
# Additionally, non-source directories can be excluded directly from
# the traversal Makefiles by setting the variable OSSS_XTRA_EXCLUDE
# to a list of those local directories, that are to be ignored.
# Example:
# OSSS_XTRA_EXCLUDE:=images

# -------------------------------------------------------------------

# disable implicit rules and automatic variables
MAKEFLAGS += -rR
MAKEFLAGS += --no-print-directory

# we've been included from a sub-directory
ifneq (,$(OSSS_SUB_MAKE))

# do not modify directory names
strip-dir   = $(1)
unstrip-dir = $(1)

else # we've been called directly

# set required variables
OSSS_CURDIR  := $(CURDIR)
OSSS_BASEDIR := $(CURDIR)

# beautify make targets at top level
# (remove osss_* from directory name)
strip-dir   = $(patsubst  osss_%,%,$(1))
unstrip-dir = $(addprefix osss_,$(1))

endif # preparation (sub-make, or called directly)

# build rules file
OSSS_BUILD_RULES := $(OSSS_CURDIR)/build/Makefile.osss
export OSSS_BUILD_RULES

# find design sub-directories
SUBDIRS   := $(sort $(shell find $(OSSS_BASEDIR) -mindepth 1 -maxdepth 1 \
                            -name .svn -prune -o -type d -printf '%f '  ))

# set names of design directories
OSSS_EXAMPLE_SETS   := $(call strip-dir,$(SUBDIRS))

# the different targets - build-*, sim-* and clean-*
OSSS_BUILD_EXAMPLES := $(addprefix build-,$(OSSS_EXAMPLE_SETS))
OSSS_SIMUL_EXAMPLES := $(addprefix sim-,$(OSSS_EXAMPLE_SETS))
OSSS_CLEAN_EXAMPLES := $(addprefix clean-,$(OSSS_EXAMPLE_SETS))

#
# common targets
#

# default target - build all designs
build: $(OSSS_BUILD_EXAMPLES) ;
PHONY += build

# simulation target - recursively simulate all designs
sim: $(OSSS_SIMUL_EXAMPLES) ;
PHONY += sim

# cleanup target - recursively clean designs
clean: $(OSSS_CLEAN_EXAMPLES) ;
PHONY += clean

# recursive build target
$(OSSS_BUILD_EXAMPLES): build-%:
	  $(call cmd-make,build,$(call unstrip-dir,$(*)))
PHONY += $(OSSS_BUILD_EXAMPLES)

# recursive simulation target
$(OSSS_SIMUL_EXAMPLES): sim-%:
	  $(call cmd-make,sim,$(call unstrip-dir,$(*)))
PHONY += $(OSSS_SIMUL_EXAMPLES)

# recursive clean target
$(OSSS_CLEAN_EXAMPLES): clean-%:
	  $(call cmd-make,clean,$(call unstrip-dir,$(*)))
PHONY += $(OSSS_CLEAN_EXAMPLES)

# recursive make call
P ?= .
define cmd-make
	$(if $(call filter-directory,$(P)/$(2)),\
	  @echo "*** Building target '$(1)' (directory: $(P)/$(2))"
	  +@$(MAKE) P=$(P)/$(2) -C $(2) $(1),\
	  @echo "--- Skipping target '$(1)' (directory: $(P)/$(2))")
endef

#
# filter directories (see documentation above)
#

# only consider those, that are on our path
OSSS_EXCLUDE := $(filter $(P)/%,$(OSSS_EXCLUDE))

# check for exlusion lists
OSSS_EXCLUDE_LIST := $(OSSS_BASEDIR)/exclude.dirs
ifneq (,$(wildcard $(OSSS_EXCLUDE_LIST)))
# add more excludes
HASH=\#
OSSS_EXCLUDE += $(addprefix $(P)/,$(shell cat "$(OSSS_EXCLUDE_LIST)" | \
                          sed -e '/^\($(HASH)\|[ 	]*$$\)/d' \
                              -e '/^\.\.\//d' \
                              -e 's/^\.\///' ) )
endif
ifneq (,$(strip $(OSSS_XTRA_EXCLUDE)))
OSSS_EXCLUDE += $(wildcard $(addprefix $(P)/,$(OSSS_XTRA_EXCLUDE)))
endif
# remove duplicates
OSSS_EXCLUDE := $(sort $(OSSS_EXCLUDE))
# export variable to sub-makes
export OSSS_EXCLUDE

# filter out certain directories
filter-directory = $(strip $(filter-out $(OSSS_EXCLUDE),$(1)))

.PHONY: $(PHONY)

