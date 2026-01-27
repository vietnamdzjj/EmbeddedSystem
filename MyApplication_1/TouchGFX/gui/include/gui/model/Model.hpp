#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
class ModelListener;

class Model
{
public:
    Model();
long modelTickCount =0;
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    void update(int16_t x);

    int16_t ImageX;
    int16_t GetImageX();
    uint32_t GetRandomNumber();

    int getHighScore();
    void saveHighScore(int score);
    
    void playScoreSound();
    void beep(int cycles, int delay);

protected:
    int highScore;
    
    int scoreSoundTimer;
    int backgroundTimer;
    int beepRemainingTicks;
    int beepFrequencyToggle;
protected:
    ModelListener* modelListener;
    
    bool prevLeftState;
    bool prevRightState;
    bool prevFireState;
};

#endif // MODEL_HPP
