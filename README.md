# AsyncAppender for log4cpp

## Dependency

Requires boost thread library.

## Compile

~~~
./autogen.sh
./configure
make
~~~

See log4cpp project website [http://log4cpp.sourceforge.net/](http://log4cpp.sourceforge.net/) for detail.

## Examples

Simple example
--------------

You can wrap any Appender instance by log4cpp::AsyncAppender.
The log4cpp::AsyncAppender appends logs in wrapped appender asynchronously.

~~~c++
#include <log4cpp/AsyncAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Category.hh>

int
main(int argc, char* argv[]) {
  log4cpp::Appender*
    appender = new log4cpp::AsyncAppender<log4cpp::Appender*>(
      "default",
      new log4cpp::FileAppender("default", "/tmp/test.log"));
  log4cpp::Category& logger = log4cpp::Category::getRoot();
  logger.setPriority(log4cpp::Priority::INFO);
  logger.addAppender(appender);
  for (int i = 0; i < 1000; ++i) {
    logger.info("async_appender test message %d", i);
  }
}
~~~

Properties file example
-----------------------

You can use bellow appenders in properties file:

- AsyncFileAppender (asynchronous version of FileAppender)
- AsyncRollingFileAppender (asynchronous version of RollingFileAppender)
- AsyncDailyRollingFileAppender (asynchronous version of DailyRollingFileAppender)

~~~
rootCategory=DEBUG, LOGFILE
category.test=DEBUG, LOGFILE
appender.LOGFILE=AsyncFileAppender
appender.LOGFILE.fileName=/tmp/test.log
appender.LOGFILE.layout=PatternLayout
appender.LOGFILE.layout.ConversionPattern=%d %t %-6p %c - %m%n
~~~

~~~c++
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Category.hh>

int
main(int argc, char* argv[]) {
  log4cpp::PropertyConfigurator::configure("property.conf");
  log4cpp::Category& logger = log4cpp::Category::getInstance("test");
  for (int i = 0; i < 1000; ++i) {
    logger.info("property test message %d", i);
  }
}
~~~

Simple file example
-------------------

You can use bellow appender names in configuration file:

- asyncfile (asynchronous version of 'file')
- asyncrolling (asynchronous version of 'rolling')
- asyncdailyrolling (asynchronous version of 'dailyrolling')

~~~
appender root basic asyncfile /tmp/test.log
~~~

~~~c++
#include <log4cpp/SimpleConfigurator.hh>
#include <log4cpp/Category.hh>

int
main(int argc, char* argv[]) {
  log4cpp::SimpleConfigurator::configure("simple.conf");
  log4cpp::Category& logger = log4cpp::Category::getInstance("test");
  for (int i = 0; i < 1000; ++i) {
    logger.info("simple test message %d", i);
  }
}
~~~
