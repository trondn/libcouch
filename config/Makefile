#
# Generic makefile for building libcouch.
# I know it may be tempting to try to use more advanced make
# features like substituting .c with .o etc, but try to keep it
# as simple as possible to make it work with GNU make, NMake,
# dmake and others
#
$(BUILDDIR) $(BUILDDIR)/bin $(BUILDDIR)/$(LIBDIR) $(BUILDDIR)/$(OBJDIR) $(BUILDDIR)/$(TESTDIR):; -@mkdir -p $@

INCLUDE_PATH=-Iinclude -I$(PREFIX)/include
LIBRARY_CFLAGS=-DLIBCOUCH_INTERNAL=1
LIBCOUCH_OBJS=$(BUILDDIR)/$(OBJDIR)/document.$(OBJ) \
              $(BUILDDIR)/$(OBJDIR)/instance.$(OBJ) \
              $(BUILDDIR)/$(OBJDIR)/error.$(OBJ)

HEADERDEPS=include/libcouch/couch.h \
           include/libcouch/types.h \
           include/libcouch/visibility.h \
           src/internal.h

$(BUILDDIR)/$(LIBDIR)/libcouch.so: $(BUILDDIR)/$(LIBDIR) $(LIBCOUCH_OBJS)
	$(SO_LINK) -o $(BUILDDIR)/$(LIBDIR)/libcouch.so $(LIBCOUCH_OBJS) \
                   -L$(PREFIX)/lib -lcouchstore

$(BUILDDIR)/$(OBJDIR)/instance.$(OBJ): $(BUILDDIR)/$(OBJDIR) src/instance.c \
                                       $(HEADERDEPS)
	$(SO_COMPILE) $(LIBRARY_FLAGS) \
                     -o $(BUILDDIR)/$(OBJDIR)/instance.$(OBJ) \
                     src/instance.c

$(BUILDDIR)/$(OBJDIR)/document.$(OBJ): $(BUILDDIR)/$(OBJDIR) src/document.c \
                                       $(HEADERDEPS)
	$(SO_COMPILE) $(LIBRARY_FLAGS) \
                     -o $(BUILDDIR)/$(OBJDIR)/document.$(OBJ) \
                     src/document.c

$(BUILDDIR)/$(OBJDIR)/error.$(OBJ): $(BUILDDIR)/$(OBJDIR) src/error.c \
                                    $(HEADERDEPS)
	$(SO_COMPILE) $(LIBRARY_FLAGS) \
                     -o $(BUILDDIR)/$(OBJDIR)/error.$(OBJ) \
                     src/error.c

$(BUILDDIR)/$(TESTDIR)/testapp$(E): $(BUILDDIR)/$(TESTDIR) \
                                    $(BUILDDIR)/$(OBJDIR)/testapp.$(OBJ) \
                                    $(BUILDDIR)/$(LIBDIR)/libcouch.so
	$(EXE_LINK) -o $(BUILDDIR)/$(TESTDIR)/testapp$(E) \
                    $(BUILDDIR)/$(OBJDIR)/testapp.$(OBJ) \
                    -L$(BUILDDIR)/$(LIBDIR) -lcouch \
                    -L$(PREFIX)/lib -lcouchstore

$(BUILDDIR)/$(OBJDIR)/testapp.$(OBJ): $(BUILDDIR)/$(OBJDIR) tests/testapp.c
	$(COMPILE) $(TEST_FLAGS) -o $(BUILDDIR)/$(OBJDIR)/testapp.$(OBJ) \
                   tests/testapp.c

all: $(BUILDDIR)/$(LIBDIR)/libcouch.so

test: $(BUILDDIR)/$(TESTDIR)/testapp$(E)
	@$(BUILDDIR)/$(TESTDIR)/testapp$(E)

clean:
	$(RM) $(LIBCOUCH_OBJS) \
              $(BUILDDIR)/$(LIBDIR)/libcouch.so \
              $(BUILDDIR)/$(TESTDIR)/testapp$(E) \
              $(BUILDDIR)/$(OBJDIR)/testapp.$(OBJ)
