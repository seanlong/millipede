#ifndef CLAWER_DRIVER_CLAWER_WEB_CONTENTS_OBSERVER_H_
#define CLAWER_DRIVER_CLAWER_WEB_CONTENTS_OBSERVER_H_

#include "content/public/browser/web_contents_observer.h"

namespace content {
class WebContents;
}

class ClawerWebContentsObserver : public content::WebContentsObserver {
 public:
  ClawerWebContentsObserver(content::WebContents* contents);
  
  virtual void DidStopLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;
};

#endif
