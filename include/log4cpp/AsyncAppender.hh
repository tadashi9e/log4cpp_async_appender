#ifndef _LOG4CPP_ASYNCAPPENDER_HH
#define _LOG4CPP_ASYNCAPPENDER_HH

#include <log4cpp/Appender.hh>

#include <pthread.h>
#include <memory>
#include <queue>

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
    : Appender(name), _appender(appender), _closed(false), _stop(false) {
    int err;
    err = pthread_mutex_init(&_mutex, NULL);
    if (err) {
      _report_error("pthread_mutex_init", errno);
    }
    err = pthread_cond_init(&_cv, NULL);
    if (err) {
      _report_error("pthread_cond_init", errno);
    }
    err = pthread_cond_init(&_cv_empty, NULL);
    if (err) {
      _report_error("pthread_cond_init", errno);
    }
    err = pthread_create(&_thread, NULL, &_call_work, this);
    if (err) {
      _report_error("pthread_cond_init", errno);
    }
  }
  virtual ~AsyncAppender() {
    _stop_worker();
    _delete(_appender);
  }
  /**
   * Log in wrapped appender asynchronously.
   **/
  virtual void doAppend(const LoggingEvent& event) {
    pthread_mutex_lock(&_mutex);
    try {
      if (!_closed) {
        _queue.push(event);
        pthread_cond_signal(&_cv);
      }
    } catch (...) {
    }
    pthread_mutex_unlock(&_mutex);
  }
  // ----------------------------------------------------------------------
  // Wrapper methods
  // ----------------------------------------------------------------------
  virtual bool reopen() {
    bool b = false;
    pthread_mutex_lock(&_mutex);
    try {
      _closed = false;
      b = _appender->reopen();
    } catch (...) {
    }
    pthread_mutex_unlock(&_mutex);
    return b;
  }
  virtual void close() {
    pthread_mutex_lock(&_mutex);
    try {
      // -- stop doAppend
      _closed = true;
      // -- wait for flush
      while (!_queue.empty()) {
        pthread_cond_signal(&_cv);
      }
      // -- close wrapped appender
      _appender->close();
    } catch (...) {
    }
    pthread_mutex_unlock(&_mutex);
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
  pthread_mutex_t _mutex;
  pthread_cond_t _cv;
  pthread_cond_t _cv_empty;
  std::queue<LoggingEvent> _queue;
  bool _closed;
  bool _stop;
  pthread_t _thread;

  /**
     Report errno as std::runtime_error.
   **/
  void _report_error(const char*msg, int err_no) {
    throw std::runtime_error(std::string(msg) +
                             (err_no == EBUSY ? "EBUSY" :
                              err_no == EINVAL ? "EINVAL" :
                              err_no == EAGAIN ? "EAGAIN" :
                              err_no == ENOMEM ? "ENOMEM" :
                              err_no == EPERM ? "EPERM" :
                              "UNKNOWN"));
  }
  /**
     Helper function.
   **/
  static void* _call_work(void* ctx) {
    reinterpret_cast<AsyncAppender*>(ctx)->_work();
    return NULL;
  }
  /**
     Worker thread main loop.
   **/
  void _work() {
    pthread_mutex_lock(&_mutex);
    try {
      for (;;) {
        while (_queue.empty()) {
          if (_stop) {
            return;
          }
          pthread_cond_wait(&_cv, &_mutex);
        }
        while (!_queue.empty()) {
          LoggingEvent event = _queue.front();
          _queue.pop();
          pthread_mutex_unlock(&_mutex);
          try {
            _appender->doAppend(event);
          } catch (...) {
          }
          pthread_mutex_lock(&_mutex);
        }
        pthread_cond_signal(&_cv_empty);
      }
    } catch (...) {
    }
    pthread_mutex_unlock(&_mutex);
  }
  /**
     Stop worker thread.
   **/
  void _stop_worker() {
    {
      pthread_mutex_lock(&_mutex);
      _stop = true;
      pthread_cond_signal(&_cv);
      pthread_mutex_unlock(&_mutex);
    }
    pthread_join(_thread, NULL);
  }
};

}  // namespace log4cpp
#endif  // _LOG4CPP_ASYNCAPPENDER_HH
