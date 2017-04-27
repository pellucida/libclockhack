

LIBSRC	= hack_time.c hack_gettimeofday.c hack_clock_gettime.c hack_init.c
OBJ	= hack_time.o hack_gettimeofday.o hack_clock_gettime.o hack_init.o
SO	= libclockhack.so.0

# Linux
CFLAGS	= -fPIC
LDFLAGS	= -shared -Wl,-soname,$(SO)
LDLIBS	= -ldl


TESTPROG= testprog
TESTOBJ	= testprog.o

# Needed for clock_gettime()
TESTLIBS	= -lrt
TEST_CFLAGS	= -c 

all:	$(SO) $(TESTPROG)

#$(OBJ):	$(LIBSRC)
#	$(CC) -c $(SO_CFLAGS) $(LIBSRC) -o $(OBJ)

$(SO):	$(OBJ)
	$(CC) $(LDFLAGS) -o $(SO) $(OBJ) $(LDLIBS)

TESTOBJ	= testprog.o
$(TESTOBJ):	$(TESTPROG).c
	$(CC) $(TEST_CFLAGS) $^

$(TESTPROG):	$(TESTOBJ)
	$(CC) -o $(TESTPROG) $(TESTOBJ) $(TESTLIBS)

test:
	@echo ""
	@echo NO PRELOAD
	@$(PWD)/$(TESTPROG)
	@echo ""
	@echo ""
	@echo LD_PRELOAD=$$\(PWD\)/$(SO) HACK_CLOCK_SHIFT=-100000000
	@env LD_PRELOAD=$(PWD)/$(SO) HACK_CLOCK_SHIFT=-100000000 $(PWD)/$(TESTPROG)
	@echo ""
	@echo LD_PRELOAD with /bin/date
	@env LD_PRELOAD=$(PWD)/$(SO) HACK_CLOCK_SHIFT=-100000000 date

clean:
	rm -f $(TESTOBJ) $(OBJ) core.* a.out

distclean:
	rm -f $(SO) $(TESTPROG) $(TESTOBJ) $(OBJ) core.* a.out
