#include "stm32f103x6.h"
#include "Debouncer.h"

Debouncer::Debouncer(uint8_t port, Callback_t callback, uint16_t duration, bool isPulledUp)
{
    this->_duration = duration;
    this->_port = port;
    this->_callback = callback; 
    this->_mesuerment = 0;
    this->_counter = 0;
    this->isPulledUp = isPulledUp;
    this->_isListening = false;
}

void Debouncer::Measure(){
    _counter++;

    bool pinValue = GPIOA->IDR & (GPIO_IDR_IDR0 << _port);

    if(isPulledUp){
        pinValue = !pinValue;
    }

    if(pinValue){
        _mesuerment++;
    }else{
        _mesuerment = 0;
    }

    if(_counter >= _duration){
        bool finalPinState = _mesuerment >= THRESHOLD;
        _callback(finalPinState);

        this->_isListening = false;
        this->_counter = 0;
        this->_mesuerment = 0;


        EXTI->FTSR |= EXTI_FTSR_TR0 << _port;
        EXTI->RTSR |= EXTI_RTSR_TR0 << _port;
    }
}

void Debouncer::Listen(){
    EXTI->FTSR &= ~(EXTI_FTSR_TR0 << _port);
    EXTI->RTSR &= ~(EXTI_RTSR_TR0 << _port);

    bool isTimerStarted = TIM3->CR1 & TIM_CR1_CEN;
    if(!isTimerStarted){
        TIM3->CR1 |= TIM_CR1_CEN;
    }

    this->_isListening = true;
    this->_counter = 0;
    this->_mesuerment = 0;
}

bool Debouncer::IsListening(){ return this->_isListening; }