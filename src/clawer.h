#ifndef CLAWER_DRIVER_CLAWER_H_
#define CLAWER_DRIVER_CLAWER_H_

#include "base/bind.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"

class Clawer : public content::WebContentsDelegate,
               public content::WebContentsObserver {
 public:
  static Clawer* Create(
      content::BrowserContext* context, const std::string& url_str);

  Clawer(const std::string& url_str, content::WebContents* web_contents);
  ~Clawer();
 
  // Getter
  content::WebContents* GetWebContents() const { return web_contents_.get(); }
  const GURL& GetURL() const { return web_contents_->GetURL(); }
  const GURL& GetStartURL() const { return start_url_; }
  int GetID() const { return id_; }

  // Clawer API
  typedef base::Callback<void(const base::string16&)> GetHTMLCallback;
  void GetHTML(const GetHTMLCallback& callback);
  
  void ExecuteJS(const base::string16& js);

  typedef base::Callback<void(const base::Value*)> ExecuteJSCallback;
  void ExecuteJSWithCallback(
      const base::string16& js, ExecuteJSCallback callback);

  // content::WebContentsDelegate
  virtual void CloseContents(content::WebContents* source) OVERRIDE;
  virtual void WebContentsCreated(content::WebContents* source_contents,
                                  int64 source_frame_id,
                                  const string16& frame_name,
                                  const GURL& target_url,
                                  content::WebContents* new_contents) OVERRIDE;

  // content::WebContentsObserver
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;
  virtual void DidStopLoading(content::RenderViewHost* rvh) OVERRIDE;
  void DidFinishLoad(int64 frame_id,
                     const GURL& validated_url,
                     bool is_main_frame,
                     content::RenderViewHost* render_view_host) OVERRIDE;

 private:
  friend class ClawerManager;

  void ConvertHTMLStrToCallback(
      const GetHTMLCallback& callback, const base::Value* html_value);

  int id_;
  bool is_load_finish_;
  GURL start_url_;
  scoped_ptr<content::WebContents> web_contents_;
  scoped_ptr<base::Closure> load_finish_callback_;
};

#endif
