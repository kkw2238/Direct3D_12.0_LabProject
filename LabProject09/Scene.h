#pragma once

#include "Timer.h"
#include "Shader.h"
#include "Camera.h"

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

protected:
	//���� ���̴����� �����̴�. ���̴����� ���� ��ü���� �����̴�. 
	CGameObject			**m_ppObjects = NULL; 
	int					m_nObjects = 0;

	/* ��Ʈ �ñ״��ĸ� ��Ÿ���� �������̽� ������ * /

	/* ��Ʈ �ñ״��Ķ� ? */
	/* � ���ҽ�(������)���� �׷��Ƚ� ���������ο� ����Ǵ� ���� ���� */
	/* �׷��� ��Ʈ �ñ״���, ��� ��Ʈ �ñ״��İ� �ִµ� �츮�� �׷��� ��Ʈ �ñ״��ĸ� �̿� */

	ID3D12RootSignature *m_pd3dGraphicsRootSignature = NULL;

};

