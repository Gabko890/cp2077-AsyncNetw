#include "cpAsyncNetw.h"

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>


RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle aHandle,
                                        RED4ext::EMainReason aReason,
                                        const RED4ext::Sdk* aSdk) {
  switch (aReason)  {
  case RED4ext::EMainReason::Load: {
    // It will automatically register types declared below.

    Red::TypeInfoRegistrar::RegisterDiscovered();
    break;
  }
  case RED4ext::EMainReason::Unload: {
    break;
  }
  }
  return true;
}



RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo* aInfo)
{
    aInfo->name = L"cpAsyncNetw";
    aInfo->author = L"WonTeu";
    aInfo->version = RED4EXT_SEMVER(0, 0, 1); // Set your version here.
    aInfo->runtime = RED4EXT_RUNTIME_LATEST;
    aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports()
{
    return RED4EXT_API_VERSION_LATEST;
}