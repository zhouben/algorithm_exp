BINDIR = $(TOPDIR)/bin
OBJDIR = $(TOPDIR)/obj
OBJLINK = objlink
EXE_FILE = $(BINDIR)/$(PROJECT)

.PHONY : all
all: $(OBJLINK) $(EXE_FILE)
pit: $(OBJLINK) $(EXE_FILE)
	@$(EXE_FILE)

OBJS = $(MODULE_CFILES:%.c=$(OBJDIR)/%.o)
OBJS += $(LOCAL_CFILES:%.c=$(OBJDIR)/$(PROJECT)/%.o)
OBJS += $(TEST_CFILES:%.c=$(OBJDIR)/%.o)

CFLAGS += $(INCPATHS)

.PHONY : $(OBJLINK)
$(OBJLINK):
	@$(MKDIR) $(BINDIR)
	@$(MKDIR) $(OBJDIR)
	@$(MKDIR) $(OBJDIR)/$(PROJECT)

$(EXE_FILE) : $(OBJS)
	@echo "CC $@"
	@$(CC) -o $@ $^
	@$(LN) $@ $(PROJECT)

$(OBJDIR)/$(PROJECT)/%.o:: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o:: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS)  -c -o $@ $<

$(OBJDIR)/%.o:: $(TESTSDIR)/%.c
	@$(CC) $(CFLAGS)  -c -o $@ $<

CLEAN_FILES = $(OBJDIR)
CLEAN_FILES += $(BINDIR)
CLEAN_FILES += $(PROJECT)
CLEAN_FILES += $(EXE_FILE)

.PHONY: clean
clean:
	@echo [CLEAN]
	@$(RM) $(CLEAN_FILES)
	
FORCE:
	@ture
