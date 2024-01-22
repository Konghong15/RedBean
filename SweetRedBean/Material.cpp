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

	// 머테리얼 이름
	{
		aiString tempName;
		material.Get(AI_MATKEY_NAME, tempName);
		mName = tempName.C_Str();
	}

	// 랜더 테크닉 : pbr인지 non pbr인지 분기
	{
		Technique renderTech{ " RenderTechnique " };

		// 베이스 패스
		const auto rootPath = path.parent_path().string() + "/../textures/";

		auto createTexture = [&](aiTextureType aiTextureType, unsigned int bindIndex, Step& step) -> bool
			{
				aiString texturePath;

				if (material.GetTexture(aiTextureType, 0, &texturePath) == AI_SUCCESS)
				{
					std::filesystem::path filePath = texturePath.C_Str();

					auto texture = Bind::Texture::Create(graphics, rootPath + filePath.filename().string(), bindIndex);
					step.AddBindable(texture);

					return true;
				}

				return false;
			};

		mVertexLayout.Append(Dvtx::VertexLayout::Position3D)
			.Append(Dvtx::VertexLayout::Texture2D)
			.Append(Dvtx::VertexLayout::Normal)
			.Append(Dvtx::VertexLayout::Tangent);

		// pbr 텍스처
		if (material.GetTextureCount(aiTextureType_METALNESS) > 0u || material.GetTextureCount(aiTextureType_SHININESS) > 0u)
		{
			Step step{ "PBR" };

			// 상수 버퍼 생성
			{
				Dcb::RawLayout PSLayout;
				PSLayout.Add<Dcb::Bool>("useDiffuse")
					.Add<Dcb::Bool>("useNormal")
					.Add<Dcb::Bool>("useEmissive")
					.Add<Dcb::Bool>("useAlpha")
					.Add<Dcb::Bool>("useMatalness")
					.Add<Dcb::Bool>("useRoughness");

				Dcb::Buffer PSBuffer{ std::move(PSLayout) };

				if (!(PSBuffer["useDiffuse"] = createTexture(aiTextureType_DIFFUSE, 0, step)))
				{
					PSBuffer["useDiffuse"] = createTexture(aiTextureType_BASE_COLOR, 0, step);
				}

				PSBuffer["useNormal"] = createTexture(aiTextureType_NORMALS, 1, step);
				PSBuffer["useEmissive"] = createTexture(aiTextureType_EMISSIVE, 2, step);

				if (PSBuffer["useAlpha"] = createTexture(aiTextureType_OPACITY, 3, step))
				{
					step.AddBindable(Bind::Rasterizer::Create(graphics, true));
				}

				PSBuffer["useMatalness"] = createTexture(aiTextureType_METALNESS, 4, step);
				PSBuffer["useRoughness"] = createTexture(aiTextureType_SHININESS, 5, step);

				step.AddBindable(std::make_unique<Bind::CachingPixelConstantBufferEx>(graphics, std::move(PSBuffer), 1u));
				step.AddBindable(std::make_unique<Bind::CachingPixelConstantBufferEx>(graphics, std::move(PSBuffer), 1u));
				step.AddBindable(std::make_shared<TransformCbuf>(graphics, 0u));
			}
			// 쉐이더 생성
			{
				std::string pixelShaderName = "../SweetRedBean/ModelPBR_PS.hlsl";

				auto vertexShader = VertexShader::Create(graphics, "../SweetRedBean/Model_VS.hlsl");
				auto inputLayout = InputLayout::Create(graphics, mVertexLayout, vertexShader->GetBytecode());
				auto pixelShader = PixelShader::Create(graphics, pixelShaderName);

				step.AddBindable(inputLayout);
				step.AddBindable(vertexShader);
				step.AddBindable(pixelShader);
			}

			renderTech.AddStep(std::move(step));
		}
		// non-pbr 
		else
		{
			Step step{ "NonPBR" };

			// 상수 버퍼 생성
			{
				Dcb::RawLayout PSLayout;
				PSLayout.Add<Dcb::Bool>("useDiffuse")
					.Add<Dcb::Bool>("useNormal")
					.Add<Dcb::Bool>("useSpecular")
					.Add<Dcb::Bool>("useAlpha");

				Dcb::Buffer PSBuffer{ std::move(PSLayout) };

				if (!(PSBuffer["useDiffuse"] = createTexture(aiTextureType_DIFFUSE, 0, step)))
				{
					PSBuffer["useDiffuse"] = createTexture(aiTextureType_BASE_COLOR, 0, step);
				}

				PSBuffer["useNormal"] = createTexture(aiTextureType_NORMALS, 1, step);
				PSBuffer["useSpecular"] = createTexture(aiTextureType_SPECULAR, 2, step);

				if (PSBuffer["useAlpha"] = createTexture(aiTextureType_OPACITY, 3, step))
				{
					step.AddBindable(Bind::Rasterizer::Create(graphics, true));
				}

				step.AddBindable(std::make_shared<TransformCbuf>(graphics, 0u));
				step.AddBindable(std::make_unique<Bind::CachingPixelConstantBufferEx>(graphics, std::move(PSBuffer), 1u));
			}

			// 쉐이더 생성
			{
				std::string pixelShaderName = "../SweetRedBean/Model_PS.hlsl";

				auto vertexShader = VertexShader::Create(graphics, "../SweetRedBean/Model_VS.hlsl");
				auto inputLayout = InputLayout::Create(graphics, mVertexLayout, vertexShader->GetBytecode());
				auto pixelShader = PixelShader::Create(graphics, pixelShaderName);

				step.AddBindable(inputLayout);
				step.AddBindable(vertexShader);
				step.AddBindable(pixelShader);
			}

			// 샘플러 스테이트
			{
				step.AddBindable(Sampler::Create(graphics));
			}

			renderTech.AddStep(std::move(step));
		}

		mTechniques.push_back(std::move(renderTech));
	}

	// 아웃라인 테크닉
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