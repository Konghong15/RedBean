#include "pch.h"

#include "Material.h"
#include "BindableCommon.h"
#include "DynamicConstant.h"
#include "ConstantBufferEx.h"
#include "TransformCbufScaling.h"
#include "Stencil.h"
#include "Texture.h"

Material::Material(Graphics& graphics, const aiMaterial& material, const std::filesystem::path& path)
	: mModelPath(path.string())
{
	using namespace Bind;

	const auto rootPath = path.parent_path().string() + "/../textures/";

	{
		aiString tempName;
		material.Get(AI_MATKEY_NAME, tempName);
		mName = tempName.C_Str();
	}

	// 재질에 따른 쉐이더 추가
	{
		// 퐁 기법은 램버티안 스탭하나로 이뤄짐
		Technique phong{ "Phong" };
		Step step("lambertian");
		std::string shaderCode = "Phong";

		// 기본 레이아웃 추가
		mVertexLayout.Append(Dvtx::VertexLayout::Position3D);
		mVertexLayout.Append(Dvtx::VertexLayout::Normal);
		Dcb::RawLayout pscLayout;
		bool hasTexture = false;
		bool hasGlossAlpha = false;

		// diffuse
		{
			aiString texFileName;
			bool hasAlpha = false;

			// diffuse가 존재하면 쉐이더 이름을 바꾸고 텍스처를 바인딩함
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				mVertexLayout.Append(Dvtx::VertexLayout::Texture2D);

				hasTexture = true;

				shaderCode += "Dif";

				std::filesystem::path filePath = texFileName.C_Str();

				auto tex = Bind::Texture::Create(graphics, rootPath + filePath.filename().string());

				if (tex->HasAlpha())
				{
					hasAlpha = true;
					shaderCode += "Msk";
				}

				step.AddBindable(std::move(tex));
			}
			// diffuse가 존재하지 않으면 상수 버퍼에 머테리얼 색상을 추가함
			else
			{
				pscLayout.Add<Dcb::Float3>("materialColor");
			}

			step.AddBindable(Bind::Rasterizer::Create(graphics, hasAlpha));
		}

		// specular
		{
			aiString texFileName;

			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Spc";
				mVertexLayout.Append(Dvtx::VertexLayout::Texture2D);

				std::filesystem::path filePath = texFileName.C_Str();
				auto tex = Bind::Texture::Create(graphics, rootPath + filePath.filename().string(), 1);

				// auto tex = Bind::Texture::Create(graphics, rootPath + texFileName.C_Str(), 1);
				hasGlossAlpha = tex->HasAlpha();
				step.AddBindable(std::move(tex));
				pscLayout.Add<Dcb::Bool>("useGlossAlpha");
				pscLayout.Add<Dcb::Bool>("useSpecularMap");
			}
			pscLayout.Add<Dcb::Float3>("specularColor");
			pscLayout.Add<Dcb::Float>("specularWeight");
			pscLayout.Add<Dcb::Float>("specularGloss");
		}
		// normal
		{
			aiString texFileName;

			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Nrm";
				mVertexLayout.Append(Dvtx::VertexLayout::Texture2D);
				mVertexLayout.Append(Dvtx::VertexLayout::Tangent);
				mVertexLayout.Append(Dvtx::VertexLayout::Bitangent);

				std::filesystem::path filePath = texFileName.C_Str();
				step.AddBindable(Bind::Texture::Create(graphics, rootPath + filePath.filename().string(), 2));

				// step.AddBindable(Bind::Texture::Create(graphics, rootPath + texFileName.C_Str(), 2));
				pscLayout.Add<Dcb::Bool>("useNormalMap");
				pscLayout.Add<Dcb::Float>("normalMapWeight");
			}
		}
		// common (post)
		{
			step.AddBindable(std::make_shared<Bind::TransformCbuf>(graphics, 0u));
			auto pvs = Bind::VertexShader::Create(graphics, "../SweetRedBean/" + shaderCode + "_VS.hlsl");
			auto pvsbc = pvs->GetBytecode();
			step.AddBindable(std::move(pvs));
			step.AddBindable(Bind::PixelShader::Create(graphics, "../SweetRedBean/" + shaderCode + "_PS.hlsl"));
			step.AddBindable(Bind::InputLayout::Create(graphics, mVertexLayout, pvsbc));
			if (hasTexture)
			{
				step.AddBindable(Bind::Sampler::Create(graphics));
			}

			// PS material params (cbuf) // 상수 버퍼 기본 값 정의
			Dcb::Buffer buf{ std::move(pscLayout) };
			if (auto r = buf["materialColor"]; r.Exists())
			{
				aiColor3D color = { 0.45f,0.45f,0.85f };
				material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
				r = reinterpret_cast<DirectX::XMFLOAT3&>(color);
			}
			buf["useGlossAlpha"].SetIfExists(hasGlossAlpha);
			buf["useSpecularMap"].SetIfExists(true);
			if (auto r = buf["specularColor"]; r.Exists())
			{
				aiColor3D color = { 0.18f,0.18f,0.18f };
				material.Get(AI_MATKEY_COLOR_SPECULAR, color);
				r = reinterpret_cast<DirectX::XMFLOAT3&>(color);
			}
			buf["specularWeight"].SetIfExists(1.0f);
			if (auto r = buf["specularGloss"]; r.Exists())
			{
				float gloss = 8.0f;
				material.Get(AI_MATKEY_SHININESS, gloss);
				r = gloss;
			}
			buf["useNormalMap"].SetIfExists(true);
			buf["normalMapWeight"].SetIfExists(1.0f);

			step.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEx>(graphics, std::move(buf), 1u));
		}
		phong.AddStep(std::move(step));

		mTechniques.push_back(std::move(phong));
	}

	// 아웃라인 스탭
	{
		Technique outline("Outline", false);
		{
			Step mask("outlineMask");

			mask.AddBindable(InputLayout::Create(graphics, mVertexLayout, VertexShader::Create(graphics, "../SweetRedBean/Solid_VS.hlsl")->GetBytecode()));
			mask.AddBindable(std::make_shared<TransformCbuf>(graphics));
			outline.AddStep(std::move(mask));
		}
		{
			Step draw("outlineDraw");

			{
				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("materialColor");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["materialColor"] = DirectX::XMFLOAT3{ 1.0f,0.4f,0.4f };
				draw.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEx>(graphics, buf, 1u));
			}

			draw.AddBindable(InputLayout::Create(graphics, mVertexLayout, VertexShader::Create(graphics, "../SweetRedBean/Solid_VS.hlsl")->GetBytecode()));
			draw.AddBindable(std::make_shared<TransformCbuf>(graphics));

			outline.AddStep(std::move(draw));
		}
		mTechniques.push_back(std::move(outline));
	}
}

