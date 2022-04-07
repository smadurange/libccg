ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libccg.la

HFILES =\
	src/mem.h           \
	src/adt/adt.h       \
	src/adt/list.h      \
	src/adt/dict.h      \
	src/adt/pqueue.h    \
	src/point.h         \
	src/polyline.h      \
	src/voronoi.h

CFILES =\
	src/mem.c           \
	src/adt/list.c      \
	src/adt/dict.c      \
	src/adt/pqueue.c    \
	src/point.c         \
	src/polyline.c      \
	src/voronoi.c

libccg_la_SOURCES = $(HFILES) $(CFILES)

TFILES =\
	test/libccg_test.h        \
	test/libccg_test.c        \
	test/adt/list_test.c      \
	test/adt/dict_test.c      \
	test/adt/pqueue_test.c    \
	test/point_test.c

TESTS = libccg_test
check_PROGRAMS = libccg_test
libccg_test_SOURCES = $(HFILES) $(TFILES)
libccg_test_CFLAGS = @CHECK_CFLAGS@
libccg_test_LDADD = libccg.la @CHECK_LIBS@

noinst_PROGRAMS = libccg_test
