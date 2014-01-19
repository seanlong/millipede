#include "clawer_driver/src/clawer_manager.h"

#include "clawer_driver/src/clawer.h"
#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "content/shell/browser/shell_browser_context.h"

ClawerManager::ClawerManager(ClawerDriverBrowserMainParts* main_parts)
  : main_parts_(main_parts),
    clawer_idx_(0) {
}

ClawerManager::~ClawerManager() {
  RemoveAllClawers();
}

void ClawerManager::GetHTMLFromNewClawer(
    const std::string& url, Clawer::GetHTMLCallback callback) {
  linked_ptr<Clawer> clawer = CreateClawer(url);
  clawer->GetHTML(callback);
}

linked_ptr<Clawer> ClawerManager::GetClawerByID(int id) {
  ClawerMap::iterator it = clawers_.find(id);
  if (it == clawers_.end())
    return linked_ptr<Clawer>();
  return it->second;
}

linked_ptr<Clawer> ClawerManager::CreateClawer(const std::string& url) {
  linked_ptr<Clawer> clawer(
      Clawer::Create(main_parts_->browser_context(), url));
  clawer->id_ = ++clawer_idx_;
  clawers_.insert(std::make_pair(clawer->id_, clawer));
  return clawer;
}

void ClawerManager::RemoveClawer(Clawer* clawer) {
  ClawerMap::iterator it = clawers_.find(clawer->id_);
  CHECK(it != clawers_.end());
  clawers_.erase(clawer->id_);
}

void ClawerManager::RemoveAllClawers() {
  clawers_.clear();
}