Dvtx::VertexBuffer Material::ExtractVertices(const aiMesh& mesh) const
{
	return { mVertexLayout,mesh };
}
std::vector<unsigned short> Material::ExtractIndices(const aiMesh& mesh) const
{
	std::vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);

	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	return indices;
}
std::shared_ptr<Bind::VertexBuffer> Material::MakeVertexBindable(Graphics& graphics, const aiMesh& mesh, float scale) const
{
	Dvtx::VertexBuffer vtc = ExtractVertices(mesh);

	if (scale != 1.0f)
	{
		for (auto i = 0u; i < vtc.Size(); i++)
		{
			DirectX::XMFLOAT3& pos = vtc[i].Attr<Dvtx::VertexLayout::ElementType::Position3D>();
			pos.x *= scale;
			pos.y *= scale;
			pos.z *= scale;
		}
	}

	return Bind::VertexBuffer::Create(graphics, makeMeshTag(mesh), std::move(vtc));
}
std::shared_ptr<Bind::IndexBuffer> Material::MakeIndexBindable(Graphics& graphics, const aiMesh& mesh) const
{
	return Bind::IndexBuffer::Create(graphics, makeMeshTag(mesh), ExtractIndices(mesh));
}

std::string Material::makeMeshTag(const aiMesh& mesh) const
{
	return mModelPath + "%" + mesh.mName.C_Str();
}
std::vector<Technique> Material::GetTechniques() const
{
	return mTechniques;
}