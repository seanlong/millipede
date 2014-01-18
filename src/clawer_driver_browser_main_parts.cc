#include "clawer_driver/src/clawer_driver_browser_main_parts.h"

#include "content/public/browser/web_contents.h"
#include "content/public/common/main_function_params.h"
#include "content/shell/browser/shell_browser_context.h"

ClawerDriverBrowserMainParts::ClawerDriverBrowserMainParts(
    const content::MainFunctionParams& parameters)
  : content::ShellBrowserMainParts(parameters) {
}

void ClawerDriverBrowserMainParts::PreMainMessageLoopRun() {
  browser_context_.reset(new content::ShellBrowserContext(false, NULL));
  //FIXME page like "baidu.com" will trigger 2 times of didstoploading(both in
  //renderer and in browser)
  //GURL url("http://baidu.com");
  GURL url("http://www.newsmth.net/bbstcon.php?board=Shanghai&gid=1869129907");
  content::WebContents* web_contents = content::WebContents::Create(
      content::WebContents::CreateParams(browser_context_.get(), NULL));
  content::NavigationController::LoadURLParams params(url);
  params.transition_type = content::PageTransitionFromInt(
      content::PAGE_TRANSITION_TYPED |
      content::PAGE_TRANSITION_FROM_ADDRESS_BAR);
  web_contents->GetController().LoadURLWithParams(params);
  contents_observer_.reset(new ClawerWebContentsObserver(web_contents));
}

bool ClawerDriverBrowserMainParts::MainMessageLoop(int* result_code) {
  return false;
}

content::ShellBrowserContext* ClawerDriverBrowserMainParts::browser_context() {
  return browser_context_.get();
}

