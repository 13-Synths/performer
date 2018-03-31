#include "CurveTrackEngine.h"

#include "Curve.h"

#include "core/Debug.h"
#include "core/utils/Random.h"
#include "core/math/Math.h"

#include "engine/SequenceUtils.h"

static Random rng;

static float evalStepShape(const CurveSequence::Step &step, float fraction) {
    auto function = Curve::function(Curve::Type(step.shape()));
    float value = function(fraction);
    float min = float(step.min()) / CurveSequence::Min::Max;
    float max = float(step.max()) / CurveSequence::Max::Max;
    return min + value * (max - min);
}

void CurveTrackEngine::reset() {
    _sequenceState.reset();
    _currentStep = -1;
    _currentStepFraction = 0.f;
    _lastRange = CurveSequence::Range::Last;
    changePattern();
}

void CurveTrackEngine::tick(uint32_t tick) {
    ASSERT(_sequence != nullptr, "invalid sequence");
    const auto &sequence = *_sequence;
    const auto *linkData = _linkedTrackEngine ? _linkedTrackEngine->linkData() : nullptr;

    // update range values
    if (sequence.range() != _lastRange) {
        _range = CurveSequence::rangeValues(sequence.range());
        _lastRange = sequence.range();
    }

    if (linkData) {
        _linkData = *linkData;
        _sequenceState = *linkData->sequenceState;
        updateOutput(linkData->relativeTick, linkData->divisor);
    } else {
        uint32_t divisor = sequence.divisor() * (CONFIG_PPQN / CONFIG_SEQUENCE_PPQN);
        uint32_t measureDivisor = (sequence.resetMeasure() * CONFIG_PPQN * 4);
        uint32_t relativeTick = measureDivisor == 0 ? tick : tick % measureDivisor;

        // handle reset measure
        if (relativeTick == 0) {
            reset();
        }

        relativeTick %= divisor;

        if (relativeTick == 0) {
            // advance sequence
            switch (_curveTrack.playMode()) {
            case Types::PlayMode::Free:
                _sequenceState.advanceFree(sequence.runMode(), sequence.firstStep(), sequence.lastStep(), rng);
                break;
            case Types::PlayMode::Aligned:
                _sequenceState.advanceAligned(tick / divisor, sequence.runMode(), sequence.firstStep(), sequence.lastStep(), rng);
                break;
            case Types::PlayMode::Last:
                break;
            }
        }

        updateOutput(relativeTick, divisor);

        _linkData.divisor = divisor;
        _linkData.relativeTick = relativeTick;
        _linkData.sequenceState = &_sequenceState;
    }
}

void CurveTrackEngine::changePattern() {
    _sequence = &_curveTrack.sequence(_pattern);
}

void CurveTrackEngine::updateOutput(uint32_t relativeTick, uint32_t divisor) {
    if (_sequenceState.step() < 0) {
        return;
    }

    int rotate = _curveTrack.rotate();

    const auto &sequence = *_sequence;
    _currentStep = SequenceUtils::rotateStep(_sequenceState.step(), sequence.firstStep(), sequence.lastStep(), rotate);
    const auto &step = sequence.step(_currentStep);

    _currentStepFraction = float(relativeTick) / divisor;

    float value = evalStepShape(step, _currentStepFraction);
    value = _range[0] + value * (_range[1] - _range[0]);
    _cvOutput = value;
}
