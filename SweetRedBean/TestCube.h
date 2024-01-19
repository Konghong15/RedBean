//#pragma once
//
//#include "IDrawable.h"
//#include "IBindable.h"
//#include "IndexBuffer.h"
//
//class TestCube : public IDrawable
//{
//public:
//	TestCube(Graphics& graphics, float size);
//	~TestCube() = default;
//
//	void SpawnControlWindow(Graphics& graphics, const char* name);
//
//	void SetPos(DirectX::XMFLOAT3 pos);
//	void SetRotation(float roll, float pitch, float yaw);
//	Matrix GetTransform() const override;
//
//private:
//	std::vector<std::shared_ptr<bind::IBindable>> outlineEffect;
//	struct PSMaterialConstant
//	{
//		float specularIntensity = 0.1f;
//		float specularPower = 20.0f;
//		BOOL normalMappingEnabled = TRUE;
//		float padding[1];
//	} pmc;
//	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
//	float roll = 0.0f;
//	float pitch = 0.0f;
//	float yaw = 0.0f;
//};