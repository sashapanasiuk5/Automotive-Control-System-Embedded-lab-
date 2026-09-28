#include "States/WarningState.h"

WarningState::WarningState(RGBLed* led, ServiceActionHandler* actionHandler): BaseState(actionHandler, led){}


uint8_t WarningState::HandleTimer(uint16_t delta){
    BaseState::HandleTimer(delta);

    _timeCounter += delta;

    if ((_timeCounter % 500) == 0)
    {
        if(ledState){
            led->Reset();
            ledState = false;
        }else{
            led->SetColor(led->YELLOW);
            ledState = true;
        }
    }

    if(_timeCounter >= 6000){
        return CLOSEDSTATE;
    }

    return -1;
}

uint8_t WarningState::HandleSensor(){ return ALARMSTATE; }