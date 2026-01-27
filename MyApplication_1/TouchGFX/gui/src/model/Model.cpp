#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <cstdint>
#ifndef SIMULATOR
#include "stm32f4xx_hal.h"
extern RNG_HandleTypeDef hrng;
#else
#include <cstdlib>
#endif
Model::Model() : ImageX(95), modelListener(0)

{
	modelTickCount = 0;
	
	prevLeftState = true;
	prevRightState = true;
	prevFireState = false;
	
	beepRemainingTicks = 0;
	beepFrequencyToggle = 0;
	
	highScore = 0;
	
	// Audio Init
	scoreSoundTimer = 0;
	backgroundTimer = 0;
	
#ifndef SIMULATOR
    
    
    __HAL_RCC_GPIOG_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Initialize PG12 (Buzzer) and PG13 (Green LED for Debug)
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
}

void Model::tick()
{
	modelTickCount++;

#ifndef SIMULATOR
    // Poll Hardware Buttons
    
    // 1. Left Button (PE2)
    GPIO_PinState leftState = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
    if (prevLeftState == GPIO_PIN_SET && leftState == GPIO_PIN_RESET)
    {
        if (modelListener) modelListener->onLeftPressed();
    }
    prevLeftState = (leftState == GPIO_PIN_SET);

    // 2. Right Button (PE3)
    GPIO_PinState rightState = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3);
    if (prevRightState == GPIO_PIN_SET && rightState == GPIO_PIN_RESET)
    {
        if (modelListener) modelListener->onRightPressed();
    }
    prevRightState = (rightState == GPIO_PIN_SET);

    // 3. Fire Button (PC3)
    GPIO_PinState fireState = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
    if (prevFireState == GPIO_PIN_SET && fireState == GPIO_PIN_RESET)
    {
         if (modelListener) modelListener->onFirePressed();
    }
    prevFireState = (fireState == GPIO_PIN_SET);
#ifndef SIMULATOR
    if (beepRemainingTicks > 0)
    {
        
        for (int i = 0; i < 5; i++)
        {
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
            for(volatile int j=0; j<1000; j++); // Micro delay
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
            for(volatile int j=0; j<1000; j++);
        }
        beepRemainingTicks--;
    }

    // Audio Logic
    backgroundTimer++;
    if (backgroundTimer >= 60)
    {
        if (beepRemainingTicks <= 0) beepRemainingTicks = 2;
        backgroundTimer = 0;
    }
#endif
#endif
}

void Model::update(int16_t x)
{
	ImageX = x;
}

int16_t Model::GetImageX()
{
	return ImageX;
}

uint32_t Model::GetRandomNumber()
{
#ifndef SIMULATOR
    uint32_t randomnumber = 0;
    if (HAL_RNG_GenerateRandomNumber(&hrng, &randomnumber) == HAL_OK)
    {
        return randomnumber;
    }
    return 0;
#else
    return (uint32_t)std::rand();
#endif
}

int Model::getHighScore()
{
    return highScore;
}

void Model::saveHighScore(int score)
{
    if (score > highScore)
    {
        highScore = score;
    }
}

void Model::playScoreSound()
{
#ifndef SIMULATOR
    beepRemainingTicks = 6; 
#endif
}

void Model::beep(int cycles, int delay)
{
#ifndef SIMULATOR
    for (int i = 0; i < cycles; i++)
    {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_Delay(delay);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_Delay(delay);
    }
#endif
}

