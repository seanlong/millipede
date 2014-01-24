#ifndef CLAWER_DRIVER_SERVICE_CLAWER_MANAGER_AUTO_H_
#define CLAWER_DRIVER_SERVICE_CLAWER_MANAGER_AUTO_H_

#include "base/strings/string16.h"
#include "clawer_driver/src/service/service_object.h"
#include "url/gurl.h"

class ClawerManager;

class ServiceClawerManagerAuto : public ServiceObject {
 public:
  typedef base::Callback<void(const base::string16&)> GetHTMLCallback;
  
  class Delegate {
   public:
    virtual void SetMaxClawerCount(int count) = 0;

    virtual void GetHTMLFromIdleClawer(
        const GURL& url, GetHTMLCallback callback) = 0;

   protected:
    virtual ~Delegate() {}
  };

  ServiceClawerManagerAuto(ClawerDriverBrowserMainParts* main_parts,
                           dbus::ExportedObject* obj);

  ~ServiceClawerManagerAuto();

  void SetDelegate();

 private:
  void SetPoolSize(dbus::MethodCall* method_call,
                   dbus::ExportedObject::ResponseSender response_sender);

  void GetHTML(dbus::MethodCall* method_call,
               dbus::ExportedObject::ResponseSender response_sender);

  void ReturnHTML(dbus::MethodCall* method_call,
                  dbus::ExportedObject::ResponseSender response_sender,
                  const base::string16& html_str);

  Delegate* delegate_;
};

#endif
