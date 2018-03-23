#pragma once

#include "sim/Simulator.h"

#include <functional>
#include <memory>

class ClockSource;

class Dio {
public:
    struct Input {
        typedef std::function<void(bool)> Handler;

        bool get() const { return _value; }
        void setHandler(Handler handler) { _handler = handler; }

    private:
        void set(bool value) {
            if (_handler && value != _value) {
                _handler(value);
            }
            _value = value;
        }

        bool _value = false;
        Handler _handler;

        friend class Dio;
    };

    struct Output {
        void set(bool value) {}
    };

    Dio();
    ~Dio();

    void init();

    Input clockInput;
    Input resetInput;

    Output clockOutput;
    Output resetOutput;

private:
    sim::Simulator &_simulator;
    std::unique_ptr<ClockSource> _clockSource;
};
