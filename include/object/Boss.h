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

    // �p��
    void Init() override;
    void Update() override;
    void Draw() override;

    void Destroy();

    void AddEnemy(XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 },
        XMFLOAT3 rotation = { 0,0,0 });


    std::vector<Enemy*> GetEnemys() { return enemys; }

    float GetBlessDamage() { return blessDamage; }

    // �����̑ҋ@��Ԃ��ǂ�����Ԃ�
    bool GetIsInit() { return isInit; }

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


    void DestroyAttackArray();

private:

    // ��������G�̔z��
    std::vector<Enemy*> enemys;

    //int pattern = 0;

    Sprite2D* hpSprite;
    Sprite2D* hpBarSprite;
    const float MAX_HEALTH = 10;
    const float HEALTH_SIZE = 800;
    // �ő�HP�ɂ�����1HP�̊���
    const float ONE_HEALTH_SIZE = HEALTH_SIZE / MAX_HEALTH;

    // �s���󋵗p
    BossPattern pattern = BossPattern::PopEnemy;
    bool isInit = false;
    int initFrame = 5 * GAME_FRAME;

    // �o�߃t���[���v���p
    int actionCount = 0;


    // ��~��Ԃ̕b��
    int stopFrame = 3 * GAME_FRAME;

    // �G�o����Ԃ̒�~�b��
    int popFrame = 3 * GAME_FRAME;
    // �G�o���p�̃��[�V�����p(��Ŏg�p)
    int popAnimationFrame;

    // �x�e��Ԃ̕b��
    int sleepFrame = 3 * GAME_FRAME;
    // �񕜂̊Ԋu
    int healFrame = 1 * GAME_FRAME;
    // �񕜑O�̃��[�V�����p(��Ŏg�p)
    int healActionFrame;
    
    // �Ǐ]��Ԃ̕b��
    int followFrame = 10 * GAME_FRAME;
    // �Ǐ]�O�̑ҋ@�b��
    int followStopFrame = 1 * GAME_FRAME;
    // �Ǐ]�p�̃��[�V�����p(��Ŏg�p)
    int followAnimetionFrame;

    // �ːi��Ԃ̕b��
    int rushFrame = 10 * GAME_FRAME;
    // �ːi�O�̑ҋ@�b��
    int rushWaitFrame = 2 * GAME_FRAME;
    // �ːi�p�̃��[�V�����p(��Ŏg�p)
    int rushAnimetionFrame;
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
    // bless�p
    
    // bless���˗p�ҋ@�t���[��
    int blessFrame = 10 * GAME_FRAME;
    // ���ˑҋ@�p(���[�V��������Œǉ�)
    int blessWaitFrame = 1 * GAME_FRAME;


    // ���˕���
    std::vector<XMFLOAT3> blessV;
    //
    float blessDamage = 1;

    // ��s��ԗp
    int flyFrame = 10 * GAME_FRAME;
    // ��s�p�̃��[�V�����p(�s���A��p)
    int flyAnimeFrame;
    // �����U������p�x
    int flyAttackFrame = 1 * GAME_FRAME;
    float flyAttackDamage = 1;

    // ��s���ő�l
    const float MAX_FLY_POS = 50;
    // ��s��Ԃ̊Ǘ��p
    bool startFly = true;
    bool endFly = false;
    bool initEndFly = true;

    bool flyAttackFlg = false; // �U�������ǂ����m�F�p
    bool flyBack = false;
    // bool isFly = false; // �U�������ǂ����m�F�p
    Easing flyEasing;
    XMVECTOR flyComboV;
};


 