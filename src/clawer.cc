#include "clawer_driver/src/clawer.h"

#include "base/strings/utf_string_conversions.cc"
#include "clawer_driver/src/clawer_manager.h"
#include "clawer_driver/src/clawer_messages.h"
#include "clawer_driver/src/clawer_request.h"
#include "content/public/browser/render_view_host.h"


Clawer* Clawer::Create(content::BrowserContext* context,
                       linked_ptr<ClawerRequest> request,
                       int clawer_id) {
  content::WebContents* web_contents = content::WebContents::Create(
        content::WebContents::CreateParams(context, NULL));
  return new Clawer(request, web_contents, clawer_id);
}

Clawer::Clawer(linked_ptr<ClawerRequest> request,
               content::WebContents* web_contents,
               int clawer_id)
    : WebContentsObserver(web_contents),
      id_(clawer_id),
      did_finish_load_(0),
      request_(request),
      web_contents_(web_contents) {
  CHECK(request_ != NULL);
  CHECK(request_->url.is_valid());
  HandleRequest();
}

Clawer::~Clawer() {
  LOG(INFO) << __LINE__;
}

void Clawer::AddObserver(Observer* obs) {
  observer_list_.AddObserver(obs);
}

void Clawer::RemoveObserver(Observer* obs) {
  observer_list_.RemoveObserver(obs);
}

bool Clawer::IsIdle() const {
  return request_ == NULL;
}

void Clawer::HandleRequest(linked_ptr<ClawerRequest> request) {
  if (request_ != NULL)
    LOG(WARNING) << "request received when clawer is not idle";
  request_ = request;
  did_finish_load_ = 0;
  HandleRequest();
}

void Clawer::HandleRequest() {
  CHECK(request_ != NULL);
  LoadURL(request_->url);

  if (request_->js.empty())
    return HandleRequestWithoutJS();

  dfl_callback_.reset(new base::Closure(
      base::Bind(&Clawer::InjectClientJS, base::Unretained(this))));
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
}

void Clawer::RenderProcessGone(base::TerminationStatus status) {
}

void Clawer::DidStopLoading(content::RenderViewHost* rvh) {
}

void Clawer::LoadURL(const GURL& url) {
  content::NavigationController::LoadURLParams params(url);
  params.transition_type = content::PageTransitionFromInt(
      content::PAGE_TRANSITION_TYPED |
      content::PAGE_TRANSITION_FROM_ADDRESS_BAR);
  web_contents_->GetController().LoadURLWithParams(params);
}

// We can't count on the first page's finish loading event to judge
// whether it's really finish loading all need information(like AJAX).
void Clawer::DidFinishLoad(int64 frame_id,
                           const GURL& validated_url,
                           bool is_main_frame,
                           content::RenderViewHost* render_view_host) {
  if (!is_main_frame) {
    return;
  }

  did_finish_load_++;
  if (dfl_callback_) {
    dfl_callback_->Run();
    // Currently seems the callbacks are only one-shot.
    dfl_callback_.reset();
  }
}

void Clawer::InjectClientJS() {
  content::RenderViewHost* rvh = web_contents_->GetRenderViewHost();
  rvh->ExecuteJavascriptInWebFrame(
      base::string16(),
      base::ASCIIToUTF16(request_->js));
}

void Clawer::HandleRequestWithoutJS() {
  if (!did_finish_load_) {
    dfl_callback_.reset(new base::Closure(
        base::Bind(&Clawer::HandleRequestWithoutJS, base::Unretained(this))));
  } else {
    content::RenderViewHost* rvh = web_contents_->GetRenderViewHost();
    rvh->ExecuteJavascriptInWebFrameCallbackResult(
        base::string16(),
        base::ASCIIToUTF16("document.documentElement.outerHTML"),
        base::Bind(&Clawer::ConvertHTMLStrToCallback,
                   base::Unretained(this)));
  }
  return;
}

void Clawer::ConvertHTMLStrToCallback(const base::Value* html_value) {
  std::string html_str;
  html_value->GetAsString(&html_str);
  request_->callback.Run(html_str);
  SetClawerToIdle();
}

void Clawer::SetClawerToIdle() {
  dfl_callback_.reset();
  request_.reset();
  did_finish_load_ = 0;
  FOR_EACH_OBSERVER(Observer, observer_list_, OnClawerIdle(this));
}

bool Clawer::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(Clawer, message)
    IPC_MESSAGE_HANDLER(ClawerMessage_SendMsgToClawer,
        OnSendMsgToClawer)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}

void Clawer::OnSendMsgToClawer(const std::string& msg) {
  if (request_ == NULL) {
    LOG(WARNING) << "message received when clawer is idle: " << msg;
    return;
  }

  request_->callback.Run(msg);
  SetClawerToIdle();
}
