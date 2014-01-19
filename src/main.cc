#include "clawer_driver/src/clawer_driver_main_delegate.h"
#include "clawer_driver/src/clawer_driver_service.h"
#include "content/public/app/content_main.h"

// Approach ONE <get info in render>:
// browser process:[get html for url DBus] -> service -> load url -> finish loading
// renderer process: [get html text IPC] -> return the text to browser(or directly return this from DBus?)

// Approach TWO <get info in browser>:
// browser process:[get html for url Dbus] -> service -> load url -> finish loading 
// -> RVH execute js to get html -> callback to service -> service callback on DBus

int main(int argc, const char** argv)
{
  //g_service = new ClawerDriverService();
  //base::PlatformThread::Join(g_service->thread_handle());
  ClawerDriverMainDelegate delegate;
  return content::ContentMain(argc, argv, &delegate);
}
