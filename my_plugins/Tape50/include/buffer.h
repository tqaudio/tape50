#pragma once

namespace Tape50 {
class Buffer {
public:
  Buffer(int sampleRate, double maxDuration /* ms */);
  ~Buffer();

  double read();
  void write(double value);

  void tapeStop();
  void tapeStart();

  void setRatio(double ratio);
  void setDuration(double duration);

private:
  int mSampleRate; // e.g. 44100 (Hz)
  int mCapacity;   // e.g. 441000 (samples)
  int mLength = 0;
  double mReadHead = 0.0;
  int mWriteHead = 0;

  int mMiddlePoint = 0;
  double mRatio = 0.0;

  bool mTapeStopTriggered = false;
  bool mTapeStartTriggered = false;

  double *mBuffer = nullptr;
};
} // namespace Tape50
