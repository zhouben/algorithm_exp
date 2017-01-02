MAKE = make
SUBDIRS := $(shell find . -maxdepth 1 -type d)
SUBDIRS := $(basename $(patsubst ./%, %, $(SUBDIRS)))

DISABLED_APPS = smoke_test

SUBDIRS := $(filter-out $(DISABLED_APPS), $(SUBDIRS))

.PHONY : all $(SUBDIRS) clean
all : $(SUBDIRS)
	@echo to compile all subdirectories
	@echo $(SUBDIRS)

$(SUBDIRS):
	@+$(MAKE) -C $@

clean:
	@echo [CLEAN]
	@rm -rf ../bin ../obj/
