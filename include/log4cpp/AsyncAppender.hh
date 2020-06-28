#ifndef _LOG4CPP_ASYNCAPPENDER_HH
#define _LOG4CPP_ASYNCAPPENDER_HH

#include <log4cpp/Appender.hh>

#include <memory>
#include <queue>
#include <boost/thread.hpp>

namespace log4cpp {

template<typename APPENDER>
class LOG4CPP_EXPORT AsyncAppender : public Appender {
 public:
  /**
     Constructs a AsyncAppender.
     @param name the name of AsyncAppender.
     @param appender wrapped appender.
   **/
  AsyncAppender(const std::string& name, APPENDER appender)
    : Appender(name), _appender(appender), _closed(false), _stop(false),
    _thread(boost::bind(&AsyncAppender::_work, this)) {
  }
  virtual ~AsyncAppender() {
    _stop_worker();
    _delete(_appender);
  }
  /**
   * Log in wrapped appender asynchronously.
   **/
  virtual void doAppend(const LoggingEvent& event) {
    boost::mutex::scoped_lock lock(_mutex);
    if (_closed) {
      return;
    }
    _queue.push(event);
    _cv.notify_one();
  }
  // ----------------------------------------------------------------------
  // Wrapper methods
  // ----------------------------------------------------------------------
  virtual bool reopen() {
    boost::mutex::scoped_lock lock(_mutex);
    _closed = false;
    return _appender->reopen();
  }
  virtual void close() {
    boost::mutex::scoped_lock lock(_mutex);
    // -- stop doAppend
    _closed = true;
    // -- wait for flush
    while (!_queue.empty()) {
      _cv_empty.wait(lock);
    }
    // -- close wrapped appender
    _appender->close();
  }
  virtual bool requiresLayout() const {
    return _appender->requiresLayout();
  }
  virtual void setLayout(Layout* layout) {
    _appender->setLayout(layout);
  }
  virtual void setThreshold(Priority::Value priority) {
    _appender->setThreshold(priority);
  }
  virtual Priority::Value getThreshold() {
    return _appender->getThreshold();
  }
  virtual void setFilter(Filter* filter) {
    _appender->setFilter(filter);
  }
  virtual Filter* getFilter() {
    return _appender->getFilter();
  }

 private:
  template<typename T> void _delete(T& appender) {
  }
  template<typename T> void _delete(T* appender) {
    delete appender;
  }
  APPENDER _appender;
  boost::mutex _mutex;
  boost::condition_variable _cv;
  boost::condition_variable _cv_empty;
  std::queue<LoggingEvent> _queue;
  bool _closed;
  bool _stop;
  boost::thread _thread;

  /**
     Worker thread main loop.
   **/
  void _work() {
    boost::mutex::scoped_lock lock(_mutex);
    for (;;) {
      while (_queue.empty()) {
        if (_stop) {
          return;
        }
        _cv.wait(lock);
      }
      while (!_queue.empty()) {
        LoggingEvent event = _queue.front();
        _queue.pop();
        lock.unlock();
        try {
          _appender->doAppend(event);
        } catch(...) {
        }
        lock.lock();
      }
      _cv_empty.notify_one();
    }
  }
  /**
     Stop worker thread.
   **/
  void _stop_worker() {
    {
      boost::mutex::scoped_lock lock(_mutex);
      _stop = true;
      _cv.notify_one();
    }
    _thread.join();
  }
};

}  // namespace log4cpp
#endif  // _LOG4CPP_ASYNCAPPENDER_HH
