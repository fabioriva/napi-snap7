#ifndef SNAP7_CLIENT_H
#define SNAP7_CLIENT_H

// #include <iostream>
#include <napi.h>
#include "snap7.h"
#include "callback.h"
#include "promise.h"

class S7Client : public Napi::ObjectWrap<S7Client>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    S7Client(const Napi::CallbackInfo &info);
    ~S7Client();
    // Control functions
    Napi::Value Connect(const Napi::CallbackInfo &info);
    Napi::Value ConnectTo(const Napi::CallbackInfo &info);
    Napi::Value Disconnect(const Napi::CallbackInfo &info);
    Napi::Value SetConnectionParams(const Napi::CallbackInfo &info);
    Napi::Value SetConnectionType(const Napi::CallbackInfo &info);
    // Data I/O Main functions
    Napi::Value ReadAreaSync(const Napi::CallbackInfo &info);
    Napi::Value ReadArea(const Napi::CallbackInfo &info);
    Napi::Value WriteAreaSync(const Napi::CallbackInfo &info);
    Napi::Value WriteArea(const Napi::CallbackInfo &info);
    // Data I/O Lean functions
    Napi::Value DBReadSync(const Napi::CallbackInfo &info);
    Napi::Value DBRead(const Napi::CallbackInfo &info);
    Napi::Value DBWriteSync(const Napi::CallbackInfo &info);
    Napi::Value DBWrite(const Napi::CallbackInfo &info);
    // Utility functions
    Napi::Value GetConnected(const Napi::CallbackInfo &info);
    Napi::Value GetLastError(const Napi::CallbackInfo &info);
    Napi::Value ErrorText(const Napi::CallbackInfo &info);
    Napi::Value ExecTime(const Napi::CallbackInfo &info);

private:
    TS7Client *Client;

    int GetWordSize(int WordLen);
    void ParametersCheck(const Napi::CallbackInfo &info, int length);
    void ParametersCheck(int Area, int DBNumber, int Start, int Amount, int WordLen);
    void ParametersCheck(int DBNumber, int Start, int Amount);
    void ParametersCheck(int Start, int Amount);
};

#endif