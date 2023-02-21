include cfg.mk

# All target
all: $(OUT)
	@echo Build was completed succesfully!

# Out target
$(OUT): $(OBJECTS)
	@echo Generating $<...
	@echo Linking object files...
	$(CC) $(LFLAGS) $? -o $(OUT)

# Objects
%.o : %.c
	@echo Building source file $<...
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	@echo Cleaning project...
	@$(RM) $(OBJECTS)
	@$(RM) $(MAP)
	@$(RM) $(WARNING_FILE)
	@$(RM) $(OUT)

flash: $(OUT)
	mspdebug tilib "prog $(OUT)"

debug: all
	$(GDB) $(OUT)
