#include "clawer_driver/src/service/service_object.h"

#include "base/logging.h"
#include "dbus/message.h"

void ServiceObject::SendErrorResponse(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender& response_sender,
    const char* error_name,
    const char* error_text) {
  scoped_ptr<dbus::ErrorResponse> error = dbus::ErrorResponse::FromMethodCall(
      method_call, error_name, error_text);
  response_sender.Run(error.PassAs<dbus::Response>());
}

ServiceObject::~ServiceObject() {
}

void ServiceObject::OnExported(const std::string& interface_name,
                               const std::string& method_name,
                               bool success) {
  LOG(INFO) << interface_name << " " << method_name << " " << success;
}
