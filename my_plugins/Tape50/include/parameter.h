#pragma once

#define DEFAULT_RATIO 50.0f /* percentage */
#define DEFAULT_RATIO_NORMALIZED DEFAULT_RATIO / 100.0f

#define MAX_DURATION 1000000.0f /* ms */

#define DEFAULT_DURATION 1000.0f /* ms */
#define DEFAULT_DURATION_NORMALIZED DEFAULT_DURATION / MAX_DURATION

#define DEFAULT_MIDI_CHANNEL_NORMALIZED 0.0f /* OMNI */
#define DEFAULT_MIDI_NOTE_NORMALIZED 1.0f    /* ALL */

#include "pluginterfaces/base/ustring.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include <math.h>

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Tape50 {
enum Parameters : ParamID {
  kBypassId = 100,
  kRatioId = 101,
  kDurationId = 102,
  kNoteChannelId = 103,
  kNoteNumberId = 104
};

struct AutomationParameter {
public:
  ParamValue value;
  bool hasChanged;
  AutomationParameter() : value(0.0f), hasChanged(false){};
};

class LinearParameter : public Parameter {
public:
  LinearParameter(const char *title, const char *unit, double maxValue,
                  int32 precision, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;

private:
  double mMaxValue;
  int32 mPrecision;
};

class NoteNumberParameter : public Parameter {
public:
  NoteNumberParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};

class MIDIChannelParameter : public Parameter {
public:
  MIDIChannelParameter(const char *title, int32 flags, int32 id);
  void toString(ParamValue normValue, String128 text) const SMTG_OVERRIDE;
  bool fromString(const TChar *string,
                  ParamValue &normValue) const SMTG_OVERRIDE;
};
} // namespace Tape50
