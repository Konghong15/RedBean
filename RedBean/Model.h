#pragma once

// �� ���̶� Ŭ������ �� ����� �ϳ��� �ε� ������ ����� �� ������ ū �ǹ̰� �ֳ�? ��

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