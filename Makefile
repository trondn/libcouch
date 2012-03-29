#
# To build the stuff, you should try building it like:
#
# make -e TARGET=sunos \
#         CFLAGS=-m64 \
#         LDFLAGS=-m64 \
#         LIBDIR=lib/amd64 \
#         ORIGINREL=../.. \
#         TESTDIR=test/amd64 \
#         PREFIX=../install \
#         test
#
PREFIX=/usr

all clean test:
	@$(MAKE) -f config/Makefile.$(TARGET) $(MAKEFLAGS) PREFIX="$(PREFIX)" $@
