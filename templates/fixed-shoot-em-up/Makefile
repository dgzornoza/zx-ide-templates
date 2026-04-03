# Name of final file (without the extension)
EXEC_OUTPUT=fixed-shoot-em-up

# -----------------------------------------------
# variables for compiler, clib and crt selection
# -----------------------------------------------

# Compiler selection
ifeq ($(COMPILER),)
  COMPILER_FLAG=
else
  COMPILER_FLAG=-compiler=$(COMPILER)
endif

# CLIB selection
ifeq ($(CLIB),)
  CLIB_FLAG=
else
  CLIB_FLAG=-clib=$(CLIB)
endif

# CRT startup file
ifeq ($(CRT),)
  CRT_FLAG=
else
  CRT_FLAG=-startup=$(CRT)
endif

# create TAP file or not
CREATE_TAP ?= false

# -----------------------------------------------
# # Get .c and .asm source files and convert to .o object files to compile
# -----------------------------------------------
OBJ_DIR=obj
SOURCES = $(shell find src \( -name '*.c' -o -name '*.asm' \))
OBJECT_SOURCES = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(patsubst src/%.asm,$(OBJ_DIR)/%.o,$(SOURCES)))

# -----------------------------------------------
# constants for z88dk compiler
# -----------------------------------------------
TOOLCHAIN=zcc
TARGET=+zx
VERBOSITY=-vn #(-v: show verbose, -vn: not show verbose)
OUT_DIR=bin
PRAGMA_FILE=zpragma.inc

# Flags
CFLAGS=$(TARGET) $(VERBOSITY) -c $(C_OPT_FLAGS) $(COMPILER_FLAG) $(CLIB_FLAG) --list --c-code-in-asm -pragma-include:$(PRAGMA_FILE)
LDFLAGS=$(TARGET) $(VERBOSITY) $(CLIB_FLAG) $(LINKER_FLAGS) -pragma-include:$(PRAGMA_FILE)
ASFLAGS=$(TARGET) $(VERBOSITY) -c --list

EXEC=$(OUT_DIR)/$(EXEC_OUTPUT)

# -----------------------------------------------
# create object files from source files
# -----------------------------------------------
$(OBJ_DIR)/%.o: src/%.c $(PRAGMA_FILE)
	@mkdir -p $(dir $@)
	$(TOOLCHAIN) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: src/%.asm
	@mkdir -p $(dir $@)
	$(TOOLCHAIN) $(ASFLAGS) -o $@ $<

# -----------------------------------------------
# rule for building the project
# -----------------------------------------------
all : $(EXEC) join_lis_files
	@echo "Total build completed"

# -----------------------------------------------
# rule for compile and create executable
# -----------------------------------------------
$(EXEC) : $(OBJECT_SOURCES)
	@mkdir -p $(OUT_DIR)
	@echo "\nCompiling ..."
	@echo "CREATE_TAP flag: '$(CREATE_TAP)'"
	@echo "Compiler flag: '$(COMPILER_FLAG)'"

ifeq ($(CREATE_TAP),true)
	@echo "\nGenerating .tap file..."
	$(TOOLCHAIN) $(LDFLAGS) $(CRT_FLAG) $(OBJECT_SOURCES) -o $(OUT_DIR)/$(EXEC_OUTPUT).bin -create-app
	@echo ".tap file generated: $(OUT_DIR)/$(EXEC_OUTPUT).tap"
endif

    # create SNA file always for debug (is better supported in all emulators)
	@echo "\nGenerating .sna file..."
	$(TOOLCHAIN) $(LDFLAGS) $(CRT_FLAG) $(OBJECT_SOURCES) -m -s -o $(OUT_DIR)/$(EXEC_OUTPUT).bin -create-app -Cz"--sna"
	@echo ".sna file generated: $(OUT_DIR)/$(EXEC_OUTPUT).sna"

	@echo "\nCompilation finished"
	@echo

# -----------------------------------------------
# rule for join lis files for easy debug asm in all project
# -----------------------------------------------
.PHONY: join_lis_files
join_lis_files:
	$(eval LIS_FILES = $(shell find . \( -name '*.c.lis' -o -name '*.asm.lis' \)))
	@if [ -n "$(LIS_FILES)" ]; then \
		echo "Join .lis files to $(OUT_DIR)/$(EXEC_OUTPUT).source.lis"; \
		cat $(LIS_FILES) > $(OUT_DIR)/$(EXEC_OUTPUT).source.lis; \
		echo; \
	fi

# -----------------------------------------------
# rule for clean all (clean build and obj folders)
# -----------------------------------------------
.PHONY: clean_all
clean_all:
	@echo "Cleaning workspace ..."
	rm -rf $(OUT_DIR) /tmp/tmpXX*
	rm -rf $(OBJ_DIR)
	@echo "Total clean completed"

