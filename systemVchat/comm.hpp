#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

constexpr const char* PATHNAME = "/tmp";
constexpr int PROJ_ID = 0x66;

struct Message {
    long mtype;
    char mtext[128];
};