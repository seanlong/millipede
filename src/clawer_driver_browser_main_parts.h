#ifndef CLAWER_DRIVER_CLAWER_DRIVER_BROWSER_MAIN_PARTS_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_BROWSER_MAIN_PARTS_H_

#include "base/memory/scoped_ptr.h"
#include "clawer_driver/src/clawer_driver_service.h"
#include "clawer_driver/src/clawer_manager.h"
#include "content/shell/browser/shell_browser_context.h"
#include "content/shell/browser/shell_browser_main_parts.h"

namespace {
class MainFunctionParams;
}

class ClawerDriverBrowserMainParts: public content::ShellBrowserMainParts {
 public:
  explicit ClawerDriverBrowserMainParts(
      const content::MainFunctionParams& parameters);

  virtual ~ClawerDriverBrowserMainParts() {}

  // ShellBrowserMainParts
  virtual void PreMainMessageLoopRun() OVERRIDE;
  virtual bool MainMessageLoop(int* result_code) OVERRIDE;

  // Getters
  content::ShellBrowserContext* browser_context();
  ClawerManager* GetClawerManager() const;
  ClawerDriverService* GetClawerDriverService() const;

 private:
  scoped_ptr<content::ShellBrowserContext> browser_context_;
  scoped_ptr<ClawerManager> clawer_manager_;
  scoped_ptr<ClawerDriverService> clawer_service_;
};

#endif
