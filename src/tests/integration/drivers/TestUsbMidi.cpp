#include "IntegrationTest.h"

#ifdef PLATFORM_STM32
#include "drivers/UsbH.h"
#endif

#include "drivers/UsbMidi.h"

#include "core/midi/MidiMessage.h"

class TestUsbMidi : public IntegrationTest {
public:
    TestUsbMidi() :
        IntegrationTest("UsbMidi", true)
#ifdef PLATFORM_STM32
        ,usbh(usbMidi)
#endif
    {}

    void init() override {
        usbMidi.init();
#ifdef PLATFORM_STM32
        usbh.init();
        usbh.powerOn();
#endif
    }

    void update() override {
#ifdef PLATFORM_STM32
        usbh.process();
#endif
        MidiMessage msg;
        while (usbMidi.recv(&msg)) {
            MidiMessage::dump(msg);
        }

        if (sendInterval++ % 200 == 0) {
            const uint8_t pattern[] = { 0, 12, 0, 12, 3, 9, 12, 3 };
            usbMidi.send(MidiMessage::makeNoteOff(0, 36 + pattern[sendPosition]));
            sendPosition = (sendPosition + 1) % sizeof(pattern);
            usbMidi.send(MidiMessage::makeNoteOn(0, 36 + pattern[sendPosition]));
        }

        os::delay(1);
    }

private:
    UsbMidi usbMidi;
#ifdef PLATFORM_STM32
    UsbH usbh;
#endif
    uint32_t sendInterval = 0;
    uint8_t sendPosition = 0;
};

INTEGRATION_TEST(TestUsbMidi)
