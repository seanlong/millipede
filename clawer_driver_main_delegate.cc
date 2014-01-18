#include "clawer_driver/clawer_driver_main_delegate.h"


content::ContentBrowserClient*
ClawerDriverMainDelegate::CreateContentBrowserClient() {
  browser_client_.reset(new ClawerDriverBrowserClient);
  return browser_client_.get();
}

content::ContentRendererClient*
ClawerDriverMainDelegate::CreateContentRendererClient() {
  renderer_client_.reset(new ClawerDriverRendererClient);
  return renderer_client_.get();
}
