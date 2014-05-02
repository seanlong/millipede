#ifndef CLAWER_DRIVER_CLAWER_DRIVER_MAIN_DELEGATE_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_MAIN_DELEGATE_H_

#include "base/memory/scoped_ptr.h"
#include "content/shell/app/shell_main_delegate.h"

namespace content {
class ContentBrowserClient;
class ContentRendererClient;
}

class ClawerDriverBrowserClient;
class ClawerDriverRendererClient;

class ClawerDriverMainDelegate : public content::ShellMainDelegate {
 public:
  ClawerDriverMainDelegate();
  ~ClawerDriverMainDelegate();

  virtual content::ContentBrowserClient*
      CreateContentBrowserClient() OVERRIDE;

  virtual content::ContentRendererClient*
      CreateContentRendererClient()OVERRIDE;

 private:
  scoped_ptr<ClawerDriverBrowserClient> browser_client_;
  scoped_ptr<ClawerDriverRendererClient> renderer_client_;
};

#endif
