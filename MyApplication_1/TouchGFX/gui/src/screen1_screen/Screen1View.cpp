#include <gui/screen1_screen/Screen1View.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

Screen1View::Screen1View() {

}

void Screen1View::setupScreen() {
	localImageX = presenter->GetImageX();
	Screen1ViewBase::setupScreen();
	
	// Initialize Player Position (Lane 3 - Start roughly center-right)
	playerLane = 3; 
	player.setX(getLaneX(playerLane, player.getWidth()));
	
	bullet.setX(0);
	bullet.setVisible(false);
	textArea1.setWildcard(textArea1Buffer);
	
	spawnTimer = 0;
	
	// Initialize Object Pool
	enemyPool[0] = &enemy1;
	enemyPool[1] = &enemy2;
	enemyPool[2] = &enemy3;
	enemyPool[3] = &enemy4;
	enemyPool[4] = &enemy5;
	enemyPool[5] = &enemy6;

	for (int i = 0; i < ENEMY_POOL_SIZE; i++)
	{
	    resetEnemy(enemyPool[i]);
	}

    // Initialize Bullet Pool
    bulletSpawnTimer = 0;
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        bulletPool[i].setBitmap(bullet.getBitmap());
        add(bulletPool[i]);
        resetBullet(&bulletPool[i]);
    }

    // Game Over Overlay (Red Box)
    gameOverOverlay.setPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gameOverOverlay.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
    gameOverOverlay.setAlpha(100); // Semi-transparent (0-255)
    gameOverOverlay.setVisible(false);
    add(gameOverOverlay);

    // Pause Overlay (Reusing Red Box concept)
    pauseOverlay.setPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    pauseOverlay.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
    pauseOverlay.setAlpha(100); 
    pauseOverlay.setVisible(false);
    add(pauseOverlay);
    
    isPaused = false;

    // Initialize Game Over Text (Using same TypedText as Score for convenience)
    isGameOver = false;

    // Hide New Game button initially
    buttonNew.setVisible(false);

    // Initialize High Score Widget (Using Generated 'highscore' widget)
    highscore.setWildcard(highScoreBuffer);
    highscore.setVisible(true);

    // Initial High Score Update
    updateHighScore(presenter->getHighScore());

	currentScore=0;
	updateScore(currentScore);
}

void Screen1View::resetEnemy(touchgfx::Image* enemy)
{
    enemy->setVisible(false);
    enemy->setY(-enemy->getHeight());
    enemy->invalidate();
}

void Screen1View::resetBullet(touchgfx::Image* bullet)
{
    bullet->setVisible(false);
    bullet->invalidate();
}

void Screen1View::tearDownScreen() {
	Screen1ViewBase::tearDownScreen();
}

void Screen1View::updateScore(int score)
{
    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", score);
    textArea1.resizeToCurrentText();
    textArea1.invalidate();
    
    // Check High Score
    if (score > presenter->getHighScore())
    {
        presenter->saveHighScore(score);
        updateHighScore(score);
    }
}

void Screen1View::updateHighScore(int score)
{
    Unicode::snprintf(highScoreBuffer, HIGHSCORE_SIZE, "%d", score);
    highscore.resizeToCurrentText();
    highscore.invalidate();
}

