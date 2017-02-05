OBJLINK = objlink
PIT_TARGETS = pit

.PHONY : all pit
all: | $(OBJLINK)

ifneq ($(PROJECT),)
all: $(BINDIR)/$(PROJECT)
endif

all: $(SUBDIRS)

$(SUBDIRS): FORCE
	@+$(MAKE) -C $@

$(PIT_TARGETS): all $(foreach dir,$(SUBDIRS),$(PIT_TARGETS)-$(dir))
	@(test -n '$(PROJECT)' && (echo to run $(PROJECT) ; ./$(PROJECT))) || (echo "this's top dir" && /bin/true)

$(PIT_TARGETS)-%: all
	@echo to run subdir pit
	@$(MAKE) -C $(subst $(PIT_TARGETS)-,,$@) $(PIT_TARGETS)


OBJS = $(MODULE_CFILES:%.c=$(OBJDIR)/%.o)
OBJS += $(LOCAL_CFILES:%.c=$(OBJDIR)/$(PROJECT)/%.o)
OBJS += $(TEST_CFILES:%.c=$(OBJDIR)/%.o)

-include $(OBJS:.o=.d)

CFLAGS += $(INCPATHS)

define GENERATE_DEP
@- test -d $(@D) || $(MKDIR) -p $(@D)
@$(CC) -MM $(CFLAGS) $< | sed 's#\w\+\.o#$(@:.d=.o)#' > $@
endef

$(OBJDIR)/$(PROJECT)/%.d: %.c
	$(GENERATE_DEP)

$(OBJDIR)/%.d: %.c
	$(GENERATE_DEP)

.PHONY : $(OBJLINK)
$(OBJLINK):
	@$(MKDIR) $(BINDIR)
	@$(MKDIR) $(OBJDIR)
	@$(MKDIR) $(OBJDIR)/$(PROJECT)

$(BINDIR)/$(PROJECT) : $(OBJS)
	@echo "curr path: $(shell basename `pwd`)"
	@echo "CC $@"
	@$(CC) -o $@ $^
	@$(LN) $@ $(PROJECT)

define COMPILE_C
@echo compile $@  $^
@$(CC) $(CFLAGS)  -c -o $@ $<
endef

$(OBJDIR)/$(PROJECT)/%.o:: %.c
	$(COMPILE_C)

$(OBJDIR)/%.o: %.c
	$(COMPILE_C)

CLEAN_FILES = $(OBJDIR)
CLEAN_FILES += $(BINDIR)
CLEAN_FILES += $(PROJECT)

.PHONY: FORCE
FORCE:
	@/bin/true

.PHONY: clean
clean:
	@echo [CLEAN]
	@$(RM) $(CLEAN_FILES)
