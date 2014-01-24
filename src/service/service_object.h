#ifndef CLAWER_DRIVER_SERVICE_SERVICE_OBJECT_H_
#define CLAWER_DRIVER_SERVICE_SERVICE_OBJECT_H_

#include "dbus/exported_object.h"

class ClawerDriverBrowserMainParts;

class ServiceObject {
 public:
  virtual ~ServiceObject() = 0;

  void OnExported(const std::string& interface_name,
                  const std::string& method_name,
                  bool success);

 protected:
  explicit ServiceObject(ClawerDriverBrowserMainParts* main_parts)
    : main_parts_(main_parts) {
  }

  void SendErrorResponse(dbus::MethodCall* method_call,
                         dbus::ExportedObject::ResponseSender& response_sender,
                         const char* error_name,
                         const char* error_text);

  dbus::ExportedObject* exported_object_;

  ClawerDriverBrowserMainParts* main_parts_;
};

#endif
