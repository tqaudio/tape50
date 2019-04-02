#include "../include/buffer.h"

namespace Tape50 {
Buffer::Buffer(int sampleRate, double capacity /* ms */)
    : mSampleRate(sampleRate), mCapacity(sampleRate * capacity / 1000.0f),
      mReadHead(0.0f), mWriteHead(0), mTapeStopTriggered(false),
      mTapeStartTriggered(false), mRatio(0.0f), mMiddlePoint(0) {
  mBuffer = new double[mCapacity];
}

Buffer::~Buffer() { delete[] mBuffer; }

double Buffer::read() {
  if (!mTapeStopTriggered) {
    return mBuffer[0];
  }
  if (!mTapeStartTriggered && mWriteHead > mMiddlePoint - 1) {
    return 0.0f;
  }

  int index = floor(mReadHead);
  double ratio = mReadHead - (double)index;
  double result =
      mBuffer[index] + ratio * (mBuffer[index + 1] - mBuffer[index]);

  double diff = 0.0f;

  if (mWriteHead < mMiddlePoint) {
    diff = 1.0f - (double)mWriteHead / (double)mMiddlePoint;
  } else {
    diff =
        (double)(mWriteHead - mMiddlePoint) / (double)(mLength - mMiddlePoint);
  }

  mReadHead += diff;

  double wetGain = 1.0f - pow(1.0f - diff, 9.42f);
  double wetFadeOut = 1.0f;
  double dryFadeIn = 0.0f;

  if (mWriteHead > mMiddlePoint) {
    wetFadeOut = 1.0f - pow((double)mWriteHead / (double)mLength, 9.42f);
    dryFadeIn = pow((double)(mWriteHead - mMiddlePoint) /
                        (double)(mLength - mMiddlePoint),
                    9.42f);
  }

  return result * wetGain * wetFadeOut + mBuffer[mWriteHead - 1] * dryFadeIn;
}

void Buffer::write(double value) {
  mBuffer[mWriteHead] = value;

  if (!mTapeStopTriggered && !mTapeStartTriggered ||
      !mTapeStartTriggered && mWriteHead > mMiddlePoint - 1) {
    return;
  }

  mWriteHead += 1;

  if (mWriteHead > mLength - 1) {
    if (!mTapeStartTriggered) {
      return;
    }
    mTapeStopTriggered = false;
    mTapeStartTriggered = false;
    mReadHead = 0.0f;
    mWriteHead = 0;
  }
}

void Buffer::tapeStop() { mTapeStopTriggered = true; }

void Buffer::tapeStart() {
  if (mTapeStopTriggered) {
    mTapeStartTriggered = true;
  }
}

void Buffer::setRatio(double ratio) {
  if (ratio > 1.0f) {
    ratio = 1.0f;
  }
  if (ratio < 0.0f) {
    ratio = 0.0f;
  }

  mRatio = ratio;
  mMiddlePoint = (int)((double)mLength * mRatio);
}

void Buffer::setDuration(double duration /* ms */) {
  int samples = floor(mSampleRate * duration / 1000.0f);

  if (samples > mCapacity - 1) {
    samples = mCapacity - 1;
  }

  mLength = samples;
  mMiddlePoint = floor(mLength * mRatio);
}
} // namespace Tape50
