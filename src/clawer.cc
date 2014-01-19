#include "clawer_driver/src/clawer.h"

#include "base/strings/utf_string_conversions.cc"
#include "content/public/browser/render_view_host.h"

Clawer* Clawer::Create(
    content::BrowserContext* context, const std::string& url_str) {
  LOG(INFO) << context;
  LOG(INFO) << url_str;
  content::WebContents* web_contents = content::WebContents::Create(
        content::WebContents::CreateParams(context, NULL));
  return new Clawer(url_str, web_contents);
}

Clawer::Clawer(const std::string& url_str, content::WebContents* web_contents)
  : WebContentsObserver(web_contents),
    id_(-1),
    is_load_finish_(false),
    start_url_(url_str),
    web_contents_(web_contents) {
  CHECK(start_url_.is_valid());
  content::NavigationController::LoadURLParams params(start_url_);
  params.transition_type = content::PageTransitionFromInt(
      content::PAGE_TRANSITION_TYPED |
      content::PAGE_TRANSITION_FROM_ADDRESS_BAR);
  web_contents_->GetController().LoadURLWithParams(params);
}

Clawer::~Clawer() {
  LOG(INFO) << __LINE__;
}

void Clawer::GetHTML(const GetHTMLCallback& callback) {
  LOG(INFO) << __LINE__;
  if (!is_load_finish_) {
    load_finish_callback_.reset(new base::Closure(
        base::Bind(&Clawer::GetHTML, base::Unretained(this), callback)));
    return;
  }

  content::RenderViewHost* rvh = web_contents_->GetRenderViewHost();
  CHECK(rvh);
  rvh->ExecuteJavascriptInWebFrameCallbackResult(
      base::string16(),
      base::ASCIIToUTF16("document.documentElement.outerHTML"),
      base::Bind(&Clawer::ConvertHTMLStrToCallback,
                 base::Unretained(this),
                 callback));
}

void Clawer::ConvertHTMLStrToCallback(
    const GetHTMLCallback& callback, const base::Value* html_value) {
  base::string16 html_str;
  html_value->GetAsString(&html_str);
  callback.Run(html_str);
}

void Clawer::ExecuteJS(const base::string16& js) {
}

void Clawer::ExecuteJSWithCallback(
    const base::string16& js, ExecuteJSCallback callback) {
}

void Clawer::CloseContents(content::WebContents* source) {
}

void Clawer::WebContentsCreated(content::WebContents* source_contents,
                                int64 source_frame_id,
                                const string16& frame_name,
                                const GURL& target_url,
                                content::WebContents* new_contents) {
  LOG(INFO) << __LINE__;
}

void Clawer::RenderProcessGone(base::TerminationStatus status) {
  LOG(INFO) << __LINE__;
}

void Clawer::DidStopLoading(content::RenderViewHost* rvh) {
  // FIXME page like "baidu.com" will trigger 2 times of didstoploading(both in
  // renderer and in browser)
  // GURL url("http://baidu.com");
  LOG(INFO) << __LINE__;  
}

void Clawer::DidFinishLoad(int64 frame_id,
                           const GURL& validated_url,
                           bool is_main_frame,
                           content::RenderViewHost* render_view_host) {
  if (!is_main_frame) {
    LOG(INFO) << __LINE__;
    return;
  }

  is_load_finish_ = true;
  if (load_finish_callback_.get())
    load_finish_callback_->Run();
}

