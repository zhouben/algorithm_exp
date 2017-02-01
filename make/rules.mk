OBJLINK = objlink

.PHONY : all
all: $(OBJLINK) $(EXE_FILE)
pit: $(OBJLINK) $(EXE_FILE)
	@$(EXE_FILE)

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

$(EXE_FILE) : $(OBJS)
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
CLEAN_FILES += $(EXE_FILE)

.PHONY: clean
clean:
	@echo [CLEAN]
	@$(RM) $(CLEAN_FILES)
