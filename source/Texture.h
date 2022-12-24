#pragma once
class Texture
{
public:
	Texture() = default;
	~Texture();

	ID3D11Texture2D* LoadFromFile(ID3D11Device* pDevice, const std::string& path);
	ID3D11ShaderResourceView* GetShaderResourcceView() const;

private:
	ID3D11Texture2D* m_pResource{};
	ID3D11ShaderResourceView* m_pResourceView{};
};

