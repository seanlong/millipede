#ifndef CLAWER_DRIVER_SERVICE_CLAWER_MANAGER_AUTO_H_
#define CLAWER_DRIVER_SERVICE_CLAWER_MANAGER_AUTO_H_

#include "clawer_driver/src/clawer_driver_service.h"

class ServiceClawerManagerAuto : public ClawerDriverService::ServiceObject {
 public:
  ServiceClawerManagerAuto(ClawerDriverService* service_owner,
                           ClawerDriverBrowserMainParts* main_parts,
                           dbus::ExportedObject* obj);

  virtual void Init() OVERRIDE;

 private:
  void GetHTML(dbus::MethodCall* method_call,
               dbus::ExportedObject::ResponseSender response_sender);

  void ReturnHTML(dbus::MethodCall* method_call,
                  dbus::ExportedObject::ResponseSender response_sender,
                  const base::string16& html_str);
};

#endif
