#include"Precompiled.h"
#include"MeshComponent.h"

#include "GameWorld.h"
#include "RenderService.h"
#include <Graphics/Inc/TextureManger.h>
#include <Graphics/Inc/MeshBuilder.h>

using namespace GGEngine;
using namespace GGEngine::Graphic;

void MeshComponent::Initialize()
{
    mRenderObject.meshBuffer.Initialize(mMesh);
    auto renderService = GetOwner().GetWorld().GetService<RenderService>();
    renderService->Register(this);
}

void MeshComponent::Terminate()
{
    auto renderService = GetOwner().GetWorld().GetService<RenderService>();
    renderService->UnRegister(this);
    mRenderObject.Terminate();
}

void MeshComponent::CreateCube(float size)
{
    mMesh = MeshBuilder::CreateCube(size);
}

void MeshComponent::CreateSphere(int slices, int rings, float rads)
{
    mMesh = MeshBuilder::CreateSphere(slices, rings, rads);
}

void MeshComponent::CreatePlane(int cols, int rows, float size)
{
    mMesh = MeshBuilder::CreatePlane(cols, rows, size);
}

void MeshComponent::SetDiffuseMap(const char* fileName)
{
    mRenderObject.diffuesMapID = TextureManger::Get()->LoadTexture(fileName);
}

void MeshComponent::SetDisplacementMap(const char* fileName)
{
    mRenderObject.displacementMapID = TextureManger::Get()->LoadTexture(fileName);
}

void MeshComponent::SetNormalMap(const char* fileName)
{
    mRenderObject.normalMapID = TextureManger::Get()->LoadTexture(fileName);
}

void MeshComponent::SetSpecularMap(const char* fileName)
{
    mRenderObject.specularMapID = TextureManger::Get()->LoadTexture(fileName);
}

void MeshComponent::SetMaterial(const Material& material)
{
    mRenderObject.material = material;
}

void MeshComponent::ParseMeshComponent(const rj::Value& data, GameObject& obj)
{
	MeshComponent* meshComponent = obj.AddComponent<MeshComponent>();
	if (data.HasMember("Shape"))
	{
		const auto& shapData = data["Shape"].GetObj();

		if (shapData.HasMember("Type"))
		{
			const char* shapeType = shapData["Type"].GetString();
			if (strcmp(shapeType, "Cube") == 0)
			{
				const float size = shapData["Size"].GetFloat();
				meshComponent->CreateCube(size);
			}
			else if (strcmp(shapeType, "Sphere") == 0)
			{
				const int slices = shapData["Slices"].GetInt();
				const int rings = shapData["Rings"].GetInt();
				const float rads = shapData["Radius"].GetFloat();

				meshComponent->CreateSphere(slices, rings, rads);
			}
			else if (strcmp(shapeType, "Plane") == 0)
			{
				const int rows = shapData["Rows"].GetInt();
				const int cols = shapData["Columns"].GetInt();
				const int spacing = shapData["Spacing"].GetFloat();

				meshComponent->CreatePlane(cols, rows, spacing);
			}
			else
			{
				ASSERT(false, "ShapeType: %s is not in the library", shapeType);
			}
		}
	}
	if (data.HasMember("Material"))
	{
		Material material;
		const auto& materialData = data["Material"].GetObj();

		if (materialData.HasMember("Ambient"))
		{
			const auto& color = materialData["Ambient"].GetArray();
			material.ambient.r = color[0].GetFloat();
			material.ambient.g = color[1].GetFloat();
			material.ambient.b = color[2].GetFloat();
			material.ambient.a = color[3].GetFloat();
		}

		if (materialData.HasMember("Diffuse"))
		{
			const auto& color = materialData["Diffuse"].GetArray();
			material.diffuse.r = color[0].GetFloat();
			material.diffuse.g = color[1].GetFloat();
			material.diffuse.b = color[2].GetFloat();
			material.diffuse.a = color[3].GetFloat();
		}

		if (materialData.HasMember("Specular"))
		{
			const auto& color = materialData["Specular"].GetArray();
			material.specular.r = color[0].GetFloat();
			material.specular.g = color[1].GetFloat();
			material.specular.b = color[2].GetFloat();
			material.specular.a = color[3].GetFloat();
		}

		if (materialData.HasMember("Emissive"))
		{
			const auto& color = materialData["Emissive"].GetArray();
			material.emissive.r = color[0].GetFloat();
			material.emissive.g = color[1].GetFloat();
			material.emissive.b = color[2].GetFloat();
			material.emissive.a = color[3].GetFloat();
		}

		if (materialData.HasMember("SpecularPower"))
		{
			material.power = materialData["SpecularPower"].GetFloat();
		}

		meshComponent->SetMaterial(material);
	}
	if (data.HasMember("Texture"))
	{
		const auto& textureData = data["Texture"].GetObj();

		if (textureData.HasMember("DiffuseMap"))
		{
			const char* diffueseName = textureData["DiffuseMap"].GetString();
			meshComponent->SetDiffuseMap(diffueseName);
		}

		if (textureData.HasMember("DisplacementMap"))
		{
			const char* displacementName = textureData["DisplacementMap"].GetString();
			meshComponent->SetDisplacementMap(displacementName);
		}

		if (textureData.HasMember("NormalMap"))
		{
			const char* normalName = textureData["NormalMap"].GetString();
			meshComponent->SetNormalMap(normalName);
		}

		if (textureData.HasMember("SpecularMap"))
		{
			const char* specularName = textureData["SpecularMap"].GetString();
			meshComponent->SetSpecularMap(specularName);
		}
	}
}

const RenderObject& MeshComponent::GetRenderObject() const
{
    return mRenderObject;
}