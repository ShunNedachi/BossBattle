#include "ObjectManager.h"

// 静的メンバ変数
ObjectManager* ObjectManager::instance = nullptr;
std::vector<Object*> ObjectManager::objArray;
std::vector<Sprite2D*> ObjectManager::spriteArray;

ObjectManager* ObjectManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ObjectManager();
	}

	return instance;
}

void ObjectManager::Destroy()
{
	// インスタンスを削除
	
	if(instance != nullptr)delete instance;
	instance = nullptr;

	// 配列も削除
	// .obj用配列
	if (objArray.size() > 0)
	{
		objArray.clear();
		objArray.shrink_to_fit();
	}
	// sprite用配列
	if (spriteArray.size() > 0)
	{
		spriteArray.clear();
		spriteArray.shrink_to_fit();
	}
}

void ObjectManager::Initialize(MyDirectX12* my12, MyWindow* window, Camera* camera)
{
	Object::Init(my12, window, camera);
	Sprite2D::Init(my12, window);
}

void ObjectManager::Update()
{
	// updateの最初で配列内にnullがないか確認をする
	// 入っていた場合配列から削除し、配列を詰める

}

void ObjectManager::Draw()
{
	for (auto x : objArray)
	{
		x->Draw();
	}
	for (auto x : spriteArray)
	{
		x->Draw();
	}
}

void ObjectManager::AddOBJ(const std::string& filename,XMFLOAT3 position,XMFLOAT3 scale,XMFLOAT3 rotation,int drawShader)
{
	Object* obj = new Object(drawShader);

	obj->CreateModel(filename);
	obj->Update(position, scale, rotation);

	objArray.push_back(obj);

}

// ↓sprite

void ObjectManager::LoadTexture(const wchar_t* filename, int textureNum)
{
	Sprite2D::LoadTex(textureNum, filename);
}

void ObjectManager::AddSprite(int textureNum, XMFLOAT2 position, float rotation,XMFLOAT2 anchorPoint, XMFLOAT4 color)
{
	Sprite2D* temp = new Sprite2D(anchorPoint.x, anchorPoint.y);

	// スプライト生成
	temp->CreateSprite(textureNum);

	// 位置、角度、色の設定
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetColor(color);

	// 配列に追加
	spriteArray.push_back(temp);
}
