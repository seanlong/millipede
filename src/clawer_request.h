#ifndef CLAWER_DRIVER_CLAWER_REQUEST_
#define CLAWER_DRIVER_CLAWER_REQUEST_

#include <string>

struct ClawerRequest {
  typedef base::Callback<void(const std::string&)> Callback;

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
