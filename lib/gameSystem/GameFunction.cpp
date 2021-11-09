#include "GameFunction.h"
#include"Player.h"
#include"Sprite2D.h"
#include"SpriteLoadDefine.h"

void GameFunction::AddEXP(float exp)
{
	Player* temp = Player::GetInstance();

	temp->ExperienceUp(exp);
	temp->EXPupdate();
}

void GameFunction::LoadGameSceneTexture()
{
	// プレイヤー関係のスプライトのロード
	Sprite2D::LoadTex(playerHealthBar, TEXT("playerHealth.png"));
	Sprite2D::LoadTex(playerHealth, TEXT("playerHealth1Dot.png"));

	Sprite2D::LoadTex(playerExpBar, TEXT("playerEXPgage.png"));
	Sprite2D::LoadTex(playerExp, TEXT("playerEXP1dot.png"));

	// 数字用読み込み
	Sprite2D::LoadTex(number0, TEXT("number/0.png"));
	Sprite2D::LoadTex(number1, TEXT("number/1.png"));
	Sprite2D::LoadTex(number2, TEXT("number/2.png"));
	Sprite2D::LoadTex(number3, TEXT("number/3.png"));
	Sprite2D::LoadTex(number4, TEXT("number/4.png"));
	Sprite2D::LoadTex(number5, TEXT("number/5.png"));
	Sprite2D::LoadTex(number6, TEXT("number/6.png"));
	Sprite2D::LoadTex(number7, TEXT("number/7.png"));
	Sprite2D::LoadTex(number8, TEXT("number/8.png"));
	Sprite2D::LoadTex(number9, TEXT("number/9.png"));
}	

