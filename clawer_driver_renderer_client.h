#ifndef CLAWER_DRIVER_CLAWER_DRIVER_RENDERER_CLIENT_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_RENDERER_CLIENT_H_

#include "base/memory/scoped_ptr.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/shell/renderer/shell_content_renderer_client.h"

namespace content {
class RenderView;
}

namespace WebKit {
class WebFrame;
}

class ClawerDriverRendererClient : public content::ShellContentRendererClient {
 public:
  virtual void RenderViewCreated(content::RenderView* render_view) OVERRIDE;

 private:
  class ClawerRenderViewObserver : public content::RenderViewObserver {
   public:
    ClawerRenderViewObserver(content::RenderView* render_view);

    virtual void DidStopLoading() OVERRIDE;

   private:
    std::string EvaluateScript(
        WebKit::WebFrame* frame, const base::string16& jscript);
  };

  scoped_ptr<ClawerRenderViewObserver> view_observer_;
};

#endif
