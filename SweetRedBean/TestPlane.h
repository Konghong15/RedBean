//#pragma once
//
//#include "IDrawable.h"
//
//class TestPlane : public IDrawable
//{
//public:
//	TestPlane(Graphics& graphics, float size, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,0.0f });
//	~TestPlane() = default;
//
//	void SpawnControlWindow(Graphics& graphics, const std::string& name);
//
//	void SetPos(DirectX::XMFLOAT3 pos);
//	void SetRotation(float roll, float pitch, float yaw);
//
//	virtual Matrix GetTransform() const override;
//
//private:
//	struct PSMaterialConstant
//	{
//		DirectX::XMFLOAT4 color;
//	} pmc;
//	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
//	float roll = 0.0f;
//	float pitch = 0.0f;
//	float yaw = 0.0f;
//};