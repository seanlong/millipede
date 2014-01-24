#ifndef CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "base/values.h"
#include "clawer_driver/src/service/service_object.h"
#include "dbus/exported_object.h"

namespace dbus {
class Bus;
class MethodCall;
}

class ClawerDriverBrowserMainParts;

class ClawerDriverService : public base::Thread,
                            public ServiceObject {
 public:
  explicit ClawerDriverService(ClawerDriverBrowserMainParts* main_parts);
  ~ClawerDriverService();

 private:
  // base::Thread
  virtual void Run(base::MessageLoop* message_loop) OVERRIDE;
  virtual void CleanUp() OVERRIDE;

  void CreateBus();
  
  void OnOwnerShip(const std::string& service_name, bool success);
  
  void Echo(dbus::MethodCall* method_call,
            dbus::ExportedObject::ResponseSender response_sender);

  void SetMode(dbus::MethodCall* method_call,
               dbus::ExportedObject::ResponseSender response_sender);
  
  dbus::Bus* bus_;

  scoped_ptr<ServiceObject> manager_service_;
};

#endif
