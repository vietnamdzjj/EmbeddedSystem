#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::UpdateImageX(int16_t x){
	model -> update(x);
}


int16_t Screen1Presenter::GetImageX(){
	return model->GetImageX();
}

void Screen1Presenter::onLeftPressed()
{
    view.moveLeft();
}

void Screen1Presenter::onRightPressed()
{
	view.moveRight();
}

void Screen1Presenter::onFirePressed()
{
    view.fireBullet();
}

uint32_t Screen1Presenter::GetRandomNumber()
{
    return model->GetRandomNumber();
}

int Screen1Presenter::getHighScore()
{
    return model->getHighScore();
}

void Screen1Presenter::saveHighScore(int score)
{
    model->saveHighScore(score);
}

void Screen1Presenter::playScoreSound()
{
    model->playScoreSound();
}
