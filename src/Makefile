

LIBSRC	= libclockhack.c
OBJ	= libclockhack.o
SO	= libclockhack.so
# Linux
SO_CFLAGS	= -fPIC
LDFLAGS	= -shared -Wl,-soname,$(SO)
LDLIBS	= -ldl
# NetBSD
# SO_CFLAGS	= -fPIC -DGETTIMEOFDAY50
# LDLIBS	=

TESTPROG= testprog
TESTOBJ	= testprog.o
# Needed for clock_gettime()
TESTLIBS	= -lrt

all:	$(SO) $(TESTPROG)

$(OBJ):	$(LIBSRC)
	$(CC) -c $(SO_CFLAGS) $(LIBSRC) -o $(OBJ)

$(SO):	$(OBJ)
	$(CC) $(LDFLAGS) -o $(SO) $(OBJ) $(LDLIBS)

TESTOBJ	= testprog.o
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
