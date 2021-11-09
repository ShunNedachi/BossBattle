#pragma once
#include "Enemy.h"
#include<vector>

class Boss :
    public Enemy
{
public:
    ~Boss() {}

    // åpè≥
    void Init() override;
    void Update() override;
    void Draw() override;

    void Destroy();

    void AddEnemy(XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 },
        XMFLOAT3 rotation = { 0,0,0 });

    std::vector<Enemy*> GetEnemys() { return enemys; }

private:

    // è¢ä´Ç∑ÇÈìGÇÃîzóÒ
    std::vector<Enemy*> enemys;

    int pattern = 0;
};

 