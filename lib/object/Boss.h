#pragma once
#include "Enemy.h"
#include"Sprite2D.h"
#include"Setting.h"
#include"AttackBase.h"
#include<vector>

#define STATE_SLEEP 0x000
#define STATE_MOVE 0x001

class Boss :
    public Enemy
{
private:

    enum class BossPattern
    {
        Stop,
        PopEnemy,
        Sleep,
        Follow,
        Rush,
        Bless,
        FlyCombo,
    };


public:
    ~Boss() {}

    // 継承
    void Init() override;
    void Update() override;
    void Draw() override;

    void Destroy();

    void AddEnemy(XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 },
        XMFLOAT3 rotation = { 0,0,0 });

    // 攻撃の解放処理用
    void DeleteAttack();

    std::vector<Enemy*> GetEnemys() { return enemys; }

    float GetBlessDamage() { return blessDamage; }

    // 初期の待機状態かどうかを返す
    bool GetIsInit() { return isInit; }

private:

    // 行動パターン用
    void Action();

    // 返り値で行動終了を返す
    bool ActionStop();
    bool ActionPopEnemy();
    bool ActionSleep();
    bool ActionFollow();
    bool ActionRush();

    // 後でパターンに組み込み

    // 特殊行動用  後で追加予定
    bool ActionSpecial();

    // 遠距離攻撃用
    bool ActionBless();
    bool ActionLightning();
    // 飛行攻撃(ジャンプ攻撃用)
    bool ActionFallConmbo();
    bool ActionFlyBless();


    void DestroyAttackArray();

private:

    // 召喚する敵の配列
    std::vector<Enemy*> enemys;

    //int pattern = 0;

    Sprite2D* hpSprite;
    Sprite2D* hpBarSprite;
    const float MAX_HEALTH = 10;
    const float HEALTH_SIZE = 800;
    // 最大HPにおける1HPの割合
    const float ONE_HEALTH_SIZE = HEALTH_SIZE / MAX_HEALTH;

    // 行動状況用
    BossPattern pattern = BossPattern::FlyCombo;
    bool isInit = false;
    int initFrame = 5 * GAME_FRAME;

    // 経過フレーム計測用
    int actionCount = 0;


    // 停止状態の秒数
    int stopFrame = 3 * GAME_FRAME;

    // 敵出現状態の停止秒数
    int popFrame = 3 * GAME_FRAME;

    // 休憩状態の秒数
    int sleepFrame = 3 * GAME_FRAME;
    // 回復の間隔
    int healFrame = 1 * GAME_FRAME;
    
    // 追従状態の秒数
    int followFrame = 10 * GAME_FRAME;
    // 追従前の待機秒数
    int followStopFrame = 1 * GAME_FRAME;

    // 突進状態の秒数
    int rushFrame = 10 * GAME_FRAME;
    // 突進前の待機秒数
    int rushWaitFrame = 2 * GAME_FRAME;
    // 1回あたりの突進時間
    int oneRushFrame = 1 * GAME_FRAME;
    XMVECTOR rushV;
    // 突進用経過時間計測用
    int rushCount = 0;
    // 突進しているのかどうか
    bool isRush = false;

    // 初期化が必要な攻撃用
    bool initAttack = true;


    // 遠距離攻撃用
    // bless用
    
    //  bless発射用待機フレーム
    int blessFrame = 10 * GAME_FRAME;
    int blessWaitFrame = 1 * GAME_FRAME;
    // 発射方向
    std::vector<XMFLOAT3> blessV;
    //
    float blessDamage = 1;

    // 飛行状態用
    int flyFrame = 10 * GAME_FRAME; // デバッグ用に時間を10 ⇒　1
    //int flyToUpFrame = 1 * GAME_FRAME;
    //int flyToDownFrame = 1 * GAME_FRAME;
    // 落下攻撃する頻度
    int flyAttackFrame = 1 * GAME_FRAME;
    float flyAttackDamage = 1;

    // 飛行時最大値
    const float MAX_FLY_POS = 50;
    bool startFly = true;
    bool endFly = false;
    bool initEndFly = true;
    bool isFly = false; // 飛行状態解除時に攻撃中かどうか確認用
    Easing flyEasing;
};


 