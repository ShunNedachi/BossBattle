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

	// �G�C���A�X
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;


public:

	MyDirectX12();
	~MyDirectX12();
	void Initialize(MyWindow* window); // �f�o�C�X��t�@�N�g���[���̏�����
	void Debug(); // �f�o�b�O���C���[�L����

	ComPtr<ID3D12Device> Device();
	ComPtr<ID3D12GraphicsCommandList> CommandList();

	void PostDraw();
	void PreDraw();


	// imgui������
	bool InitImgui();

	ID3D12Device* GetDevice() { return dev.Get(); }
	ID3D12CommandList* GetCommandList() { return cmdList.Get(); }

	void SetClearColor(DirectX::XMFLOAT4 color) { clearBackColor = color; }
	
private:

	// �`��p�@�ォ�珇�Ɏg�p����
	void BeginBarrier(); // ���\�[�X�o���A��ύX
	void ClearScreen(); // ��ʃN���A�R�}���h
	void EndBarrier();   //�@���\�[�X�o���A��߂�
	void ExecuteCmd();


	void SetViewport();
	void SetScissorrect();

	bool CreatDepthDesc();

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

	// ���\�[�X�o���A�p
	std::vector<ComPtr<ID3D12Resource>> backBuffers{ 2 };
	D3D12_RESOURCE_BARRIER barrierDesc{};
	UINT bbIndex;

	// �`��p
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorrect{};

	DirectX::XMFLOAT4 clearBackColor = { 0.0f, 0.0f, 0.5f, 0.0f };

};

// XMFLOAT3���m�̉��Z����
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const float& rhs);


const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);


const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const float& rhs);


const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const float& rhs);

DirectX::XMFLOAT4& operator-=(const DirectX::XMFLOAT4& lhs, const float& rhs);