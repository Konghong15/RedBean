#pragma once

// 이 모델이란 클래스는 이 사람이 하나의 로딩 단위로 사용할 뿐 나한테 큰 의미가 있나? 흠

class Material;
class MeshGeometry;

struct Model
{
	shared_ptr<Material> Material = nullptr;
	shared_ptr<MeshGeometry> MeshGeometry = nullptr;
};

struct ModelInstance
{
	shared_ptr<Model> Model;
	Matrix World;
};