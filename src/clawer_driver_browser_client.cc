#include "clawer_driver/src/clawer_driver_browser_client.h"

#include "content/public/common/main_function_params.h"
#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "clawer_driver/src/clawer_driver_resource_dispatcher_host_delegate.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/resource_dispatcher_host.h"
#include "content/shell/browser/shell_browser_context.h"

ClawerDriverBrowserClient::ClawerDriverBrowserClient() {
}

ClawerDriverBrowserClient::~ClawerDriverBrowserClient() {
}

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

void ClawerDriverBrowserClient::ResourceDispatcherHostCreated() {
  resource_dispatcher_host_delegate_.reset(
      new ClawerDriverResourceDispatcherHostDelegate());
  content::ResourceDispatcherHost::Get()->SetDelegate(
      resource_dispatcher_host_delegate_.get());
}
