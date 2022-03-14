#include "snap7_client.h"

Napi::Object S7Client::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func =
        DefineClass(env,
                    "S7Client",
                    {
                        InstanceMethod("Connect", &S7Client::Connect),
                        InstanceMethod("ConnectTo", &S7Client::ConnectTo),
                        InstanceMethod("Disconnect", &S7Client::Disconnect),
                        InstanceMethod("SetConnectionParams", &S7Client::SetConnectionParams),
                        InstanceMethod("SetConnectionType", &S7Client::SetConnectionType),
                        InstanceMethod("ReadAreaSync", &S7Client::ReadAreaSync),
                        InstanceMethod("ReadArea", &S7Client::ReadArea),
                        InstanceMethod("WriteAreaSync", &S7Client::WriteAreaSync),
                        InstanceMethod("WriteArea", &S7Client::WriteArea),
                        InstanceMethod("GetConnected", &S7Client::GetConnected),
                        InstanceMethod("GetLastError", &S7Client::GetLastError),
                        InstanceMethod("ErrorText", &S7Client::ErrorText),
                        InstanceMethod("ExecTime", &S7Client::ExecTime),
                    });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);

    exports.Set("S7Client", func);
    return exports;
}
//---------------------------------------------------------------------------
S7Client::S7Client(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<S7Client>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    Client = new TS7Client();
}
//---------------------------------------------------------------------------
S7Client::~S7Client()
{
    delete Client;
}
//---------------------------------------------------------------------------
Napi::Value S7Client::Connect(const Napi::CallbackInfo &info)
{
    int res = Client->Connect();
    return Napi::Number::New(info.Env(), res);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::ConnectTo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 3)
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    // const char *RemAddress = info[0].As<Napi::String>().Utf8Value().c_str(); // not for windows!
    std::string str = info[0].As<Napi::String>().Utf8Value();
    const char *RemAddress = str.c_str();
    int Rack = info[1].As<Napi::Number>().Int32Value();
    int Slot = info[2].As<Napi::Number>().Int32Value();
    int res = Client->ConnectTo(RemAddress, Rack, Slot);

    return Napi::Number::New(env, res);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::Disconnect(const Napi::CallbackInfo &info)
{
    int res = Client->Disconnect();
    return Napi::Number::New(info.Env(), res);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::SetConnectionParams(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 3)
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    // const char *Address = info[0].As<Napi::String>().Utf8Value().c_str(); // not for windows!
    std::string str = info[0].As<Napi::String>().Utf8Value() ;
    const char *Address = str.c_str();
    int LocalTSAP = info[1].As<Napi::Number>().Uint32Value();
    int RemoteTSAP = info[2].As<Napi::Number>().Uint32Value();
    int res = Client->SetConnectionParams(Address, LocalTSAP, RemoteTSAP);
    return Napi::Number::New(env, res);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::SetConnectionType(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "ConnectionType PG=0x01, OP=0x02, S7=0x03..0x10").ThrowAsJavaScriptException();
        return env.Null();
    }

    word ConnectionType = info[0].As<Napi::Number>().Uint32Value();
    int res = Client->SetConnectionType(ConnectionType);
    return Napi::Number::New(env, res);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::ReadAreaSync(const Napi::CallbackInfo &info)
{
    ParametersCheck(info, 5);

    Napi::Env env = info.Env();

    int Area = info[0].As<Napi::Number>().Int32Value();
    int DBNumber = info[1].As<Napi::Number>().Int32Value();
    int Start = info[2].As<Napi::Number>().Int32Value();
    int Amount = info[3].As<Napi::Number>().Int32Value();
    int WordLen = info[4].As<Napi::Number>().Int32Value();

    int WordSize = GetWordSize(WordLen);
    int size = Amount * WordSize;
    uint8_t *pBuffer = new uint8_t[size];

    int res = Client->ReadArea(Area, DBNumber, Start, Amount, WordLen, pBuffer);

    if (res == 0)
    {
        return Napi::Buffer<uint8_t>::New(env, pBuffer, size);
    }
    else
    {
        TextString err = CliErrorText(res);
        Napi::TypeError::New(env, err).ThrowAsJavaScriptException();
        return env.Null();
    }
}
//---------------------------------------------------------------------------
Napi::Value S7Client::ReadArea(const Napi::CallbackInfo &info)
{
    ParametersCheck(info, 5);

    Napi::Env env = info.Env();

    int Area = info[0].As<Napi::Number>().Int32Value();
    int DBNumber = info[1].As<Napi::Number>().Int32Value();
    int Start = info[2].As<Napi::Number>().Int32Value();
    int Amount = info[3].As<Napi::Number>().Int32Value();
    int WordLen = info[4].As<Napi::Number>().Int32Value();
    int WordSize = GetWordSize(WordLen);

    if (info[5].IsFunction())
    {
        Napi::Function callback = info[5].As<Function>();
        ReadCallbackWorker *wk = new ReadCallbackWorker(callback, Client, Area, DBNumber, Start, Amount, WordLen, WordSize);
        wk->Queue();
        return env.Undefined();
    }
    else
    {
        ReadPromiseWorker *wk = new ReadPromiseWorker(env, Client, Area, DBNumber, Start, Amount, WordLen, WordSize);
        auto promise = wk->GetPromise();
        wk->Queue();
        return promise;
    }
}
//---------------------------------------------------------------------------
Napi::Value S7Client::WriteAreaSync(const Napi::CallbackInfo &info)
{
    ParametersCheck(info, 5);

    Napi::Env env = info.Env();

    int Area = info[0].As<Napi::Number>().Int32Value();
    int DBNumber = info[1].As<Napi::Number>().Int32Value();
    int Start = info[2].As<Napi::Number>().Int32Value();
    int Amount = info[3].As<Napi::Number>().Int32Value();
    int WordLen = info[4].As<Napi::Number>().Int32Value();

    Napi::Uint8Array arr = info[5].As<Napi::Uint8Array>();
    // size_t length = arr.ElementLength(); // == size
    uint8_t *Buffer = reinterpret_cast<uint8_t *>(arr.ArrayBuffer().Data());

    int res = Client->WriteArea(Area, DBNumber, Start, Amount, WordLen, Buffer);

    return Napi::Number::New(env, res);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::WriteArea(const Napi::CallbackInfo &info)
{
    ParametersCheck(info, 5);

    Napi::Env env = info.Env();

    int Area = info[0].As<Napi::Number>().Int32Value();
    int DBNumber = info[1].As<Napi::Number>().Int32Value();
    int Start = info[2].As<Napi::Number>().Int32Value();
    int Amount = info[3].As<Napi::Number>().Int32Value();
    int WordLen = info[4].As<Napi::Number>().Int32Value();

    Napi::Uint8Array arr = info[5].As<Napi::Uint8Array>();
    // size_t length = arr.ElementLength(); // == size
    uint8_t *Buffer = reinterpret_cast<uint8_t *>(arr.ArrayBuffer().Data());

    if (info[6].IsFunction())
    {
        Napi::Function callback = info[6].As<Function>();
        WriteCallbackWorker *wk = new WriteCallbackWorker(callback, Client, Area, DBNumber, Start, Amount, WordLen, Buffer);
        wk->Queue();
        return env.Undefined();
    }
    else
    {
        WritePromiseWorker *wk = new WritePromiseWorker(env, Client, Area, DBNumber, Start, Amount, WordLen, Buffer);
        auto promise = wk->GetPromise();
        wk->Queue();
        return promise;
    }
}
//---------------------------------------------------------------------------

Napi::Value S7Client::GetConnected(const Napi::CallbackInfo &info)
{
    bool connected = Client->Connected();
    return Napi::Boolean::New(info.Env(), connected);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::GetLastError(const Napi::CallbackInfo &info)
{
    int res = Client->LastError();
    return Napi::Boolean::New(info.Env(), res);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::ErrorText(const Napi::CallbackInfo &info)
{
    double arg0 = info[0].As<Napi::Number>().DoubleValue();

    TextString err = CliErrorText(arg0);
    return Napi::String::New(info.Env(), err);
}
//---------------------------------------------------------------------------
Napi::Value S7Client::ExecTime(const Napi::CallbackInfo &info)
{
    int time = Client->ExecTime();
    return Napi::Number::New(info.Env(), time);
}
//---------------------------------------------------------------------------
void S7Client::ParametersCheck(const Napi::CallbackInfo &info, int length)
{
    Napi::Env env = info.Env();

    if (info.Length() < length)
    {
        Napi::TypeError::New(env, "Usage: Area, DBNumber, Start, Amount, WordLen").ThrowAsJavaScriptException();
        return;
    }
    if (!info[0].IsNumber()) // Area
    {
        Napi::TypeError::New(env, "Area must be of type integer").ThrowAsJavaScriptException();
        return;
    }
    if (!info[1].IsNumber()) // DBNumber
    {
        Napi::TypeError::New(env, "DBNumber must be of type integer").ThrowAsJavaScriptException();
        return;
    }
    if (!info[2].IsNumber()) // Start
    {
        Napi::TypeError::New(env, "Start must be of type integer").ThrowAsJavaScriptException();
        return;
    }
    if (!info[3].IsNumber()) // Amount
    {
        Napi::TypeError::New(env, "Amount must be of type integer").ThrowAsJavaScriptException();
        return;
    }
    if (!info[4].IsNumber()) // WordLen
    {
        Napi::TypeError::New(env, "WordLen must be of type integer").ThrowAsJavaScriptException();
        return;
    }
}
//---------------------------------------------------------------------------
void S7Client::ParametersCheck(int Area, int DBNumber, int Start, int Amount, int WordLen)
{
}
//---------------------------------------------------------------------------
int S7Client::GetWordSize(int WordLen)
{
    switch (WordLen)
    {
    case S7WLBit:
    case S7WLByte:
        return 1;
    case S7WLWord:
    case S7WLCounter:
    case S7WLTimer:
        return 2;
    case S7WLDWord:
    case S7WLReal:
        return 4;
    default:
        return 0;
    }
}
//---------------------------------------------------------------------------