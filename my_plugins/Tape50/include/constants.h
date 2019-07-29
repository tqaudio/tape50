#pragma once

namespace Tape50 {
namespace Constants {
constexpr double defaultRatio{50.0 /* percentage */};
constexpr double defaultRatioNormalized{defaultRatio / 100.0};
constexpr double maxDuration{1000000.0 /* ms */};
constexpr double defaultDuration{1000.0 /* ms */};
constexpr double defaultDurationNormalized{defaultDuration / maxDuration};
} // namespace Constants
} // namespace Tape50
