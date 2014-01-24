#ifndef CLAWER_DRIVER_CLAWER_MANAGER_H_
#define CLAWER_DRIVER_CLAWER_MANAGER_H_

#include <map>

#include "base/memory/linked_ptr.h"
#include "clawer_driver/src/clawer.h"

class ClawerDriverBrowserMainParts;

class ClawerManager {
 public:
  ~ClawerManager();

 protected:
  explicit ClawerManager(ClawerDriverBrowserMainParts* main_parts);

  // Common clawer API
  linked_ptr<Clawer> GetClawerByID(int id);

  linked_ptr<Clawer> CreateClawer(const GURL& url);

  void RemoveClawer(Clawer* clawer);

  void RemoveAllClawers();

  typedef std::map<int, linked_ptr<Clawer> > ClawerMap;
  ClawerMap clawers_;

  ClawerDriverBrowserMainParts* main_parts_;

 private:
  int clawer_idx_;
};

#endif
