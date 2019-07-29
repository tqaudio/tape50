#include "../include/constants.h"
#include "../include/controller.h"

namespace Tape50 {
tresult PLUGIN_API Controller::initialize(FUnknown *context) {
  tresult result = EditController::initialize(context);

  if (result != kResultOk) {
    return result;
  }

  parameters.addParameter(STR16("Bypass"), 0, 1, 0,
                          ParameterInfo::kCanAutomate |
                              ParameterInfo::kIsBypass,
                          Parameters::kBypassId);

  LinearParameter *ratio =
      new LinearParameter("Ratio", "", 100.0f, 2, ParameterInfo::kCanAutomate,
                          Parameters::kRatioId);
  parameters.addParameter(ratio);

  LinearParameter *duration =
      new LinearParameter("Duration", "ms", Constants::maxDuration, 2,
                          ParameterInfo::kCanAutomate, Parameters::kDurationId);
  parameters.addParameter(duration);

  return result;
}

tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);

  int32 savedBypass;
  float savedRatio;
  float savedDuration;

  if (!streamer.readInt32(savedBypass)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedRatio)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedDuration)) {
    return kResultFalse;
  }

  setParamNormalized(Parameters::kBypassId, savedBypass ? 1 : 0);
  setParamNormalized(Parameters::kRatioId, savedRatio);
  setParamNormalized(Parameters::kDurationId, savedDuration);

  return kResultOk;
}

IPlugView *PLUGIN_API Controller::createView(const char *name) {
  if (name && strcmp(name, "editor") == 0) {
    auto *view = new VST3Editor(this, "Tape50", "main.uidesc");
    return view;
  }
  return nullptr;
}
} // namespace Tape50
