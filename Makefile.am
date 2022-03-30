ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libccg.la

HFILES =\
  src/mem.h      \
  src/adt.h      \
  src/list.h     \
  src/dict.h     \
  src/pqueue.h     

CFILES =\
  src/mem.c      \
  src/list.c     \
  src/dict.c     \
  src/pqueue.c

libccg_la_SOURCES = $(HFILES) $(CFILES)

TFILES =\
  test/libccg_test.h    \
  test/libccg_test.c    \
  test/list_test.c      \
  test/dict_test.c      \
  test/pqueue_test.c

TESTS = libccg_test
check_PROGRAMS = libccg_test
libccg_test_SOURCES = $(HFILES) $(TFILES)
libccg_test_CFLAGS = @CHECK_CFLAGS@
libccg_test_LDADD = libccg.la @CHECK_LIBS@

noinst_PROGRAMS = libccg_test