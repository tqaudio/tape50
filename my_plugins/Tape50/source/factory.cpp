#include "public.sdk/source/main/pluginfactory.h"

#include "../include/controller.h"
#include "../include/id.h"
#include "../include/processor.h"
#include "../include/version.h"

BEGIN_FACTORY_DEF(stringCompanyName, stringCompanyWeb, stringCompanyEmail)
DEF_CLASS2(INLINE_UID_FROM_FUID(Tape50::ProcessorID),
           PClassInfo::kManyInstances, kVstAudioEffectClass, stringPluginName,
           Vst::kDistributable, Vst::PlugType::kFx, FULL_VERSION_STR,
           kVstVersionString, Tape50::Processor::createInstance)
DEF_CLASS2(INLINE_UID_FROM_FUID(Tape50::ControllerID),
           PClassInfo::kManyInstances, kVstComponentControllerClass,
           stringPluginName "Controller", 0, "", FULL_VERSION_STR,
           kVstVersionString, Tape50::Controller::createInstance)
END_FACTORY

bool InitModule() { return true; }

bool DeinitModule() { return true; }
