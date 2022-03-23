ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libccg.la

HFILES =\
  src/mem.h     \
  src/map.h     \
  src/pqueue.h     

CFILES =\
  src/mem.c     \
  src/map.c     \
  src/pqueue.c

libccg_la_SOURCES = $(HFILES) $(CFILES)

TESTS = libccg_test
check_PROGRAMS = libccg_test
libccg_test_SOURCES = $(HFILES) test/libccg_test.h test/libccg_test.c test/pqueue_test.c
libccg_test_CFLAGS = @CHECK_CFLAGS@
libccg_test_LDADD = libccg.la @CHECK_LIBS@

noinst_PROGRAMS = libccg_test