#ifndef CLAWER_DRIVER_CLAWER_MANAGER_AUTO_H_
#define CLAWER_DRIVER_CLAWER_MANAGER_AUTO_H_

#include <queue>

#include "clawer_driver/src/clawer.h"
#include "clawer_driver/src/clawer_manager.h"
#include "clawer_driver/src/service/service_clawer_manager_auto.h"

class ClawerManagerAuto : public ClawerManager,
                          public ServiceClawerManagerAuto::Delegate,
                          public Clawer::Observer {
 public:
  explicit ClawerManagerAuto(ClawerDriverBrowserMainParts* main_parts);
  ~ClawerManagerAuto();

  // ServiceClawerManagerAuto::Delegate
  virtual void SetMaxClawerCount(int count) OVERRIDE;
  virtual void GetHTMLFromIdleClawer(
      const GURL& url,
      ServiceClawerManagerAuto::GetHTMLCallback callback) OVERRIDE;

  // Clawer::Observer
  virtual void OnClawerIdle(Clawer* clawer) OVERRIDE;

 private:
  struct Request {
    Request(const GURL& url, ServiceClawerManagerAuto::GetHTMLCallback callback)
      : url(url),
        callback(callback) {
    }
    const GURL url;
    const ServiceClawerManagerAuto::GetHTMLCallback callback;
  };

  int max_clawer_count_;
  std::queue<Request> requests_;
};

#endif
