#include "clawer_driver/src/clawer_driver_main_delegate.h"

#include "clawer_driver/src/clawer_driver_browser_client.h"
#include "clawer_driver/src/clawer_driver_renderer_client.h"

ClawerDriverMainDelegate::ClawerDriverMainDelegate() {
}

ClawerDriverMainDelegate::~ClawerDriverMainDelegate() {
}

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
