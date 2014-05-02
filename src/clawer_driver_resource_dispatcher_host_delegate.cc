#include "clawer_driver/src/clawer_driver_resource_dispatcher_host_delegate.h"

ClawerDriverResourceDispatcherHostDelegate::
    ClawerDriverResourceDispatcherHostDelegate() {
}

ClawerDriverResourceDispatcherHostDelegate::
    ~ClawerDriverResourceDispatcherHostDelegate() {
}

bool ClawerDriverResourceDispatcherHostDelegate::ShouldBeginRequest(
    int child_id,
    int route_id,
    const std::string& method,
    const GURL& url,
    ResourceType::Type resource_type,
    content::ResourceContext* resource_context) {
  if (resource_type == ResourceType::MAIN_FRAME ||
      resource_type == ResourceType::SUB_FRAME ||
      resource_type == ResourceType::STYLESHEET ||
      resource_type == ResourceType::SCRIPT ||
      resource_type == ResourceType::XHR)
    return true;
  return false;
}
