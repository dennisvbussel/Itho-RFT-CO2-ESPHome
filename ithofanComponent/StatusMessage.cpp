#include "StatusMessage.h"

#include <map>

const std::map<uint8_t, FanStatus> fanStatusBytes = {
    {0x18, FanStatus::automatic},
    {0x19, FanStatus::night},
    {0x01, FanStatus::low},
    {0x02, FanStatus::medium},
    {0x03, FanStatus::high},
    {0x0D, FanStatus::timer}
};

StatusMessage::StatusMessage(std::vector<uint8_t> data) : 
    Message(std::move(data)) {}

FanStatus StatusMessage::getFanStatus() const {
    if (valid()) {
        auto it = fanStatusBytes.find(contentBytes.at(28));
        if (it != fanStatusBytes.end()) {
            return it->second;
        }
    }
    return FanStatus::unknown;
}

uint16_t StatusMessage::getRemainingTime() const {
    if (valid())
        return ((uint16_t)contentBytes.at(31) << 8 | contentBytes.at(32));
    else
        return 0;
}

bool StatusMessage::valid() const {
    return (contentBytes.size() == 40 || contentBytes.size() == 41) && validateChecksum();
}