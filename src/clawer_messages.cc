#define IPC_MESSAGE_IMPL
#include "clawer_driver/src/clawer_messages.h"

#include "ipc/struct_constructor_macros.h"
#include "clawer_driver/src/clawer_messages.h"

// Generate destructors.
#include "ipc/struct_destructor_macros.h"
#include "clawer_driver/src/clawer_messages.h"

// Generate param traits write methods.
#include "ipc/param_traits_write_macros.h"
namespace IPC {
#include "clawer_driver/src/clawer_messages.h"
}  // namespace IPC

// Generate param traits read methods.
#include "ipc/param_traits_read_macros.h"
namespace IPC {
#include "clawer_driver/src/clawer_messages.h"
}  // namespace IPC

// Generate param traits log methods.
#include "ipc/param_traits_log_macros.h"
namespace IPC {
#include "clawer_driver/src/clawer_messages.h"
}  // namespace IPC
