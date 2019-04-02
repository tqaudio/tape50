#pragma once

#include <math.h>

namespace Tape50 {
class Buffer {
public:
  Buffer(int sampleRate, double capacity /* ms */);
  ~Buffer();

  double read();
  void write(double value);

  void tapeStop();
  void tapeStart();

  void setRatio(double ratio);
  void setDuration(double duration);

protected:
  int mSampleRate;
  int mCapacity;
  int mLength;
  double mReadHead;
  int mWriteHead;

  int mMiddlePoint;
  double mRatio;

  bool mTapeStopTriggered;
  bool mTapeStartTriggered;

  double *mBuffer;
};
} // namespace Tape50
