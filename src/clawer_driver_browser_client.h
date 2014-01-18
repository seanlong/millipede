#ifndef CLAWER_DRIVER_CLAWER_DRIVER_BROWSER_CLIENT_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_BROWSER_CLIENT_H_

#include "content/shell/browser/shell_content_browser_client.h"

namespace content {
class BrowserContext;
class BrowserMainParts;
class MainFunctionParams;
}

class ClawerDriverBrowserMainParts;

class ClawerDriverBrowserClient : public content::ShellContentBrowserClient {
 public:
  virtual content::BrowserMainParts* CreateBrowserMainParts(
      const content::MainFunctionParams& parameters) OVERRIDE;

  virtual net::URLRequestContextGetter* CreateRequestContext(
      content::BrowserContext* content_browser_context,
      content::ProtocolHandlerMap* protocol_handlers) OVERRIDE;

 private:
  ClawerDriverBrowserMainParts* main_parts_;
};

#endif
