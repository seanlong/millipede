#include "clawer_driver/src/clawer_driver_browser_main_parts.h"

#include "clawer_driver/src/clawer_manager_auto.h"
#include "clawer_driver/src/clawer_manager_manual.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/main_function_params.h"

ClawerDriverBrowserMainParts::ClawerDriverBrowserMainParts(
    const content::MainFunctionParams& parameters)
  : content::ShellBrowserMainParts(parameters) {
}

void ClawerDriverBrowserMainParts::PreMainMessageLoopRun() {
  browser_context_.reset(new content::ShellBrowserContext(false, NULL));
  clawer_service_.reset(new ClawerDriverService(this));
}

bool ClawerDriverBrowserMainParts::MainMessageLoop(int* result_code) {
  return false;
}

void ClawerDriverBrowserMainParts::CreateAutoManger(
    const base::Closure& callback) {
  clawer_manager_.reset(new ClawerManagerAuto(this));
  callback.Run();
}

void ClawerDriverBrowserMainParts::CreateManualManager(
    const base::Closure& callback) {
//  clawer_manager_.reset(new ClawerManagerManual(this));
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
