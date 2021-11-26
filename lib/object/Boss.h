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
    };


public:
    ~Boss() {}

    // �p��
    void Init() override;
    void Update() override;
    void Draw() override;

    void Destroy();

    void AddEnemy(XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 },
        XMFLOAT3 rotation = { 0,0,0 });

    // �U���̉�������p
    void DeleteAttack();

    std::vector<Enemy*> GetEnemys() { return enemys; }

   

private:

    // �s���p�^�[���p
    void Action();

    // �Ԃ�l�ōs���I����Ԃ�
    bool ActionStop();
    bool ActionPopEnemy();
    bool ActionSleep();
    bool ActionFollow();
    bool ActionRush();

    // ��Ńp�^�[���ɑg�ݍ���

    // ����s���p  ��Œǉ��\��
    bool ActionSpecial();

    // �������U���p
    bool ActionBless();
    bool ActionLightning();
    // ��s�U��(�W�����v�U���p)
    bool ActionFallConmbo();
    bool ActionFlyBless();


private:

    // ��������G�̔z��
    std::vector<Enemy*> enemys;

    //int pattern = 0;

    Sprite2D* hpSprite;

    // �s���󋵗p
    BossPattern pattern = BossPattern::Stop;

    // �o�߃t���[���v���p
    int actionCount = 0;


    // ��~��Ԃ̕b��
    int stopFrame = 3 * GAME_FRAME;

    // �G�o����Ԃ̒�~�b��
    int popFrame = 3 * GAME_FRAME;

    // �x�e��Ԃ̕b��
    int sleepFrame = 3 * GAME_FRAME;
    // �񕜂̊Ԋu
    int healFrame = 1 * GAME_FRAME;
    
    // �Ǐ]��Ԃ̕b��
    int followFrame = 10 * GAME_FRAME;
    // �Ǐ]�O�̑ҋ@�b��
    int followStopFrame = 1 * GAME_FRAME;

    // �ːi��Ԃ̕b��
    int rushFrame = 10 * GAME_FRAME;
    // �ːi�O�̑ҋ@�b��
    int rushWaitFrame = 2 * GAME_FRAME;
    // 1�񂠂���̓ːi����
    int oneRushFrame = 1 * GAME_FRAME;
    XMVECTOR rushV;
    // �ːi�p�o�ߎ��Ԍv���p
    int rushCount = 0;
    // �ːi���Ă���̂��ǂ���
    bool isRush = false;

    // ���������K�v�ȍU���p
    bool initAttack = true;
    // �������U���p
    AttackBase* attack = nullptr;
};


 