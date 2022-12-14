#include "pch.h"
#include "Effect.h"
	Effect::Effect(ID3D11Device* pDevice, const std::wstring& assetFilePath)
	{
		m_pEffect = Effect::LoadEffect(pDevice, assetFilePath);

		m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
		if (!m_pTechnique->IsValid())
		{
			std::wcout << L"Technique not valid\n";
		}

		m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
		if (!m_pMatWorldViewProjVariable->IsValid())
		{
			std::wcout << L"m_pMatWorldViewProjVariable not valid\n";
		}

		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseMapVariable->IsValid())
		{
			std::wcout << L"m_pDiffuseMapVariable not valid\n";
		}

		//Vertex layout
		static constexpr uint32_t numElements{ 3 };
		D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

		vertexDesc[0].SemanticName = "POSITION";
		vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[1].SemanticName = "COLOR";
		vertexDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[2].SemanticName = "TEXCOORD";
		vertexDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		vertexDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		//Input layout
		D3DX11_PASS_DESC passDesc{};
		m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

		HRESULT result = pDevice->CreateInputLayout(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pInputLayout
		);

		if (FAILED(result))
		{
			return;
		}
	}

	Effect::~Effect()
	{
		m_pTechnique->Release();
		m_pEffect->Release();
		m_pInputLayout->Release();
	}

	ID3DX11Effect* Effect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFilePath)
	{
		HRESULT result;
		ID3D10Blob* pErrorBlob{ nullptr };
		ID3DX11Effect* pEffect;

		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // defined(DEBUG) || defined(_DEBUG)

		result = D3DX11CompileEffectFromFile(
			assetFilePath.c_str(),
			nullptr,
			nullptr,
			shaderFlags,
			0,
			pDevice,
			&pEffect,
			&pErrorBlob);

		if (FAILED(result))
		{
			if (pErrorBlob != nullptr)
			{
				const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

				std::wstringstream ss;
				for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); ++i)
				{
					ss << pErrors[i];
				}

				OutputDebugStringW(ss.str().c_str());
				pErrorBlob->Release();
				pErrorBlob = nullptr;

				std::wcout << ss.str() << '\n';
			}
			else
			{
				std::wstringstream ss;
				ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFilePath;
				std::wcout << ss.str() << "\n";
				return nullptr;
			}
		}
		return pEffect;
	}
	ID3DX11Effect* Effect::GetEffect() const
	{
		return m_pEffect;
	}
	ID3DX11EffectTechnique* Effect::GetTechnique() const
	{
		return m_pTechnique;
	}

	ID3D11InputLayout* Effect::GetInputLayout() const
	{
		return m_pInputLayout;
	}

	ID3DX11EffectMatrixVariable* Effect::GetWorldViewProjectionMatrix() const
	{
		return m_pMatWorldViewProjVariable;
	}

	void Effect::SetDiffuseMap(ID3D11Texture2D* pTexture, ID3D11ShaderResourceView* pShaderResourceView)
	{
		if (m_pDiffuseMapVariable)
		{
			m_pDiffuseMapVariable->SetResource(pShaderResourceView);
		}
	}
