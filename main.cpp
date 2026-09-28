#include "stm32f103x6.h"
#include "Debouncer.h"
#include "RGBLed.h"
#include "States/OpenState.h"
#include "States/ClosedState.h"
#include "States/WarningState.h"
#include "States/AlarmState.h"
#include "inits.h"

BaseState* state;

OpenState* openState;
ClosedState* closedState;
WarningState* warningState;
AlarmState* alarmState;

ServiceActionHandler* serviceActionHandler;
RGBLed* led;

Debouncer* Button1Debouncer;
Debouncer* Button2Debouncer;
Debouncer* SensorDebouncer;

void makeTransition(uint8_t nextState){
    switch (nextState)
    {
        case OPENSTATE:
            openState->BeginState();
            state = openState;
            break;
        case CLOSEDSTATE:
            closedState->BeginState();
            state = closedState;
            break;
        case WARNINGSTATE:
            warningState->BeginState();
            state = warningState;
            break;
        case ALARMSTATE:
            alarmState->BeginState();
            state = alarmState;
            break;
        default:
            break;
    }
}

extern "C" void TIM3_IRQHandler(void){
    if(Button1Debouncer->IsListening()){
        Button1Debouncer->Measure();
    }

    if(Button2Debouncer->IsListening()){
        Button2Debouncer->Measure();
    }

   if(SensorDebouncer->IsListening()){
        SensorDebouncer->Measure();
    }

    if(!Button1Debouncer->IsListening() && !Button2Debouncer->IsListening() && !SensorDebouncer->IsListening()){
        TIM3->CR1 &= ~TIM_CR1_CEN;
    }
    TIM3->SR &= ~TIM_SR_UIF;
}

extern "C" void EXTI0_IRQHandler(void){
    Button1Debouncer->Listen();
    EXTI->PR |= EXTI_PR_PR0;
}

extern "C" void EXTI1_IRQHandler(void){
    Button2Debouncer->Listen();
    EXTI->PR |= EXTI_PR_PR1;
}

extern "C" void EXTI9_5_IRQHandler(void){
    SensorDebouncer->Listen();
    EXTI->PR |= EXTI_PR_PR7;
}

extern "C" void SysTick_Handler()
{
    uint8_t nextState = state->HandleTimer(50);
    makeTransition(nextState);
}

int main(){

    Init_Ports();

    Init_Timer3();


    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(SysTick_IRQn);

    led = new RGBLed(5, 6, 7);

    GPIOB->ODR |= GPIO_ODR_ODR4; // buzzer reset
    led->Reset();

    serviceActionHandler = new ServiceActionHandler(led); 

    openState = new OpenState(led, serviceActionHandler);
    closedState = new ClosedState(led, serviceActionHandler);
    warningState = new WarningState(led, serviceActionHandler);
    alarmState = new AlarmState(led, serviceActionHandler);

    state = openState;

    state->HandleTimer(0);
    Init_SysTick();

    Button1Debouncer = new Debouncer(0, [](bool isPressed){
        
        if(isPressed){
            uint8_t nextState = state->HandleButton();
            makeTransition(nextState);
        }
    }, 30, true);

    Button2Debouncer = new Debouncer(1, [](bool isPressed){
        
        if(isPressed){
            state->HandleServiceButton();
        }
    }, 30, true);

    SensorDebouncer = new Debouncer(7, [](bool isPressed){
        if(isPressed){

            uint8_t nextState = state->HandleSensor();
            makeTransition(nextState);
        }
    }, 30, false);

    while(1){
        __WFI();
    }
    return 0;
}