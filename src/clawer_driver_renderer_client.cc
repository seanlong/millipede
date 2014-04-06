#include "clawer_driver/src/clawer_driver_renderer_client.h"

#include "base/strings/utf_string_conversions.h"
#include "clawer_driver/src/clawer_messages.h"
#include "content/public/renderer/render_view.h"
#include "third_party/WebKit/public/web/WebDocument.h"
#include "third_party/WebKit/public/web/WebElement.h"
#include "third_party/WebKit/public/web/WebFrame.h"
#include "third_party/WebKit/public/web/WebScriptSource.h"
#include "third_party/WebKit/public/web/WebView.h"
#include "third_party/WebKit/public/web/WebSettings.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"

namespace {

void SendClawerMessage(const v8::FunctionCallbackInfo<v8::Value>& args) {
  void* ptr = v8::External::Cast(*args.Data())->Value();
  ClawerDriverRendererClient* client =
      static_cast<ClawerDriverRendererClient*>(ptr);

  // Send message to corresponding clawer to notify the retrieved data.
  client->render_view()->Send(new ClawerMessage_SendMsgToClawer(
    client->render_view()->GetRoutingID(), *(v8::String::Utf8Value(args[0]))));

#if 0
  // FIXME profiled by perf, the renderer will cost CPU after running some
  // script, the root cause is unknown, so let's temporarily disable the script
  // running after this message.
  // perf usage: www.chromium.org/developers/profiling-chromium-and-webkit 
  client->render_view()->GetWebView()->settings()->setJavaScriptEnabled(false);
#endif
  // TODO let browser explicitly call this by message
  client->render_view()->GetWebView()->mainFrame()->loadRequest(
      WebKit::WebURLRequest(GURL("about:blank")));
}

}

ClawerDriverRendererClient::ClawerRenderViewObserver::ClawerRenderViewObserver(
    content::RenderView* render_view)
  : content::RenderViewObserver(render_view) {
}

void ClawerDriverRendererClient::ClawerRenderViewObserver::DidStopLoading() {
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

void ClawerDriverRendererClient::DidCreateScriptContext(
    WebKit::WebFrame* frame,
    v8::Handle<v8::Context> context,
    int extension_group,
    int world_id) {
  WebKit::WebFrame* main_frame = render_view_->GetWebView()->mainFrame();
  if (frame != main_frame)
    return;

  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope handle_scope(isolate);
  v8::Handle<v8::Object> global = context->Global();

  v8::Handle<v8::Value> data = v8::External::New(this);
  global->Set(v8::String::New("___sendClawerMessage"),
      v8::FunctionTemplate::New(SendClawerMessage, data)->GetFunction());
}

void ClawerDriverRendererClient::RenderViewCreated(
    content::RenderView* render_view) {
  render_view_ = render_view;
  content::ShellContentRendererClient::RenderViewCreated(render_view);
  //view_observer_.reset(new ClawerRenderViewObserver(render_view));
}
