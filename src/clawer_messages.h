#include <string>
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START ExtensionMsgStart

IPC_MESSAGE_ROUTED1(ClawerMessage_SendMsgToClawer, std::string)
