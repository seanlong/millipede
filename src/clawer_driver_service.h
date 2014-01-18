#ifndef CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_

#include <string>

#include "base/threading/thread.h"
#include "base/values.h"
#include "dbus/exported_object.h"

namespace dbus {
class Bus;
class ExportedObject;
class MethodCall;
}

class ClawerDriverService : public base::Thread {
 public:
  ClawerDriverService();
  ~ClawerDriverService();
  
  void SetHTML(const base::Value*);

 private:
  virtual void Run(base::MessageLoop* message_loop) OVERRIDE;
  virtual void CleanUp() OVERRIDE;

  void CreateBus();
  
  void Echo(dbus::MethodCall* method_call,
            dbus::ExportedObject::ResponseSender response_sender);

  void GetHTML(dbus::MethodCall* method_call,
               dbus::ExportedObject::ResponseSender response_sender);

  void OnOwnerShip(const std::string& service_name, bool success);

  void OnExported(const std::string& interface_name,
                  const std::string& method_name,
                  bool success);

  dbus::Bus* bus_;
  dbus::ExportedObject* exported_object_;
  base::string16 html_; 
};

#endif
