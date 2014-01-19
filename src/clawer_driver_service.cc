#include "clawer_driver/src/clawer_driver_service.h"

#include "base/strings/utf_string_conversions.h"
#include "clawer_driver/src/clawer.h"
#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "clawer_driver/src/clawer_manager.h"
#include "content/public/browser/browser_thread.h"
#include "dbus/bus.h"
#include "dbus/message.h"
#include "dbus/object_manager.h"
#include "dbus/object_path.h"
#include "dbus/property.h"

using content::BrowserThread;

ClawerDriverService::ClawerDriverService(
    ClawerDriverBrowserMainParts* main_parts)
  : base::Thread("Download service thread"),
    main_parts_(main_parts) {
  /*
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  std::string process_type =
    command_line.GetSwitchValueASCII(switches::kProcessType);
  LOG(INFO) << process_type;
  */
  base::Thread::Options thread_options;
  thread_options.message_loop_type = base::MessageLoop::TYPE_IO;
  StartWithOptions(thread_options);
}

ClawerDriverService::~ClawerDriverService() {
  LOG(INFO) << __FUNCTION__;
}

void ClawerDriverService::Run(base::MessageLoop* message_loop) {
  LOG(INFO) << "running...";

  CreateBus();
  bus_->RequestOwnership("org.seanlong.ClawerDriverService",
                         dbus::Bus::REQUIRE_PRIMARY,
                         base::Bind(&ClawerDriverService::OnOwnerShip,
                                    base::Unretained(this)));
  exported_object_ = bus_->GetExportedObject(
      dbus::ObjectPath("/org/seanlong/DownloadObject"));

  exported_object_->ExportMethod(
      "org.seanlong.DownloadInterface",
      "Echo",
      base::Bind(&ClawerDriverService::Echo,
                 base::Unretained(this)),
      base::Bind(&ClawerDriverService::OnExported,
                 base::Unretained(this)));

  exported_object_->ExportMethod(
      "org.seanlong.DownloadInterface",
      "GetHTML",
      base::Bind(&ClawerDriverService::GetHTML,
                 base::Unretained(this)),
      base::Bind(&ClawerDriverService::OnExported,
                 base::Unretained(this)));

  base::Thread::Run(message_loop);
}

void ClawerDriverService::CleanUp() {
  LOG(INFO) << "clean up...";
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

void ClawerDriverService::GetHTML(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender) {
  dbus::MessageReader reader(method_call);
  std::string url;
  if (!reader.PopString(&url)) {
    response_sender.Run(scoped_ptr<dbus::Response>());
    return;
  }

  LOG(INFO) << "Get message:" << url;

  Clawer::GetHTMLCallback callback =
    base::Bind(&ClawerDriverService::ReturnHTML,
               base::Unretained(this),
               method_call,
               response_sender);              
  
  BrowserThread::PostTask(BrowserThread::UI,
      FROM_HERE,
      base::Bind(&ClawerManager::GetHTMLFromNewClawer,
                 base::Unretained(main_parts_->GetClawerManager()),
                 url,
                 callback));
}

void ClawerDriverService::ReturnHTML(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender,
    const base::string16& html_str) {
  scoped_ptr<dbus::Response> response =
    dbus::Response::FromMethodCall(method_call);
  dbus::MessageWriter writer(response.get());
  writer.AppendString(UTF16ToUTF8(html_str));
  response_sender.Run(response.Pass());
}

void ClawerDriverService::OnOwnerShip(const std::string& service_name,
                                  bool success) {
  LOG(INFO) << service_name << ": " << success;
}

void ClawerDriverService::OnExported(const std::string& interface_name,
                                 const std::string& method_name,
                                 bool success) {
  LOG(INFO) << interface_name << " " << method_name << " " << success;
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
