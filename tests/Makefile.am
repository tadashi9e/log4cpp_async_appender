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
testmain_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testCategory_SOURCES = testCategory.cpp
testCategory_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testFixedContextCategory_SOURCES = testFixedContextCategory.cpp
testFixedContextCategory_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testNDC_SOURCES = testNDC.cpp
testNDC_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testPattern_SOURCES = testPattern.cpp
testPattern_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testPriority_SOURCES = testPriority.cpp
testPriority_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testConfig_SOURCES = testConfig.cpp
testConfig_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testPropertyConfig_SOURCES = testPropertyConfig.cpp
testPropertyConfig_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testFilter_SOURCES = testFilter.cpp
testFilter_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testProperties_SOURCES = testProperties.cpp
testProperties_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testbench_SOURCES = Clock.cpp Clock.hh testbench.cpp 
testbench_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testErrorCollision_SOURCES = testErrorCollision.cpp
testErrorCollision_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testRollingFileAppender_SOURCES = testRollingFileAppender.cpp
testRollingFileAppender_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

testDailyRollingFileAppender_SOURCES = testDailyRollingFileAppender.cpp
testDailyRollingFileAppender_LDADD = $(top_builddir)/src/liblog4cpp.la -lboost_thread

distclean-local:
	$(RM) -f *.log
