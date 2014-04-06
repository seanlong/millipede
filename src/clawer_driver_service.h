#ifndef CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_
#define CLAWER_DRIVER_CLAWER_DRIVER_SERVICE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "base/values.h"
#include "dbus/exported_object.h"

namespace base {
class MessageLoop;
}

namespace dbus {
class Bus;
class MethodCall;
}

class ClawerManager;

class ClawerDriverService : public base::Thread {
 public:
  explicit ClawerDriverService(ClawerManager* clawer_manager);
  ~ClawerDriverService();

 private:
  // base::Thread
  virtual void Run(base::MessageLoop* message_loop) OVERRIDE;
  virtual void CleanUp() OVERRIDE;

  void CreateBus();
  
  void OnOwnerShip(const std::string& service_name, bool success);
  void OnExported(const std::string& interface_name,
                  const std::string& method_name,
                  bool success);
  
  void Echo(dbus::MethodCall* method_call,
            dbus::ExportedObject::ResponseSender response_sender);

  void SetPoolSize(dbus::MethodCall* method_call,
                   dbus::ExportedObject::ResponseSender response_sender);

  void GetHTML(dbus::MethodCall* method_call,
               dbus::ExportedObject::ResponseSender response_sender);

  void ReturnHTML(dbus::MethodCall* method_call,
                  dbus::ExportedObject::ResponseSender response_sender,
                  const std::string& html_str);

  void SendErrorResponse(dbus::MethodCall* method_call,
                         dbus::ExportedObject::ResponseSender& response_sender,
                         const char* error_name,
                         const char* error_text);
 
  dbus::Bus* bus_;
  dbus::ExportedObject* exported_object_;
  base::MessageLoop* dbus_message_loop_;
  ClawerManager* clawer_manager_;
};

#endif