void Screen1View::handleTickEvent()
{
	Screen1ViewBase::handleTickEvent();
	
	if (isGameOver) return; 
	if (isPaused) return; 
	
	tickCount++;
	spawnTimer++;

	// 1. Enemy Spawning Logic
	if (spawnTimer >= 60)
	{
	    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
	    {
	        if (!enemyPool[i]->isVisible())
	        {
	            // Lane Spawning Logic
	            int lane = presenter->GetRandomNumber() % LANE_COUNT;
	            enemyPool[i]->setX(getLaneX(lane, enemyPool[i]->getWidth()));
	            enemyPool[i]->setY(-enemyPool[i]->getHeight());
	            enemyPool[i]->setVisible(true);
	            enemyPool[i]->invalidate();
	            spawnTimer = 0;
	            break;
	        }
	    }
	}


	for (int i = 0; i < ENEMY_POOL_SIZE; i++)
	{
	    touchgfx::Image* enemy = enemyPool[i];
	    if (enemy->isVisible())
	    {
	        enemy->invalidate();
	        enemy->setY(enemy->getY() + 3);
	        enemy->invalidate();


            // Collision with Player (Lane-based)
            int enemyLane = (enemy->getX() + enemy->getWidth() / 2) / LANE_WIDTH;
            
            if (playerLane == enemyLane)
            {
                if (player.getRect().intersect(enemy->getRect()))
                {
                    isGameOver = true;
                    
                    gameOverOverlay.setVisible(true);
                    gameOverOverlay.invalidate();
                    
                    buttonNew.setVisible(true);
                    buttonNew.invalidate();
                    
                    return;
                }
            }

	        if (enemy->getY() > SCREEN_HEIGHT)
	        {
	            resetEnemy(enemy);
	        }
	    }
	}

    // 4. Bullet Update & Collision
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        touchgfx::Image* b = &bulletPool[i];
        if (b->isVisible())
        {
             b->invalidate();
             b->setY(b->getY() - 10);
             b->invalidate();

             if (b->getY() < -b->getHeight())
             {
                 resetBullet(b);
                 continue; 
             }

             // Collision with Enemies
             for (int j = 0; j < ENEMY_POOL_SIZE; j++)
             {
                 touchgfx::Image* e = enemyPool[j];
                 if (e->isVisible() && b->getRect().intersect(e->getRect()))
                 {
                     resetBullet(b);
                     resetEnemy(e);
                     
                     currentScore += SCORE_INCREMENT;
                     updateScore(currentScore);
                     presenter->playScoreSound();
                     break; 
                 }
             }
        }
    }
}
void Screen1View::moveLeft()
{
    if (playerLane > 0)
    {
        player.invalidate();
        playerLane--;
        player.setX(getLaneX(playerLane, player.getWidth()));
        player.invalidate();
    }
}

void Screen1View::moveRight()
{
    if (playerLane < LANE_COUNT - 1)
    {
        player.invalidate();
        playerLane++;
        player.setX(getLaneX(playerLane, player.getWidth()));
        player.invalidate();
    }
}

void Screen1View::fireBullet()
{
    // Single Bullet Logic
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        if (!bulletPool[i].isVisible())
        {
            // Fire from current Player Lane
            bulletPool[i].setX(getLaneX(playerLane, bulletPool[i].getWidth()));
            bulletPool[i].setY(BULLET_START_Y);
            bulletPool[i].setVisible(true);
            bulletPool[i].invalidate();
            break; 
        }
    }
}

int16_t Screen1View::getLaneX(int laneIndex, int objectWidth)
{
    return (laneIndex * LANE_WIDTH) + (LANE_WIDTH - objectWidth) / 2;
}

void Screen1View::resetGame()
{
    // Reset game state
    isGameOver = false;
    currentScore = 0;
    
    updateHighScore(presenter->getHighScore());
    
    spawnTimer = 0;
    tickCount = 0;
    
    gameOverOverlay.setVisible(false);
    gameOverOverlay.invalidate();
    
    buttonNew.setVisible(false);
    buttonNew.invalidate();
    
    playerLane = 3;
    player.setX(getLaneX(playerLane, player.getWidth()));
    player.invalidate();
    
    // Reset all enemies
    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        resetEnemy(enemyPool[i]);
    }
    
    // Reset all bullets
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        resetBullet(&bulletPool[i]);
    }
    
    // Update score display
    updateScore(currentScore);
}

void Screen1View::newGame()
{
    resetGame();
}

void Screen1View::function1()
{
    togglePause();
}

void Screen1View::togglePause()
{
    if (isGameOver) return;

    isPaused = !isPaused;
    
    pauseOverlay.setVisible(isPaused);
    pauseOverlay.invalidate();
}
