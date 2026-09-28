#include "States/OpenState.h"

OpenState::OpenState(RGBLed* led, ServiceActionHandler* actionHandler): BaseState(actionHandler, led){}

uint8_t OpenState::HandleButton(){ return CLOSEDSTATE; }

uint8_t OpenState::HandleTimer(uint16_t delta){
    BaseState::HandleTimer(delta);
    _timeCounter += delta;
    ledTimeCounter += delta;

    if(!isStartActionsExecuted){
        if(_timeCounter < 100){
            GPIOB->ODR &= ~GPIO_ODR_ODR4;
        }else{
            GPIOB->ODR |= GPIO_ODR_ODR4;
        }
        

        if(_timeCounter < 250){
            GPIOB->ODR |= GPIO_ODR_ODR0;
        }else{
            GPIOB->ODR &= ~GPIO_ODR_ODR0;
            isStartActionsExecuted = true;
        }
    }

    if (ledTimeCounter < 100) {
        led->SetColor(led->GREEN);      
    }
    else if (ledTimeCounter < 200) {
        led->Reset();                   
    }
    else if (ledTimeCounter < 300) {
        led->SetColor(led->GREEN);      
    }
    else if (ledTimeCounter < 3000) {
        led->Reset();                   
    }
    else {
        ledTimeCounter = 0;
    }

    return -1;
}