#pragma once
	class Effect
	{
	public:
		Effect(ID3D11Device* pDevice, const std::wstring& assetFilePath);
		~Effect();

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFilePath);

		ID3DX11Effect* GetEffect() const;
		ID3DX11EffectTechnique* GetTechnique() const;
		ID3D11InputLayout* GetInputLayout() const;
		ID3DX11EffectMatrixVariable* GetWorldViewProjectionMatrix() const;

		void SetDiffuseMap(ID3D11Texture2D* pTexture, ID3D11ShaderResourceView* pShaderResourceView);
	private:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};

		ID3D11InputLayout* m_pInputLayout{};

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
	};