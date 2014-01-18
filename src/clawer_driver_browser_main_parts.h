#ifndef CLAWER_DRIVER_CLAWER_DRIVER_BROWSER_MAIN_PARTS_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_BROWSER_MAIN_PARTS_H_

#include "base/memory/scoped_ptr.h"
#include "clawer_driver/src/clawer_web_contents_observer.h"
#include "content/shell/browser/shell_browser_main_parts.h"

namespace {
class MainFunctionParams;
class ShellBrowserContext;
}

class ClawerWebContentsObserver;

class ClawerDriverBrowserMainParts: public content::ShellBrowserMainParts {
 public:
  explicit ClawerDriverBrowserMainParts(
      const content::MainFunctionParams& parameters);

  virtual ~ClawerDriverBrowserMainParts() {}

  virtual void PreMainMessageLoopRun() OVERRIDE;

  virtual bool MainMessageLoop(int* result_code) OVERRIDE;

  content::ShellBrowserContext* browser_context();

 private:
  scoped_ptr<content::ShellBrowserContext> browser_context_;
  scoped_ptr<ClawerWebContentsObserver> contents_observer_;
};

#endif
