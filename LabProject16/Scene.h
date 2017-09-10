#pragma once

class CTimer;
class CShader;
class CObjectsShader;
class CCamera;
class CGameObject;
class CHeightMapTerrain;

class CScene
{

public:
	CScene();
	~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera *pCamera);

	/* �����ϱ� 7�� */

	void ReleaseUploadBuffers();

	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�. 
	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice); 
	ID3D12RootSignature *GetGraphicsRootSignature();
	//���� ��� ���� ��ü�鿡 ���� ���콺 ��ŷ�� �����Ѵ�. 
	CGameObject *PickObjectPointedByCursor(int xClient, int yClient, CCamera *pCamera);

protected:
	//��ġ(Batch) ó���� �ϱ� ���Ͽ� ���� ���̴����� ����Ʈ�� ǥ���Ѵ�.
	CObjectsShader *m_pShaders = NULL;
	int m_nShaders = 0;

	/* ��Ʈ �ñ״��ĸ� ��Ÿ���� �������̽� ������ * /

	/* ��Ʈ �ñ״��Ķ� ? */
	/* � ���ҽ�(������)���� �׷��Ƚ� ���������ο� ����Ǵ� ���� ���� */
	/* �׷��� ��Ʈ �ñ״���, ��� ��Ʈ �ñ״��İ� �ִµ� �츮�� �׷��� ��Ʈ �ñ״��ĸ� �̿� */

	ID3D12RootSignature *m_pd3dGraphicsRootSignature = NULL;

protected: 
	CHeightMapTerrain *m_pTerrain = NULL;

public: 
	CHeightMapTerrain *GetTerrain() { return(m_pTerrain); }

};

