#pragma once


class CCamera;
class CGameObject;
class CPlayer;

//���� ��ü�� ������ ���̴����� �Ѱ��ֱ� ���� ����ü(��� ����)�̴�. 
struct CB_GAMEOBJECT_INFO { 

	XMFLOAT4X4 m_xmf4x4World;

};


//���̴� �ҽ� �ڵ带 �������ϰ� �׷��Ƚ� ���� ��ü�� �����Ѵ�. 
class CShader { 

public:    
	CShader() {};
	virtual ~CShader();

private:
	int						m_nReferences = 0;

public: 
	void AddRef() { m_nReferences++; } 
	void Release() { if (--m_nReferences <= 0) delete this; }

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState(); 
	virtual D3D12_BLEND_DESC CreateBlendState(); 
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	
	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob **ppd3dShaderBlob);
	
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList); 
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList); 
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	virtual void OnPrepareRender(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);

protected: //���̴��� �����ϴ� ���� ��ü���� ����Ʈ(�迭)�̴�. 
	ID3D12PipelineState**	m_ppd3dPipelineStates = NULL; 
	int						m_nPipelineStates = 0;

};

class CPlayerShader : public CShader {

public: 
	CPlayerShader(); 
	virtual ~CPlayerShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();	
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob); 
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

};

//��CObjectsShader�� Ŭ������ ���� ��ü���� �����ϴ� ���̴� ��ü�̴�. 
class CObjectsShader : public CShader {

public:   
	CObjectsShader() {};
	virtual ~CObjectsShader() {};
	
	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext);
	virtual void AnimateObjects(float fTimeElapsed); 
	virtual void ReleaseObjects();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout(); 
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob); 
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void ReleaseUploadBuffers();

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
public: 
	//���̴��� ���ԵǾ� �ִ� ��� ���� ��ü�鿡 ���� ���콺 ��ŷ�� �����Ѵ�.
	virtual CGameObject *PickObjectByRayIntersection(XMFLOAT3& xmf3PickPosition, XMFLOAT4X4& xmf4x4View, float *pfNearHitDistance);

protected:
	CGameObject **m_ppObjects = NULL; 
	int m_nObjects = 0;

};


/* 

	�����ϱ� 10�������� ��ü�� ���̴� ��ü�� �޽��� ���� �����͸� ���� �ִ� �����̴�.
	�׸��� Scene��ü�� ���� ��ü���� �迭�� ���� ������ Scene�� ������ �ϱ� ���� 
	���� ��ü���� ������� ������ �Ѵ�.
	�� ���� ��ü�� ������ �ϱ����� �Ź� ���̴� ��ü�� �������ϰ� �޽� ������ �����ؾ� �Ѵ�.

	�̷��� ���������� �� ��ü�� ������ �ϱ����� Device�� ���¸� �����ؾ� �Ѵ�.
	���� �����ϴ� �Yü�� ���� ���� ��� ���� ������ ���� �ð��� �þ �� �����Ƿ� ������ ����Ʈ�� ������ �� �ִ�.

*/

/* 

	�����ϱ� 11�������� ���̴� ��ü�� ���� ��ü���� �迭�� ������ �ִ� ������ ������ �ִ�.
	���̴� ��ü�� �޽��� ���� �����͸� ���� ���� �ִ�. 
	�׸��� �� ��ü�� ���̴� ��ü���� �迭�� ������ ������ ���� ������ �ϱ� ���ؼ� ���̴� ��ü���� ������� ������ �Ѵ�.

	�� ���̴� ��ü�� ������ �ϱ� ���ؼ� ���̴� ������ �ѹ��� �����ϰ� ���� ��ü���� �迭�� ���Ե� ��� ��ü���� ������� �������ϸ� �ȴ�.

	�̷��� ���������� ���� ������ �ϱ� ���� ���̴� ������ ��ü�� ������ŭ �������� �ʾƵ� �ȴ�.
	= ����̽� ���� ��ȭ�� ������ �� �ִ�.

*/

class CTerrainShader : public CShader {

public: 
	CTerrainShader();
	virtual ~CTerrainShader();
	
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

};
