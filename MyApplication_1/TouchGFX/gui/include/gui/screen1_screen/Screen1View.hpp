#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void moveLeft();
    virtual void moveRight();
    virtual void fireBullet();
    virtual void resetGame();
    virtual void newGame();
    virtual void function1();

    static const int LANE_COUNT = 6;

    static const int LANE_WIDTH = 40; // 240 / 6
    int playerLane;
    int16_t getLaneX(int laneIndex, int objectWidth);
protected:
    int16_t localImageX;
    uint32_t tickCount;
    int currentScore;
    void updateScore (int currentScore);
    // High Score Widget
    // touchgfx::TextAreaWithOneWildcard highScoreText; // Removed to use generated 'highscore'
    static const uint16_t HIGHSCORE_SIZE = 20;
    Unicode::UnicodeChar highScoreBuffer[HIGHSCORE_SIZE];
    void updateHighScore(int score);

    // static const uint16_t TEXTAREA1_SIZE = 10;
    // Unicode::UnicodeChar textArea1Buffer[TEXTAREA1_SIZE];
    
    
    // Game Over Widgets
    touchgfx::Box gameOverOverlay; // Red overlay
    touchgfx::TextAreaWithOneWildcard gameOverText;
    static const uint16_t GAMEOVER_SIZE = 20;
    Unicode::UnicodeChar gameOverBuffer[GAMEOVER_SIZE];
    bool isGameOver;
    
    // Pause State
    bool isPaused;
    touchgfx::Box pauseOverlay; // Additional Red overlay for Pause
    void togglePause();
    
    static const int SCREEN_WIDTH = 240;
    static const int SCREEN_HEIGHT = 320;
    static const int SCORE_INCREMENT = 10;
    static const int BULLET_START_Y = 260;

    static const int ENEMY_POOL_SIZE = 6;
    touchgfx::Image* enemyPool[ENEMY_POOL_SIZE];
    int spawnTimer;
    void resetEnemy(touchgfx::Image* enemy);

    static const int BULLET_POOL_SIZE = 1;
    touchgfx::Image bulletPool[BULLET_POOL_SIZE];
    int bulletSpawnTimer;
    void resetBullet(touchgfx::Image* bullet);


};

#endif // SCREEN1VIEW_HPP
