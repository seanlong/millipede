#ifndef CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "base/values.h"
#include "dbus/exported_object.h"

namespace dbus {
class Bus;
class ExportedObject;
class MethodCall;
}

class ClawerDriverBrowserMainParts;

class ClawerDriverService : public base::Thread {
 public:
  class ServiceObject {
   public:
    ServiceObject(ClawerDriverService* service_owner,
                  ClawerDriverBrowserMainParts* main_parts,
                  dbus::ExportedObject* obj)
      : service_owner_(service_owner),
        main_parts_(main_parts),
        exported_object_(obj) {
    }

    virtual ~ServiceObject() {}

    virtual void Init() = 0;

   protected:
    ClawerDriverService* service_owner_;
    ClawerDriverBrowserMainParts* main_parts_;
    dbus::ExportedObject* exported_object_;
  };

  explicit ClawerDriverService(ClawerDriverBrowserMainParts* main_parts);
  ~ClawerDriverService();
 
  void OnOwnerShip(const std::string& service_name, bool success);

  void OnExported(const std::string& interface_name,
                  const std::string& method_name,
                  bool success);

 private:
  virtual void Run(base::MessageLoop* message_loop) OVERRIDE;
  virtual void CleanUp() OVERRIDE;

  void CreateBus();
  
  void Echo(dbus::MethodCall* method_call,
            dbus::ExportedObject::ResponseSender response_sender);

  void SetMode(dbus::MethodCall* method_call,
               dbus::ExportedObject::ResponseSender response_sender);
  
  dbus::Bus* bus_;
  dbus::ExportedObject* exported_object_;

  scoped_ptr<ServiceObject> manager_;

  ClawerDriverBrowserMainParts* main_parts_;
};

#endif
