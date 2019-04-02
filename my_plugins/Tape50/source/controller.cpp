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
      new LinearParameter("Duration", "ms", MAX_DURATION, 2,
                          ParameterInfo::kCanAutomate, Parameters::kDurationId);
  parameters.addParameter(duration);

  MIDIChannelParameter *noteChannel =
      new MIDIChannelParameter("Note On Channel", ParameterInfo::kCanAutomate,
                               Parameters::kNoteChannelId);
  parameters.addParameter(noteChannel);

  NoteNumberParameter *noteName = new NoteNumberParameter(
      "Note Name", ParameterInfo::kCanAutomate, Parameters::kNoteNumberId);
  parameters.addParameter(noteName);

  return result;
}

tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);

  int32 savedBypass;
  float savedRatio = 0.0f;
  float savedDuration = 0.0f;
  float savedNoteChannel = 0.0f;
  float savedNoteNumber = 0.0f;

  if (!streamer.readInt32(savedBypass)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedRatio)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedDuration)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedNoteChannel)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedNoteNumber)) {
    return kResultFalse;
  }

  setParamNormalized(Parameters::kBypassId, savedBypass ? 1 : 0);
  setParamNormalized(Parameters::kRatioId, savedRatio);
  setParamNormalized(Parameters::kDurationId, savedDuration);
  setParamNormalized(Parameters::kNoteChannelId, savedNoteChannel);
  setParamNormalized(Parameters::kNoteNumberId, savedNoteNumber);

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
