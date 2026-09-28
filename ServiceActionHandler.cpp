#include "ServiceActionHandler.h"

ServiceActionHandler::ServiceActionHandler(RGBLed* led){
    this->led = led;
}

void ServiceActionHandler::ExecuteServiceAction(){
    if(_isServiceActionEnabled == false){
        _isServiceActionEnabled = true;
        led->Disable(true);
        _serviceActionCounter = 0;
    }
}

void ServiceActionHandler::HandleTimer(uint16_t delta){
    if(_isServiceActionEnabled){
        _serviceActionCounter += delta;

        if(_serviceActionCounter < 500){
            GPIOB->ODR &= ~GPIO_ODR_ODR4;
        }else{
            GPIOB->ODR |= GPIO_ODR_ODR4;
        }

        if(_serviceActionCounter >= 10000){
            led->Disable(false);
            _isServiceActionEnabled = false;
            _serviceActionCounter = 0;
        }
    }
}