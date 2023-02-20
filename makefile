include cfg.mk

# All target
all: $(OUT)
	@echo Build was completed succesfully!

# Out target
$(OUT): $(OBJECTS)
	@echo Generating "$<...
	@echo Linking object files...
	$(CC) $(LFLAGS) $? -o $(OUT)

# Objects
%.o : %.c
	@echo "Building source file "$<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	@$(RM) $(OBJECTS)
	@$(RM) build\$(MAP)
	@$(RM) build\$(OUT)

debug: all
	$(GDB) $(DEVICE).out
