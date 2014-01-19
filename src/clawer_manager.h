#ifndef CLAWER_DRIVER_CLAWER_MANAGER_H_
#define CLAWER_DRIVER_CLAWER_MANAGER_H_

#include <map>

#include "base/memory/linked_ptr.h"
#include "clawer_driver/src/clawer.h"

class ClawerDriverBrowserMainParts;

class ClawerManager {
 public:
  explicit ClawerManager(ClawerDriverBrowserMainParts* main_parts);
  ~ClawerManager();

  // Higher level clawing API
  void GetHTMLFromNewClawer(
      const std::string& url, Clawer::GetHTMLCallback callback);

  // Clawer API
  linked_ptr<Clawer> GetClawerByID(int id);
  
  linked_ptr<Clawer> CreateClawer(const std::string& url);

  void RemoveClawer(Clawer* clawer);
  void RemoveAllClawers();

 private:
  typedef std::map<int, linked_ptr<Clawer> > ClawerMap;
  ClawerMap clawers_;
  ClawerDriverBrowserMainParts* main_parts_;

  int clawer_idx_;
};

#endif
