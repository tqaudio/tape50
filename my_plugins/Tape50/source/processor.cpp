#include "../include/processor.h"

namespace Tape50 {
Processor::Processor() { setControllerClass(ControllerID); }

tresult PLUGIN_API Processor::initialize(FUnknown *context) {
  tresult result = AudioEffect::initialize(context);

  if (result != kResultTrue) {
    return kResultFalse;
  }

  addEventInput(STR16("Event Input"), 1);
  addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
  addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);

  return kResultTrue;
}

tresult PLUGIN_API Processor::setBusArrangements(SpeakerArrangement *inputs,
                                                 int32 numIns,
                                                 SpeakerArrangement *outputs,
                                                 int32 numOuts) {
  if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0]) {
    return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
  }

  return kResultFalse;
}

tresult PLUGIN_API Processor::setupProcessing(ProcessSetup &newSetup) {
  return AudioEffect::setupProcessing(newSetup);
}

tresult PLUGIN_API Processor::setActive(TBool state) {
  SpeakerArrangement arrangement;

  if (getBusArrangement(kOutput, 0, arrangement) != kResultTrue) {
    return kResultFalse;
  }

  int channelCount = SpeakerArr::getChannelCount(arrangement);

  if (channelCount != 2) {
    return kResultFalse;
  }
  if (state) {
    mBuffer = new Buffer *[channelCount];
    mNoteOns = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mNoteOffs = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mRatios = new AutomationParameter[processSetup.maxSamplesPerBlock];
    mDurations = new AutomationParameter[processSetup.maxSamplesPerBlock];

    for (int channel = 0; channel < channelCount; channel++) {
      mBuffer[channel] =
          new Buffer(processSetup.sampleRate, Constants::maxDuration);
      mBuffer[channel]->setRatio(1.0 - mRatio);
      mBuffer[channel]->setDuration(mDuration * Constants::maxDuration);
    }
  } else {
    for (int channel = 0; channel < channelCount; channel++) {
      delete mBuffer[channel];
    }

    delete[] mNoteOns;
    delete[] mNoteOffs;
    delete[] mRatios;
    delete[] mDurations;
    delete mBuffer;
  }

  return AudioEffect::setActive(state);
}

tresult PLUGIN_API Processor::process(ProcessData &data) {
  if (data.inputParameterChanges) {
    int32 numParamsChanged = data.inputParameterChanges->getParameterCount();

    for (int32 index = 0; index < numParamsChanged; index++) {
      IParamValueQueue *paramQueue =
          data.inputParameterChanges->getParameterData(index);

      if (paramQueue == nullptr) {
        continue;
      }

      int32 numPoints = paramQueue->getPointCount();
      int32 sampleOffset;
      ParamValue value;

      if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) !=
          kResultTrue) {
        continue;
      }
      if (sampleOffset < 0) {
        sampleOffset = 0;
      }
      if (sampleOffset > data.numSamples - 1) {
        sampleOffset = data.numSamples - 1;
      }
      switch (paramQueue->getParameterId()) {
      case Parameters::kBypassId:
        mBypass = (value > 0.5);
        break;
      case Parameters::kRatioId:
        mRatio = value;
        mRatios[sampleOffset].value = value;
        mRatios[sampleOffset].hasChanged = true;
        break;
      case Parameters::kDurationId:
        mDuration = value;
        mDurations[sampleOffset].value = value;
        mDurations[sampleOffset].hasChanged = true;
        break;
      }
    }
  }

  if (data.inputEvents) {
    int eventCount = data.inputEvents->getEventCount();

    for (int i = 0; i < eventCount; i++) {
      Event incomingEvent;

      if (data.inputEvents->getEvent(i, incomingEvent) != kResultOk) {
        continue;
      }

      int sampleOffset = incomingEvent.sampleOffset;

      if (sampleOffset < 0) {
        sampleOffset = 0;
      }
      if (sampleOffset > data.numSamples - 1) {
        sampleOffset = data.numSamples - 1;
      }
      switch (incomingEvent.type) {
      case Event::kNoteOnEvent:
        mNoteOns[sampleOffset].hasChanged = true;
        break;
      case Event::kNoteOffEvent:
        mNoteOffs[sampleOffset].hasChanged = true;
        break;
      }
    }
  }
  if (data.numInputs == 0 || data.numOutputs == 0) {
    return kResultTrue;
  }
  if (data.numSamples > 0) {
    SpeakerArrangement arrangement;
    getBusArrangement(kOutput, 0, arrangement);
    int32 channelCount = SpeakerArr::getChannelCount(arrangement);
    float *inputChannel = nullptr;
    float *outputChannel = nullptr;

    for (int32 sample = 0; sample < data.numSamples; sample++) {
      for (int32 channel = 0; channel < channelCount; channel++) {
        inputChannel = data.inputs[0].channelBuffers32[channel];
        outputChannel = data.outputs[0].channelBuffers32[channel];

        if (mNoteOns[sample].hasChanged) {
          mBuffer[channel]->tapeStop();
          if (channel == channelCount - 1) {
            mNoteOns[sample].hasChanged = false;
          }
        }
        if (mNoteOffs[sample].hasChanged) {
          mBuffer[channel]->tapeStart();
          if (channel == channelCount - 1) {
            mNoteOffs[sample].hasChanged = false;
          }
        }
        if (mRatios[sample].hasChanged) {
          mBuffer[channel]->setRatio(1.0 - mRatios[sample].value);
          if (channel == channelCount - 1) {
            mRatios[sample].hasChanged = false;
          }
        }
        if (mDurations[sample].hasChanged) {
          mBuffer[channel]->setDuration(mDurations[sample].value *
                                        Constants::maxDuration);
          if (channel == channelCount - 1) {
            mDurations[sample].hasChanged = false;
          }
        }

        mBuffer[channel]->write(inputChannel[sample]);
        outputChannel[sample] = mBuffer[channel]->read();
      }
    }
  }

  return kResultOk;
}

tresult PLUGIN_API Processor::setState(IBStream *state) {
  if (!state) {
    return kResultFalse;
  }

  IBStreamer streamer(state, kLittleEndian);
  int32 savedBypass{0};
  float savedRatio{0.0};
  float savedDuration{0.0};

  if (!streamer.readInt32(savedBypass)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedRatio)) {
    return kResultFalse;
  }
  if (!streamer.readFloat(savedDuration)) {
    return kResultFalse;
  }

  mBypass = savedBypass > 0;
  mRatio = savedRatio;
  mDuration = savedDuration;

  return kResultOk;
}

tresult PLUGIN_API Processor::getState(IBStream *state) {
  int32 saveBypass = mBypass ? 1 : 0;
  float saveRatio = mRatio;
  float saveDuration = mDuration;

  IBStreamer streamer(state, kLittleEndian);

  streamer.writeInt32(saveBypass);
  streamer.writeFloat(saveRatio);
  streamer.writeFloat(saveDuration);

  return kResultOk;
}
} // namespace Tape50
