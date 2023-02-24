include cfg.mk

test:
	@echo $(BUILD_DIRS)
	
# All target
all: $(OUT)
	@echo Build was completed succesfully!

# Out target
$(OUT): $(OBJECTS)
	@echo Linking object files...
	$(CC) $(LFLAGS) -o $(OUT) $(OBJECTS_QUOTED) $(LINKER_FILE) $(LD_LIBS)

# Objects
$(BUILD_DIR)/%.obj: %.c | $(BUILD_DIRS)
	@echo Building source file $<... 
	$(CC) $(CFLAGS) "$<"

$(BUILD_DIR)/%/:
	@$(MKDIR) $@

clean: 
	@echo Cleaning project..
	@$(RM) $(RAWS)
	@$(RM) $(OBJECTS)
	@$(RM) $(MAP)
	@$(RM) $(XML)
	@$(RM) $(WARNING_FILE)
	@$(RM) $(OUT)
