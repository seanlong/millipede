#include "clawer_driver/src/clawer_manager_auto.h"

ClawerManagerAuto::ClawerManagerAuto(ClawerDriverBrowserMainParts* main_parts)
  : ClawerManager(main_parts),
    max_clawer_count_(-1) {
}

ClawerManagerAuto::~ClawerManagerAuto() {
}

void ClawerManagerAuto::SetMaxClawerCount(int count) {
  max_clawer_count_ = count; 
}

void ClawerManagerAuto::GetHTMLFromIdleClawer(
      const GURL& url,
      ServiceClawerManagerAuto::GetHTMLCallback callback) {
  linked_ptr<Clawer> clawer;
  ClawerMap::iterator it = clawers_.begin();
  for (; it != clawers_.end(); ++it) {
    if(it->second->IsIdle()) {
      clawer = it->second;
      break;
    }
  }

  if (clawer.get()) {
    clawer->ReloadURL(url);
  } else {
    if (max_clawer_count_ > 0 && int(clawers_.size()) == max_clawer_count_)
      requests_.push(Request(url, callback));
    else
      clawer = CreateClawer(url);
  }

  clawer->GetHTML(callback);
}

void ClawerManagerAuto::OnClawerIdle(Clawer* clawer) {
  if (requests_.empty())
    return;

  Request req = requests_.front();
  requests_.pop();
  clawer->ReloadURL(req.url);
  clawer->GetHTML(req.callback);
}

