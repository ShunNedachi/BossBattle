#pragma once
#include "Enemy.h"
#include"Sprite2D.h"
#include"Setting.h"
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

    std::vector<Enemy*> GetEnemys() { return enemys; }

   

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
    bool ActionRangeAttack();
    //bool Action

private:

    // 召喚する敵の配列
    std::vector<Enemy*> enemys;

    //int pattern = 0;

    Sprite2D* hpSprite;

    // 行動状況用
    BossPattern pattern = BossPattern::Stop;

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
};


 