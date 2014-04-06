#ifndef CLAWER_DRIVER_CLAWER_MANAGER_H_
#define CLAWER_DRIVER_CLAWER_MANAGER_H_

#include <map>
#include <queue>

#include "base/memory/linked_ptr.h"
#include "clawer_driver/src/clawer.h"
#include "clawer_driver/src/clawer_request.h"

class ClawerDriverBrowserMainParts;

class ClawerManager : public Clawer::Observer {
 public:
  explicit ClawerManager(ClawerDriverBrowserMainParts* main_parts);
  ~ClawerManager();

  void SetMaxClawerCount(int count);
  void GetHTMLFromIdleClawer(const GURL& url, const std::string& js,
                             const ClawerRequest::Callback& callback);

  // Clawer::Observer
  virtual void OnClawerIdle(Clawer* clawer) OVERRIDE;

 private:
  // Common clawer management APIs
  linked_ptr<Clawer> GetClawerByID(int id);
  linked_ptr<Clawer> CreateClawer(linked_ptr<ClawerRequest> request);
  void RemoveClawer(Clawer* clawer);
  void RemoveAllClawers();

  typedef std::map<int, linked_ptr<Clawer> > ClawerMap;
  ClawerMap clawers_;

  ClawerDriverBrowserMainParts* main_parts_;
  int clawer_id_counter_;

  int clawer_pool_max_;
  std::queue<linked_ptr<ClawerRequest> > requests_;
};

#endif
