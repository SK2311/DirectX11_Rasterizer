#include "pch.h"
#include "Mesh.h"
#include "Effect.h"

	Mesh::Mesh(ID3D11Device* pDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		//Create effect instance
		const std::wstring path = { L"Resources/PosCol3D.fx" };
		m_pEffect = new Effect(pDevice, path);

		//Create vertex buffer
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = vertices.data();

		HRESULT result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

		if (FAILED(result))
		{
			return;
		}

		//Create index buffer
		m_NumIndices = static_cast<uint32_t>(indices.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = indices.data();
		result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);

		if (FAILED(result))
		{
			return;
		}
	}

	Mesh::~Mesh()
	{
		m_pVertexBuffer->Release();
		m_pIndexBuffer->Release();

		delete m_pEffect;
	}

	void Mesh::Render(ID3D11DeviceContext* pDeviceContext, dae::Matrix& worldViewProjection) const
	{
		//1 Set primitive topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//2 Set the input layout
		pDeviceContext->IASetInputLayout(m_pEffect->GetInputLayout());

		//3 Set VertexBuffer
		constexpr UINT stride = sizeof(Vertex);
		constexpr UINT offset = 0;

		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		m_pEffect->GetWorldViewProjectionMatrix()->SetMatrix((float*)(&worldViewProjection));

		//4 Set indexbuffer
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//5 Draw
		D3DX11_TECHNIQUE_DESC techDesc{};
		m_pEffect->GetTechnique()->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_pEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		}
	}

	void Mesh::SetTexture(ID3D11Texture2D* pTexture, ID3D11ShaderResourceView* pShaderResourceView)
	{
		m_pEffect->SetDiffuseMap(pTexture, pShaderResourceView);
	}
