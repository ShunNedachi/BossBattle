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

    // テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    // srv
    ComPtr<ID3D12DescriptorHeap> srvDescHeap;


    // 深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    // RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    // DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    // 画面クリアカラー
    static const float clearColor[4];

};

