#include "clawer_driver/src/clawer_manager.h"

#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "content/shell/browser/shell_browser_context.h"

ClawerManager::ClawerManager(ClawerDriverBrowserMainParts* main_parts)
  : main_parts_(main_parts),
    clawer_id_counter_(0),
    clawer_pool_max_(-1) {
}

ClawerManager::~ClawerManager() {
  RemoveAllClawers();
}

linked_ptr<Clawer> ClawerManager::GetClawerByID(int id) {
  ClawerMap::iterator it = clawers_.find(id);
  if (it == clawers_.end())
    return linked_ptr<Clawer>();
  return it->second;
}

linked_ptr<Clawer> ClawerManager::CreateClawer(
    linked_ptr<ClawerRequest> request) {
  linked_ptr<Clawer> clawer(Clawer::Create(main_parts_->browser_context(),
                            request, ++clawer_id_counter_));
  clawers_.insert(std::make_pair(clawer->id(), clawer));
  return clawer;
}

void ClawerManager::RemoveClawer(Clawer* clawer) {
  ClawerMap::iterator it = clawers_.find(clawer->id());
  CHECK(it != clawers_.end());
  clawers_.erase(clawer->id());
}

void ClawerManager::RemoveAllClawers() {
  clawers_.clear();
}

void ClawerManager::SetMaxClawerCount(int count) {
  clawer_pool_max_ = count; 
}

void ClawerManager::GetHTMLFromIdleClawer(
      const GURL& url,
      const std::string& js,
      const ClawerRequest::Callback& callback) {
  linked_ptr<Clawer> clawer;
  ClawerMap::iterator it = clawers_.begin();
  for (; it != clawers_.end(); ++it) {
    if(it->second->IsIdle()) {
      clawer = it->second;
      break;
    }
  }

  linked_ptr<ClawerRequest> new_request(new ClawerRequest(url, js, callback));
  if (clawer.get()) {
    clawer->HandleRequest(new_request);
  } else {
    if (clawer_pool_max_ > 0 && int(clawers_.size()) == clawer_pool_max_)
      requests_.push(new_request);
    else
      CreateClawer(new_request);
  }
}

void ClawerManager::OnClawerIdle(Clawer* clawer) {
  if (requests_.empty())
    return;

  linked_ptr<ClawerRequest> req = requests_.front();
  requests_.pop();
  clawer->HandleRequest(req);
}

