#include "clawer_driver/src/clawer_driver_browser_main_parts.h"

#include "content/public/browser/web_contents.h"
#include "content/public/common/main_function_params.h"

ClawerDriverBrowserMainParts::ClawerDriverBrowserMainParts(
    const content::MainFunctionParams& parameters)
  : content::ShellBrowserMainParts(parameters) {
}

void ClawerDriverBrowserMainParts::PreMainMessageLoopRun() {
  browser_context_.reset(new content::ShellBrowserContext(false, NULL));
  clawer_manager_.reset(new ClawerManager(this));
  clawer_service_.reset(new ClawerDriverService(clawer_manager_.get()));
}

bool ClawerDriverBrowserMainParts::MainMessageLoop(int* result_code) {
  return false;
}

content::ShellBrowserContext* ClawerDriverBrowserMainParts::browser_context() {
  return browser_context_.get();
}

ClawerManager* ClawerDriverBrowserMainParts::GetClawerManager() const {
  return clawer_manager_.get();
}

ClawerDriverService*
ClawerDriverBrowserMainParts::GetClawerDriverService() const {
  return clawer_service_.get();
}
