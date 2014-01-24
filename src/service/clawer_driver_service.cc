#include "clawer_driver/src/service/clawer_driver_service.h"

#include "base/strings/utf_string_conversions.h"
#include "clawer_driver/src/clawer.h"
#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "clawer_driver/src/clawer_manager.h"
#include "clawer_driver/src/service/service_clawer_manager_auto.h"
#include "content/public/browser/browser_thread.h"
#include "dbus/bus.h"
#include "dbus/message.h"
#include "dbus/object_manager.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

using content::BrowserThread;

namespace {

// Methods:
//   SetMode(string mode) -> void
//     "set mode to 'auto' or 'manual'"
const char kClawerManagerInterface[] = "org.seanlong.ClawerDriver.Manager";
const char kClawerManagerError[] = "org.seanlong.ClawerDriver.Manager.Error";

}

ClawerDriverService::ClawerDriverService(
    ClawerDriverBrowserMainParts* main_parts)
  : base::Thread("ClawerService thread"),
    ServiceObject(main_parts) {
  base::Thread::Options thread_options;
  thread_options.message_loop_type = base::MessageLoop::TYPE_IO;
  StartWithOptions(thread_options);
}

ClawerDriverService::~ClawerDriverService() {
  LOG(INFO) << __LINE__;
}

void ClawerDriverService::Run(base::MessageLoop* message_loop) {
  CreateBus();
  bus_->RequestOwnership("org.seanlong.ClawerDriver",
                         dbus::Bus::REQUIRE_PRIMARY,
                         base::Bind(&ClawerDriverService::OnOwnerShip,
                                    base::Unretained(this)));
  exported_object_ = bus_->GetExportedObject(dbus::ObjectPath("/Manager"));

  // Test echo method.
  exported_object_->ExportMethod(
      kClawerManagerInterface,
      "Echo",
      base::Bind(&ClawerDriverService::Echo,
                 base::Unretained(this)),
      base::Bind(&ClawerDriverService::OnExported,
                 base::Unretained(this)));

  exported_object_->ExportMethod(
      kClawerManagerInterface,
      "SetMode",
      base::Bind(&ClawerDriverService::SetMode,
                 base::Unretained(this)),
      base::Bind(&ClawerDriverService::OnExported,
                 base::Unretained(this)));

  message_loop->Run();
}

void ClawerDriverService::CleanUp() {
  LOG(INFO) << __LINE__;
}

void ClawerDriverService::CreateBus() {
  base::Thread::Options thread_options;
  thread_options.message_loop_type = base::MessageLoop::TYPE_IO;
  std::string thread_name = "D-Bus thread";
  base::Thread* dbus_thread = new base::Thread(thread_name.c_str());
  dbus_thread->StartWithOptions(thread_options);

  dbus::Bus::Options options;
  options.dbus_task_runner = dbus_thread->message_loop_proxy();
  bus_ = new dbus::Bus(options);
}

void ClawerDriverService::Echo(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender) {
  dbus::MessageReader reader(method_call);
  std::string text_message;
  if (!reader.PopString(&text_message)) {
    response_sender.Run(scoped_ptr<dbus::Response>());
    return;
  }

  LOG(INFO) << "Get message:" << text_message;
  scoped_ptr<dbus::Response> response =
    dbus::Response::FromMethodCall(method_call);
  dbus::MessageWriter writer(response.get());
  writer.AppendString(text_message);
  response_sender.Run(response.Pass());
}

void ClawerDriverService::SetMode(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender) {
  dbus::MessageReader reader(method_call);
  std::string mode_str;
  if (!reader.PopString(&mode_str)) {
    SendErrorResponse(method_call, response_sender,
                      kClawerManagerError, "No mode string");
    return;
  }

  // FIXME We don't support switch between service modes at the moment.
  if (manager_service_.get()) {
    response_sender.Run(dbus::Response::FromMethodCall(method_call).Pass());
    return;
  }
    
  LOG(INFO) << "Get message:" << mode_str;
  if (mode_str == "auto") {
    manager_service_.reset(
        new ServiceClawerManagerAuto(main_parts_, exported_object_));
    response_sender.Run(dbus::Response::FromMethodCall(method_call).Pass());
  } else {
    // manual mode not support yet.
    SendErrorResponse(method_call, response_sender,
                      kClawerManagerError, "Invalid mode string");
  }
}

void ClawerDriverService::OnOwnerShip(const std::string& service_name,
                                      bool success) {
  LOG(INFO) << service_name << ": " << success;
}

