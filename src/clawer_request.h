#ifndef CLAWER_DRIVER_CLAWER_REQUEST_
#define CLAWER_DRIVER_CLAWER_REQUEST_

#include <string>

#include <url/gurl.h>

struct ClawerRequest {
  enum Error {
    NOERROR = -1,
    TIMEOUT,
    CRASH,
    UNKNOWN,
  };

  typedef base::Callback<void(const std::string&, const Error)> Callback;

  static std::string ErrorToString(Error error) {
    std::string prefix = "Clawer request error: ";
    switch(error) {
      case TIMEOUT:
        return prefix + "timeout";
      case CRASH:
        return prefix + "renderer crashed";
      default:
        return prefix + "unknown";
    }
  }

  ClawerRequest(const GURL& url,
          const std::string& js,
          const Callback& callback)
    : url(url),
      js(js),
      callback(callback) {
  }

  ~ClawerRequest() {}

  const GURL url;
  const std::string js;
  const Callback callback;
};

#endif
