#include "GameFunction.h"
#include"Player.h"
#include"Sprite2D.h"

void GameFunction::AddEXP(float exp)
{
	Player* temp = Player::GetInstance();

	temp->ExperienceUp(exp);
	temp->EXPupdate();
}

void GameFunction::LoadGameSceneTexture()
{
	LoadPlayerTexture();
	LoadBossTexture();
	LoadNumberTexture();
}
void GameFunction::LoadTitleSceneTexture()
{
	Sprite2D::LoadTex(titleSprite, "gameName.png");
	Sprite2D::LoadTex(titleExplainSprite, "title.png");
	Sprite2D::LoadTex(selectTutorial, "selectTutorial.png");
	Sprite2D::LoadTex(selectGameStart, "selectGamePlay.png");
	Sprite2D::LoadTex(arrowSprite, "arrow.png");
}

void GameFunction::LoadEndSceneTexture()
{
	Sprite2D::LoadTex(endSprite, "gameOver.png");
}

void GameFunction::LoadClearSceneTexture()
{
	Sprite2D::LoadTex(clearSprite, "gameClear.png");
}


DirectX::XMFLOAT3 GameFunction::GetPlayerPos()
{
	return Player::GetInstance()->GetPos();
}

bool GameFunction::GetPlayerIsSpecial()
{
	return Player::GetInstance()->GetIsSpecial();
}


void GameFunction::LoadPlayerTexture()
{
	// プレイヤー関係の画像読み込み
	// 体力
	Sprite2D::LoadTex(playerHealthBar, TEXT("playerHealth.png"));
	Sprite2D::LoadTex(playerHealth, TEXT("playerHealth1Dot.png"));
	// 経験値
	Sprite2D::LoadTex(playerExpBar, TEXT("playerEXPgage.png"));
	Sprite2D::LoadTex(playerExp, TEXT("playerEXP1dot.png"));
}

void GameFunction::LoadBossTexture()
{
	// ボス用の画像読み込み
	Sprite2D::LoadTex(bossHealth, TEXT("bossHealth1dot.png"));
	Sprite2D::LoadTex(bossHealthBar, TEXT("playerHealth.png"));
}

void GameFunction::LoadNumberTexture()
{
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


