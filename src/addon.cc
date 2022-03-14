#include <napi.h>
#include "snap7_client.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
  return S7Client::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)