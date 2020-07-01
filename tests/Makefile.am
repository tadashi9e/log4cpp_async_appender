TESTS = testCategory testFixedContextCategory testNDC testPattern \
	testErrorCollision testPriority testFilter testProperties \
	testConfig testPropertyConfig testRollingFileAppender testDailyRollingFileAppender

check_PROGRAMS = $(TESTS)
check_DATA = log4cpp.init log4cpp.properties testProperties.properties \
	testConfig.log4cpp.properties

EXTRA_DIST = log4cpp.init log4cpp.properties testProperties.properties \
	testConfig.log4cpp.properties \
	testNTEventLog.cpp

noinst_PROGRAMS = testmain testbench

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src

testmain_SOURCES = testmain.cpp
testmain_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testCategory_SOURCES = testCategory.cpp
testCategory_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testFixedContextCategory_SOURCES = testFixedContextCategory.cpp
testFixedContextCategory_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testNDC_SOURCES = testNDC.cpp
testNDC_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testPattern_SOURCES = testPattern.cpp
testPattern_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testPriority_SOURCES = testPriority.cpp
testPriority_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testConfig_SOURCES = testConfig.cpp
testConfig_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testPropertyConfig_SOURCES = testPropertyConfig.cpp
testPropertyConfig_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testFilter_SOURCES = testFilter.cpp
testFilter_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testProperties_SOURCES = testProperties.cpp
testProperties_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testbench_SOURCES = Clock.cpp Clock.hh testbench.cpp 
testbench_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testErrorCollision_SOURCES = testErrorCollision.cpp
testErrorCollision_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testRollingFileAppender_SOURCES = testRollingFileAppender.cpp
testRollingFileAppender_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

testDailyRollingFileAppender_SOURCES = testDailyRollingFileAppender.cpp
testDailyRollingFileAppender_LDADD = $(top_builddir)/src/liblog4cpp.la -lpthread

distclean-local:
	$(RM) -f *.log
