TOPDIR = ..
SUBDIRS := $(shell find . -maxdepth 1 -type d)
SUBDIRS := $(basename $(patsubst ./%, %, $(SUBDIRS)))

DISABLED_APPS = smoke_test

SUBDIRS := $(filter-out $(DISABLED_APPS), $(SUBDIRS))

include $(TOPDIR)/make/top.mk
include $(TOPDIR)/make/rules.mk
