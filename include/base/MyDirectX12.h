#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
#include<d3dcompiler.h>
#include<wrl.h>
#include"MyWindow.h"


class MyDirectX12
{
private:

	// エイリアス
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;


public:

	MyDirectX12();
	~MyDirectX12();
	void Initialize(MyWindow* window); // デバイスやファクトリー等の初期化
	void Debug(); // デバッグレイヤー有効化

	void SetViewport();
	void SetScissorrect();

	void Draw(ComPtr<ID3D12DescriptorHeap>basicDescHeap, D3D12_INDEX_BUFFER_VIEW  ibView,
		D3D12_VERTEX_BUFFER_VIEW vbView, UINT Indices, D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV, D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV);

	ComPtr<ID3D12Device> Device();
	ComPtr<ID3D12GraphicsCommandList> CommandList();

	//ID3D12Device* Device();
	//ID3D12GraphicsCommandList* CommandList();

	// 描画用　上から順に使用する
	void BeginBarrier(); // リソースバリアを変更
	void ClearScreen(); // 画面クリアコマンド
	void EndBarrier();   //　リソースバリアを戻す
	void ExecuteCmd();

	void PostDraw();
	void PreDraw();

	bool CreatDepthDesc();

	// imgui初期化
	bool InitImgui();

	ID3D12Device* GetDevice() { return dev.Get(); }
	ID3D12CommandList* GetCommandList() { return cmdList.Get(); }

	void SetClearColor(DirectX::XMFLOAT4 color) { clearBackColor = color; }

private:
	MyWindow* window;

	HRESULT result;
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator>cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	// imgui
	ComPtr<ID3D12DescriptorHeap> imguiHeap;

	// リソースバリア用
	std::vector<ComPtr<ID3D12Resource>> backBuffers{ 2 };
	D3D12_RESOURCE_BARRIER barrierDesc{};
	UINT bbIndex;

	// 描画用
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorrect{};

	DirectX::XMFLOAT4 clearBackColor = { 0.0f, 0.0f, 0.5f, 0.0f };

};

// XMFLOAT3同士の加算処理
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const float& rhs);


const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);


const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const float& rhs);


const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const float& rhs);

DirectX::XMFLOAT4& operator-=(const DirectX::XMFLOAT4& lhs, const float& rhs);