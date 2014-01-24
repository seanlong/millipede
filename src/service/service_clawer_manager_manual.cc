
namespace {

// In manual mode, the manager will only provides clawer create/remove API to
// client. Client should control the crawling job itself, with clawer API more
// can be achieved than raw html text.
//
// Methods:
//   CreateClawer(string url) -> int clawer_id 
//     "create a new clawer and return its ID"
//   RemoveClawer(int id) -> void
//     "remove a clawer by its ID"
const char kClawerManagerManualModeInterface[] =
  "org.seanlong.ClawerDriver.Manager.Manual";

}
