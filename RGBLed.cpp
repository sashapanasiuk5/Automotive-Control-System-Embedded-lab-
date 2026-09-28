#include "RGBLed.h"
#include "stm32f103x6.h"

RGBLed::RGBLed(uint8_t rpin, uint8_t gpin, uint8_t bpin){
    this->_rpin = rpin;
    this->_gpin = gpin;
    this->_bpin = bpin;
}

void RGBLed::SetColor(uint8_t color) {

    if(isDisabled)
        return;

    Reset();

    uint32_t pin_mask = 0;

    switch (color) {
        case RED:
            pin_mask = (1U << _rpin);
            break;

        case GREEN:
            pin_mask = (1U << _gpin);
            break;

        case BLUE:
            pin_mask = (1U << _bpin);
            break;

        case YELLOW:
            pin_mask = (1U << _gpin) | (1U << _rpin);
            break;

        default:
            return;
    }

    GPIOB->ODR &= ~pin_mask;
}

void RGBLed::Reset() {
    
    uint32_t all_mask = (1U << _rpin) | (1U << _gpin) | (1U << _bpin);
    GPIOB->ODR |= all_mask;
}

void RGBLed::Disable(bool isDisabled){
    this->isDisabled = isDisabled;

    if(isDisabled){
        Reset();
    }
}