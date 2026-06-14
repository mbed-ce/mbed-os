#pragma once

#include "platform/FileHandle.h"
#include "SEGGER_RTT.h"

class RTTConsole : public mbed::FileHandle {
public:
    RTTConsole() {}

    virtual ssize_t write(const void *buffer, size_t size) override {
        return SEGGER_RTT_Write(0, buffer, size);
    }

    virtual ssize_t read(void *buffer, size_t size) override {
        return SEGGER_RTT_Read(0, buffer, size);
    }

    virtual off_t seek(off_t offset, int whence = SEEK_SET) override {
        return -ESPIPE; // not seekable
    }

    virtual int close() override {
        return 0;
    }

    virtual short poll(short events) const override {
        return POLLIN | POLLOUT;
    }

    virtual int isatty() override {
        return true;
    }
};
