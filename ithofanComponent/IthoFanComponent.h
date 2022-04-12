#include "esphome.h"

#include "FanStatus.h"
#include "IthoController.h"

class IthoFan;
class IthoSwitch;

IthoFan* fanPtr;

class IthoFan : public Component {
    public:
        IthoFan(int8_t radioIo0Pin, uint8_t firstByte, uint32_t fanAddress, uint32_t remoteAddress) :
            fanControl(radioIo0Pin, firstByte, fanAddress, remoteAddress) {}
        float get_setup_priority() const override {
            return esphome::setup_priority::DATA;
        }
        void setup() override {
            fanPtr = this;
            if (!fanControl.init())
                ESP_LOGE("IthoFanComponent", "Failed to initialize CC1101.");
            fanControl.addChangedCallback([=]() {
                FanStatus newStatus = fanControl.getFanStatus();
                id(switch_auto).publish_state(newStatus == FanStatus::automatic);
                id(switch_night).publish_state(newStatus == FanStatus::night);
                id(switch_low).publish_state(newStatus == FanStatus::low);
                id(switch_medium).publish_state(newStatus == FanStatus::medium);
                id(switch_high).publish_state(newStatus == FanStatus::high);
                id(timer_number).publish_state(fanControl.getTimer());
                id(co2_sensor).publish_state(fanControl.getCo2());
                id(exhausttemp_sensor).publish_state(fanControl.getExhaustTemp());
                id(supplytemp_sensor).publish_state(fanControl.getSupplyTemp());
                id(indoortemp_sensor).publish_state(fanControl.getIndoorTemp());
                id(outdoortemp_sensor).publish_state(fanControl.getOutdoorTemp());
                id(inletflow_sensor).publish_state(fanControl.getInletFlow());
                id(exhaustflow_sensor).publish_state(fanControl.getExhaustFlow());
            });
        }
        void loop() override {
            fanControl.listen();
        }
        void setStatus(FanStatus status) {
            bool result = fanControl.setFanStatus(status);
        }
        void setTimer(float timer) {
            fanControl.setTimer(static_cast<uint8_t>(timer));
        }
    private:
        IthoController fanControl;
};