#pragma once
class Effect;
	struct Vertex_PosCol
	{
		dae::Vector3 Position;
		dae::ColorRGB Color;
	};

	class Mesh
	{
	public:
		Mesh(ID3D11Device* pDevice, std::vector<Vertex_PosCol> vertices, std::vector<uint32_t> indices);
		~Mesh();

		void Render(ID3D11DeviceContext* pDeviceContext);

	private:
		Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};

		ID3D11Buffer* m_pVertexBuffer{};
		ID3D11Buffer* m_pIndexBuffer{};

		uint32_t m_NumIndices{};
	};

