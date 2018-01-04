#include "SystemConfig.h"

#include "drivers/ADC.h"
#include "drivers/ButtonLedMatrix.h"
#include "drivers/Console.h"
#include "drivers/DAC.h"
#include "drivers/DIO.h"
#include "drivers/GateOutput.h"
#include "drivers/LCD.h"
#include "drivers/MIDI.h"
#include "drivers/System.h"
#include "drivers/ShiftRegister.h"
#include "drivers/DebugLed.h"
#include "drivers/HighResolutionTimer.h"
#include "drivers/USBH.h"
#include "drivers/USBMIDI.h"
#include "drivers/ClockTimer.h"

#include "os/os.h"

#include "core/profiler/Profiler.h"

#include "model/Model.h"
#include "engine/Engine.h"
#include "ui/UI.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static ClockTimer clockTimer;
static ShiftRegister shiftRegister;
static ButtonLedMatrix blm(shiftRegister);
static Encoder encoder;
static LCD lcd;
static ADC adc;
static DAC dac;
static DIO dio;
static GateOutput gateOutput;
static MIDI midi;
static USBMIDI usbMidi;
static USBH usbh(usbMidi);

static Profiler profiler;

static Model model;
static Engine engine(model, clockTimer, adc, dac, dio, gateOutput, midi, usbMidi);
static UI ui(model, engine, lcd, blm, encoder);

static os::PeriodicTask<1024> driverTask("driver", 5, os::time::ms(1), [] () {
    shiftRegister.process();
    blm.process();
    encoder.process();
});

static os::PeriodicTask<2048> usbhTask("usbh", 1, os::time::ms(1), [] () {
    usbh.process();
});

static os::PeriodicTask<1024> profilerTask("profiler", 0, os::time::ms(1000), [] () {
    profiler.dump();
});

static os::PeriodicTask<4096> engineTask("engine", 4, os::time::ms(1), [] () {
    engine.update();
});

static os::PeriodicTask<4096> uiTask("ui", 3, os::time::ms(1), [] () {
    ui.update();
});

#if CONFIG_ENABLE_STACK_USAGE
template<typename Task>
static void dumpStackUsage(const Task &task) {
    DBG("%s: %d/%d bytes", task.name(), task.stackUsage(), task.stackSize());
}

static os::PeriodicTask<1024> stackCheckTask("stack", 0, os::time::ms(5000), [&] () {
    DBG("----------------------------------------");
    DBG("Stack Usage:");
    dumpStackUsage(driverTask);
    dumpStackUsage(usbhTask);
    dumpStackUsage(profilerTask);
    dumpStackUsage(engineTask);
    dumpStackUsage(uiTask);
    dumpStackUsage(stackCheckTask);
    DBG("----------------------------------------");
});
#endif // CONFIG_ENABLE_STACK_USAGE

int main(void) {
    System::init();
    Console::init();
    HighResolutionTimer::init();

    profiler.init();

    shiftRegister.init();
    clockTimer.init();
    blm.init();
    encoder.init();
    lcd.init();
    adc.init();
    dac.init();
    dio.init();
    gateOutput.init();
    midi.init();
    usbMidi.init();
    usbh.init();

    engine.init();
    ui.init();

	os::startScheduler();
}
