noinst_PROGRAMS = test-bidi-bracketing test-character test test-bracketing

test_bidi_bracketing_SOURCES = test-bidi-bracketing.c
test_character_SOURCES = test-character.c
test_SOURCES = test.c
test_bracketing_SOURCES = test-bracketing.c

AM_CPPFLAGS = \
		-I$(top_builddir)/lib \
		-I$(top_srcdir)/lib \
		-I$(top_srcdir)/charset \
		$(MISC_CFLAGS)

-include $(top_srcdir)/git.mk
LDADD = $(top_builddir)/lib/libfribidi.la $(MISC_LIBS)

