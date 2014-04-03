#include "clawer_driver/src/clawer_driver_service.h"

#include "base/strings/utf_string_conversions.h"
#include "clawer_driver/src/clawer.h"
#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "clawer_driver/src/clawer_manager.h"
#include "clawer_driver/src/clawer_request.h"
#include "content/public/browser/browser_thread.h"
#include "dbus/bus.h"
#include "dbus/message.h"
#include "dbus/object_manager.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

using content::BrowserThread;

namespace {

// When a new URL request is received, the manager will find an idle clawer and
// let it to load such URL. If there's no existing idle clawer, a new one will
// be created with the new URL. When the max_clawer_count is set, and no idle
// clawer exist, then the request will be queued until a free one is avaliable.
//
// Methods:
//   SetPoolSize(int size) -> void
//     "set max_clawer_count in the pool, the default is unlimited, if the size
//     is -1, then the pool size will reset to unlimited"
//   GetHTML(string url, optional string inject_js) -> string html
//     "return the html string after the clawer finish loads the url or when
//     injected js explicitly return the data by using ___sendClawerMessage"
//   GetIdleClawer() -> int clawer_id
//     "return an idle clawer and set it busy. When there's no idle clawer the
//     method will be blocked until there's free one"
//   RunAction(int clawer_id, string action_js) -> string data
//     "run action_js in the specified clawer which may return the data by using
//     ___sendClawerMessage"
const char kClawerManagerInterface[] = "org.seanlong.ClawerDriver.Manager";
const char kClawerManagerError[] = "org.seanlong.ClawerDriver.Manager.Error";

}

ClawerDriverService::ClawerDriverService(ClawerManager* clawer_manager)
  : base::Thread("ClawerService thread"),
    clawer_manager_(clawer_manager) {
  base::Thread::Options thread_options;
  thread_options.message_loop_type = base::MessageLoop::TYPE_IO; StartWithOptions(thread_options);
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
      "SetPoolSize",
      base::Bind(&ClawerDriverService::SetPoolSize,
                 base::Unretained(this)),
      base::Bind(&ClawerDriverService::OnExported,
                 base::Unretained(this)));

  exported_object_->ExportMethod(
      kClawerManagerInterface,
      "GetHTML",
      base::Bind(&ClawerDriverService::GetHTML,
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

void ClawerDriverService::SetPoolSize(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender) {
  dbus::MessageReader reader(method_call);
  int size;
  if (!reader.PopInt32(&size)) {
    SendErrorResponse(method_call, response_sender,
                      kClawerManagerError, "Invaid pool size");
    return;
  }

  //LOG(INFO) << "Get message:" << size;
  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(&ClawerManager::SetMaxClawerCount,
                 base::Unretained(clawer_manager_), size));

  scoped_ptr<dbus::Response> response =
    dbus::Response::FromMethodCall(method_call);
  response_sender.Run(response.Pass());
}

void ClawerDriverService::GetHTML(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender) {
  dbus::MessageReader reader(method_call);
  std::string url_str;
  GURL url;
  if (!reader.PopString(&url_str) || !(url = GURL(url_str)).is_valid()) {
    SendErrorResponse(method_call, response_sender,
                      kClawerManagerError, "Invalid URL");
    return;
  }

  std::string js_str;
  reader.PopString(&js_str);

  LOG(INFO) << "Get message:" << url_str;
  LOG(INFO) << "Get inject JS:" << js_str;
  ClawerRequest::Callback callback =
    base::Bind(&ClawerDriverService::ReturnHTML,
               base::Unretained(this), method_call, response_sender);

  BrowserThread::PostTask(
      BrowserThread::UI,
      FROM_HERE,
      base::Bind(&ClawerManager::GetHTMLFromIdleClawer,
                 base::Unretained(clawer_manager_), url, js_str, callback));
}

void ClawerDriverService::ReturnHTML(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender,
    const std::string& html_str) {
  scoped_ptr<dbus::Response> response =
    dbus::Response::FromMethodCall(method_call);
  dbus::MessageWriter writer(response.get());
  writer.AppendString(html_str);
  response_sender.Run(response.Pass());
}

void ClawerDriverService::OnOwnerShip(const std::string& service_name,
                                      bool success) {
  LOG(INFO) << service_name << ": " << success;
}

void ClawerDriverService::SendErrorResponse(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender& response_sender,
    const char* error_name,
    const char* error_text) {
  scoped_ptr<dbus::ErrorResponse> error = dbus::ErrorResponse::FromMethodCall(
      method_call, error_name, error_text);
  response_sender.Run(error.PassAs<dbus::Response>());
}

void ClawerDriverService::OnExported(const std::string& interface_name,
                                     const std::string& method_name,
                                     bool success) {
  LOG(INFO) << interface_name << " " << method_name << " " << success;
}

