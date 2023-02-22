include cfg.mk

test:
	@echo $(OBJECTS)
	@echo $(OBJECTS_QUOTED)
# All target
all: $(OUT)
	@echo Build was completed succesfully!

# Out target
$(OUT): $(OBJECTS)
	@echo Generating $<...
	@echo Linking object files...
	$(CC) $(LFLAGS) -o $(OUT) $(OBJECTS_QUOTED) $(LINKER_FILE) $(LD_LIBS)

# Objects
%.obj : %.c
	@echo Building source file $<... 
	$(CC) $(CFLAGS) "$<"

clean: 
	@echo Cleaning project...
	@$(RM) $(subst /,\, $(OBJECTS))
	@$(RM) $(subst /,\, $(RAWS))
	@$(RM) $(subst /,\, $(MAP))
	@$(RM) $(subst /,\, $(XML))
	@$(RM) $(subst /,\, $(WARNING_FILE))
	@$(RM) $(subst /,\, $(OUT))

debug: all
	$(GDB) $(DEVICE).out
