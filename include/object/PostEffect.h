#pragma once
#include "Sprite2D.h"
class PostEffect :
    public Sprite2D
{
public:

    PostEffect();

    void Initialize();

    void Draw();


    void PreDrawScene();
    void PostDrawScene();

protected:

    void DrawCommand();

protected:

    // �e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    // srv
    ComPtr<ID3D12DescriptorHeap> srvDescHeap;


    // �[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    // RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    // DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    // ��ʃN���A�J���[
    static const float clearColor[4];

};

