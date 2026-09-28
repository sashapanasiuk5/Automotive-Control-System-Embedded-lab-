#include "stm32f103x6.h"
#include "States/BaseState.h"

BaseState::BaseState(ServiceActionHandler* actionHandler, RGBLed* led)
{
    this->led = led;
    this->_actionHandler = actionHandler;
}

uint8_t BaseState::HandleTimer(uint16_t delta){
    if (_serviceButtonPressCount > 0)
    {
        _serviceButtonTimeCounter += delta;

        if (_serviceButtonTimeCounter >= 2000)
        {
            _serviceButtonPressCount = 0;
            _serviceButtonTimeCounter = 0;
        }
    }


    _actionHandler->HandleTimer(delta);
    return -1;
}

void BaseState::HandleServiceButton(){
    if (_serviceButtonPressCount == 0)
    {
        _serviceButtonTimeCounter = 0; 
    }

    _serviceButtonPressCount++;

    if (_serviceButtonPressCount >= 3 && _serviceButtonTimeCounter < 2000)
    {
        _actionHandler->ExecuteServiceAction();
        _serviceButtonPressCount = 0;
        _serviceButtonTimeCounter = 0;
    }
}

void BaseState::BeginState(){
    _timeCounter = 0;
    isStartActionsExecuted = false;
}

uint8_t BaseState::HandleButton() { return -1; }

uint8_t BaseState::HandleSensor() { return -1; }