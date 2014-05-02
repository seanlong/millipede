#ifndef CLAWER_DRIVER_RESOURCE_DISPATCHER_HOST_DELEGATE_H_
#define CLAWER_DRIVER_RESOURCE_DISPATCHER_HOST_DELEGATE_H_

#include "content/public/browser/resource_dispatcher_host_delegate.h"

class ClawerDriverResourceDispatcherHostDelegate
    : public content::ResourceDispatcherHostDelegate {
 public:
  ClawerDriverResourceDispatcherHostDelegate();
  virtual ~ClawerDriverResourceDispatcherHostDelegate();

  virtual bool ShouldBeginRequest(
      int child_id,
      int route_id,
      const std::string& method,
      const GURL& url,
      ResourceType::Type resource_type,
      content::ResourceContext* resource_context) OVERRIDE;
};

#endif
