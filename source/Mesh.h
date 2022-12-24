#pragma once
class Effect;
	struct Vertex
	{
		dae::Vector3 position{};
		dae::Vector3 colour{ 1,1,1 };
		dae::Vector2 uv{};
	};

	class Mesh
	{
	public:
		Mesh(ID3D11Device* pDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Mesh();

		void Render(ID3D11DeviceContext* pDeviceContext, dae::Matrix& worldViewProjection) const;
		void SetTexture(ID3D11Texture2D* pTexture, ID3D11ShaderResourceView* pShaderResourceView);

	private:
		Effect* m_pEffect{};

		ID3D11Buffer* m_pVertexBuffer{};
		ID3D11Buffer* m_pIndexBuffer{};

		uint32_t m_NumIndices{};
	};

