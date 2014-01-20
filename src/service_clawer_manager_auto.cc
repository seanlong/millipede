#include "clawer_driver/src/service_clawer_manager_auto.h"

#include "base/strings/utf_string_conversions.h"
#include "clawer_driver/src/clawer.h"
#include "clawer_driver/src/clawer_driver_browser_main_parts.h"
#include "clawer_driver/src/clawer_manager.h"
#include "content/public/browser/browser_thread.h"
#include "dbus/message.h"
#include "dbus/object_manager.h"
#include "dbus/object_path.h"

using content::BrowserThread;

namespace {

// In auto mode, the manager will running in an automatic style. The clawers
// will be controlled by the manager itself, and all crawling job will started
// by this iterface.
// When a new URL request is received, the manager will find an idle clawer and
// let it to load such URL. If there's no existing idle clawer, a new one will
// be created with the new URL. When the max_clawer_count is set, and no idle
// clawer exist, then the request will be queued until a free one is avaliable.
// Client should not access the individual clawer object in this mode.
//
// Methods:
//   SetPoolSize(int size) -> void
//     "set max_clawer_count in the pool, the default is unlimited"
//   GetHTML(string url) -> string html
//     "return the html string after the clawer finish loads the url" 
const char kClawerManagerAutoModeInterface[] =
  "org.seanlong.ClawerDriver.Manager.AutoMode";

}

ServiceClawerManagerAuto::ServiceClawerManagerAuto(
      ClawerDriverService* service_owner,
      ClawerDriverBrowserMainParts* main_parts,
      dbus::ExportedObject* obj)
  : ServiceObject(service_owner, main_parts, obj) {
}
  
void ServiceClawerManagerAuto::Init() {
#if 0
  exported_object_->ExportMethod(
      kClawerManagerAutoModeInterface,
      "SetPoolSize",
      base::Bind(&ServiceClawerManagerAuto::SetPoolSize,
                 base::Unretained(this))
      base::Bind(&ClawerDriverService::OnExported,
                 base::Unretained(this)));
#endif
  exported_object_->ExportMethod(
      kClawerManagerAutoModeInterface,
      "GetHTML",
      base::Bind(&ServiceClawerManagerAuto::GetHTML,
                 base::Unretained(this)),
      base::Bind(&ClawerDriverService::OnExported,
                 base::Unretained(service_owner_)));
}

void ServiceClawerManagerAuto::GetHTML(
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
    base::Bind(&ServiceClawerManagerAuto::ReturnHTML,
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

void ServiceClawerManagerAuto::ReturnHTML(
    dbus::MethodCall* method_call,
    dbus::ExportedObject::ResponseSender response_sender,
    const base::string16& html_str) {
  scoped_ptr<dbus::Response> response =
    dbus::Response::FromMethodCall(method_call);
  dbus::MessageWriter writer(response.get());
  writer.AppendString(UTF16ToUTF8(html_str));
  response_sender.Run(response.Pass());
}
