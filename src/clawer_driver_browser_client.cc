#include "clawer_driver/src/clawer_driver_browser_client.h"

#include "content/public/common/main_function_params.h"
#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "content/public/browser/browser_context.h"
#include "content/shell/browser/shell_browser_context.h"

content::BrowserMainParts* ClawerDriverBrowserClient::CreateBrowserMainParts(
    const content::MainFunctionParams& parameters) {
  main_parts_ = new ClawerDriverBrowserMainParts(parameters);
  return main_parts_;
}

net::URLRequestContextGetter* ClawerDriverBrowserClient::CreateRequestContext(
    content::BrowserContext* content_browser_context,
    content::ProtocolHandlerMap* protocol_handlers) {
  content::ShellBrowserContext* context = main_parts_->browser_context();
  return context->CreateRequestContext(protocol_handlers);
}
