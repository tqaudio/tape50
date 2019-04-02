#include "../include/parameter.h"
#include "../include/constants.h"

namespace Tape50 {
LinearParameter::LinearParameter(const char *title, const char *unit,
                                 double maxValue, int32 precision, int32 flags,
                                 int32 id)
    : mMaxValue(maxValue), mPrecision(precision) {
  UString(info.title, USTRINGSIZE(info.title)).fromAscii(title);
  UString(info.units, USTRINGSIZE(info.units)).fromAscii(unit);
  info.flags = flags;
  info.id = id;
  info.stepCount = 0;
  info.unitId = kRootUnitId;
}

void LinearParameter::toString(ParamValue normValue, String128 text) const {
  UString(text, 128).printFloat(normValue * mMaxValue, mPrecision);
}

bool LinearParameter::fromString(const TChar *text,
                                 ParamValue &normValue) const {
  double value = 0.0f;
  UString wrapper((TChar *)text, -1);

  if (!wrapper.scanFloat(value)) {
    return false;
  }
  if (value < 0.0f) {
    value = 0.0f;
  }
  if (value > mMaxValue) {
    value = mMaxValue;
  }

  normValue = value / mMaxValue;

  return true;
}

NoteNumberParameter::NoteNumberParameter(const char *title, int32 flags,
                                         int32 id) {
  UString(info.title, USTRINGSIZE(info.title)).fromAscii(title);
  info.flags = flags;
  info.id = id;
  info.stepCount = 129;
  info.unitId = kRootUnitId;
}

void NoteNumberParameter::toString(ParamValue normValue, String128 text) const {
  char buffer[32];

  int note = (int)(round(normValue * 128.0f));

  if (note > 127 || note < 0.0f) {
    sprintf(buffer, "ALL");
  } else {
    int octave = note / 12 - 2;

    sprintf(buffer, "%s%d", Notes[note % 12], octave);
  }

  UString(text, 128).fromAscii(buffer);
}

bool NoteNumberParameter::fromString(const TChar *text,
                                     ParamValue &normValue) const {
  char buffer[32];

  UString wrapper((TChar *)text, -1);
  wrapper.toAscii(buffer, 32);

  for (int i = 0; i < NoteNameLength; i++) {
    if (strcmp(buffer, NoteName[i].Name) == 0) {
      normValue = NoteName[i].Value;
      return true;
    }
  }

  normValue = 1.0f;

  return true;
}

MIDIChannelParameter::MIDIChannelParameter(const char *title, int32 flags,
                                           int32 id) {
  UString(info.title, USTRINGSIZE(info.title)).fromAscii(title);
  info.flags = flags;
  info.id = id;
  info.stepCount = 17;
  info.unitId = kRootUnitId;
}

void MIDIChannelParameter::toString(ParamValue normValue,
                                    String128 text) const {
  if (normValue == 0.0f) {
    UString(text, 128).fromAscii("OMNI");
  } else {
    UString(text, 128).printInt((int)(normValue * 16.0f));
  }
}

bool MIDIChannelParameter::fromString(const TChar *text,
                                      ParamValue &normValue) const {
  double channel = 0.0f;
  UString wrapper((TChar *)text, -1);

  if (!wrapper.scanFloat(channel)) {
    return false;
  }
  if (channel < 0.0f) {
    channel = 0.0f;
  }
  if (channel > 16.0f) {
    channel = 16.0f;
  }

  normValue = channel / 16.0f;

  return true;
}
} // namespace Tape50
