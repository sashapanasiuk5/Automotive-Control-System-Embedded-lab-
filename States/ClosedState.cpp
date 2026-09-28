#include "States/ClosedState.h"

ClosedState::ClosedState(RGBLed* led, ServiceActionHandler* actionHandler): BaseState(actionHandler, led){}

uint8_t ClosedState::HandleButton(){ return OPENSTATE; }

uint8_t ClosedState::HandleTimer(uint16_t delta){
    BaseState::HandleTimer(delta);
    led->SetColor(led->RED);

    _timeCounter += delta;

    if(!isStartActionsExecuted){
        if(_timeCounter < 100){
            GPIOB->ODR &= ~GPIO_ODR_ODR4;
        }else if(_timeCounter < 200){
            GPIOB->ODR |= GPIO_ODR_ODR4;
        }else if(_timeCounter < 300){
            GPIOB->ODR &= ~GPIO_ODR_ODR4;
        }else{
            GPIOB->ODR |= GPIO_ODR_ODR4;
            isStartActionsExecuted = true;
        }

        if(_timeCounter < 250){
            GPIOB->ODR |= GPIO_ODR_ODR0;
        }else{
            GPIOB->ODR &= ~GPIO_ODR_ODR0;
        }
    }
    return -1;
}

uint8_t ClosedState::HandleSensor(){
    return WARNINGSTATE;
}