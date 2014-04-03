#ifndef CLAWER_DRIVER_CLAWER_H_
#define CLAWER_DRIVER_CLAWER_H_

#include "base/bind.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"


class ClawerRequest;

class Clawer : public content::WebContentsDelegate,
               public content::WebContentsObserver {
 public:
  class Observer {
   public:
    virtual void OnClawerIdle(Clawer* clawer) = 0;
    virtual ~Observer() {}
  }; 

  static Clawer* Create(content::BrowserContext* context,
                        linked_ptr<ClawerRequest> request,
                        int clawer_id);

  Clawer(linked_ptr<ClawerRequest> request,
         content::WebContents* web_contents,
         int clawer_id);
  ~Clawer();
 
  // Getter
  content::WebContents* web_contents() const { return web_contents_.get(); }
  int id() const { return id_; }

  // Observer methods
  void AddObserver(Observer* obs);
  void RemoveObserver(Observer* obs);

  bool IsIdle() const;

  void HandleRequest(linked_ptr<ClawerRequest> request);
  void HandleRequest();

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
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  void LoadURL(const GURL& url);
  void OnSendMsgToClawer(const std::string& msg);
  void InjectClientJS();
  void HandleRequestWithoutJS();
  void ConvertHTMLStrToCallback(const base::Value* html_value);
  void SetClawerToIdle();

  int id_;
  int did_finish_load_;
  ObserverList<Observer> observer_list_;
  linked_ptr<ClawerRequest> request_;
  scoped_ptr<content::WebContents> web_contents_;
  scoped_ptr<base::Closure> dfl_callback_;
};

#endif
