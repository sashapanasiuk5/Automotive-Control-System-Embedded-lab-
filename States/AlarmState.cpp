#include "States/AlarmState.h"

AlarmState::AlarmState(RGBLed* led, ServiceActionHandler* actionHandler): BaseState(actionHandler, led){}


uint8_t AlarmState::HandleTimer(uint16_t delta){
    BaseState::HandleTimer(delta);

    _timeCounter += delta;

    if ((_timeCounter % 200) == 0)
    {
        if(ledState){
            led->Reset();
            ledState = false;
        }else{
            led->SetColor(led->RED);
            ledState = true;
        }
    }

    if ((_timeCounter % 150) == 0)
    {
        if(zoomerState){
            GPIOB->ODR |= GPIO_ODR_ODR4;
            zoomerState = false;
        }else{
            GPIOB->ODR &= ~GPIO_ODR_ODR4;
            zoomerState = true;
        }
    }

    return -1;
}