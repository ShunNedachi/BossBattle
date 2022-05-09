#include "Application.h"

// �ÓI�ϐ�
MyWindow Application::myw;
MyDirectX12 Application::my12;
Input* Application::input;
Xinput* Application::xinput;
SceneManager* Application::sceneManager;
FrameFixed* Application::frameFixed;

void Application::Initialize()
{
	// �E�B���h�E������
	myw.Initialize("Engine");

	// DirectX12������
	my12.Debug();
	my12.Initialize(&myw);

	// input xinput
	input = Input::GetInstance();
	input->Initialize(myw.GetConfig().hInstance, myw.GetHWND());
	xinput = Xinput::GetInstance();
	Xinput::Initialize();

	// �I�u�W�F�N�g�}�l�[�W���[������
	ObjectManager::Initialize(&my12);

	// �V�[���}�l�[�W���[
	sceneManager = SceneManager::GetInstance();
	// fps�Œ�
	frameFixed = FrameFixed::GetInstance();

	// particle�֌W
	Particle2D::Init(&my12);
	ParticleManager::Init(&my12);
}

void Application::Destroy()
{
	// �������
	input->Destroy();
	Camera::Destroy();
	sceneManager->Destroy();
	xinput->Destroy();
	frameFixed->Destroy();
}

void Application::Update()
{
	// �t���[�����[�g�Œ�
	frameFixed->PreWait();

	// �X�V
	input->Update();
	xinput->Update();

	ObjectManager::GetInstance()->Update();

	// �e�V�[����update
	sceneManager->Update();

	Draw();

	// �t���[�����[�g�Œ�p
	frameFixed->PostWait();
}

void Application::Draw()
{
	// �����_�[�e�N�X�`���ւ̕`��
	//postEffect->PreDrawScene();
	//postEffect->PostDrawScene();

	// �`�揀���p
	my12.PreDraw();

	//postEffect->Draw();
	sceneManager->Draw();
	// �J�����̃f�o�b�O�p
	//Camera::DebugDraw();

	// �`��R�}���h�����p
	my12.PostDraw();
}
