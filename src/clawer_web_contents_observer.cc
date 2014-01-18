#include "clawer_driver/src/clawer_web_contents_observer.h"

#include "base/bind.h"
#include "base/strings/utf_string_conversions.h"
#include "clawer_driver/src/clawer_driver_service.h"
#include "content/public/browser/render_view_host.h"

extern ClawerDriverService* g_service;

ClawerWebContentsObserver::ClawerWebContentsObserver(
    content::WebContents* contents)
  : content::WebContentsObserver(contents) {
}

void ClawerWebContentsObserver::DidStopLoading(
    content::RenderViewHost* render_view_host) {
  render_view_host->ExecuteJavascriptInWebFrameCallbackResult(
      base::string16(),
      base::ASCIIToUTF16("document.documentElement.outerHTML"),
      base::Bind(&ClawerDriverService::SetHTML, base::Unretained(g_service)));
}
