#include "clawer_driver/clawer_driver_renderer_client.h"

#include "base/strings/utf_string_conversions.h"
#include "content/public/renderer/render_view.h"
#include "third_party/WebKit/public/web/WebDocument.h"
#include "third_party/WebKit/public/web/WebElement.h"
#include "third_party/WebKit/public/web/WebFrame.h"
#include "third_party/WebKit/public/web/WebScriptSource.h"
#include "third_party/WebKit/public/web/WebView.h"

ClawerDriverRendererClient::ClawerRenderViewObserver::ClawerRenderViewObserver(
    content::RenderView* render_view)
  : content::RenderViewObserver(render_view) {
  LOG(INFO) << __FUNCTION__;
}

void ClawerDriverRendererClient::ClawerRenderViewObserver::DidStopLoading() {
  LOG(INFO) << __FUNCTION__;
#if 0
  //std::string js = "console.log(document.getElementsByTagName('html')[0].outerHTML)";
  std::string js = "console.log(document.documentElement.outerHTML)";
  base::string16 js16;
  if (!UTF8ToUTF16(js.c_str(), js.length(), &js16)) {
    NOTREACHED();
    return;
  }
  base::string16 frame_xpath;
  render_view()->EvaluateScript(frame_xpath, js16, 0, false);
#endif
#if 0
  WebKit::WebFrame* frame = render_view()->GetWebView()->mainFrame();
  LOG(INFO) << "====" << frame;
  //LOG(INFO) << frame->document().documentElement().innerText().utf8();
  //WebKit::Element* priv = WebKit::unwrap<WebKit::Element>(frame->document().documentElement());
  while((frame = frame->traverseNext(false))) {
    LOG(INFO) << frame;
  }
#endif
  // If "undefined" is returned here, the renderer will crash because of
  // v8::internal::Execution::ToString() fail.
#if 0
  std::string js = "document.documentElement.outerHTML";
  base::string16 js16;
  if (!UTF8ToUTF16(js.c_str(), js.length(), &js16)) {
    NOTREACHED();
    return;
  }

  WebKit::WebFrame* frame = render_view()->GetWebView()->mainFrame();
  std::string ret = EvaluateScript(frame, js16);
  //htmls.push_back(ret);
  while((frame = frame->traverseNext(false))) {
//      LOG(INFO) << EvaluateScript(frame, js16);
      htmls.push_back(EvaluateScript(frame, js16));
  }
#endif

#if 0
  base::MessageLoop::current()->PostDelayedTask(
    FROM_HERE,
    base::Bind(&MyRenderViewObserver::EvaluateScript,
               base::Unretained(this),
               render_view()->GetWebView()->mainFrame(),
               js16),
    base::TimeDelta::FromMilliseconds(20000));
#endif
}

std::string
ClawerDriverRendererClient::ClawerRenderViewObserver::EvaluateScript(
    WebKit::WebFrame* frame, const base::string16& jscript) {
  v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
  v8::Handle<v8::Value> result = frame->executeScriptAndReturnValue(
      WebKit::WebScriptSource(jscript));
  v8::String::Utf8Value str(result);
  return std::string(*str, str.length());
}

void ClawerDriverRendererClient::RenderViewCreated(
    content::RenderView* render_view) {
  content::ShellContentRendererClient::RenderViewCreated(render_view);
  view_observer_.reset(new ClawerRenderViewObserver(render_view));
}
