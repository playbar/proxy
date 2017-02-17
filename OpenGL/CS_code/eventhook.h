
#ifndef EVENTHOOK_H
#define EVENTHOOK_H

#include "engine/event_args.h"

void HookEvent(char *name, void (*pfnEvent)(struct event_args_s *args));

#endif
