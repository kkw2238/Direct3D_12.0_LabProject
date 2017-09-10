#pragma once

#include "Timer.h"
#include "Shader.h"
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
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);


	/* 따라하기 7번 */

	void ReleaseUploadBuffers();

	//그래픽 루트 시그너쳐를 생성한다. 
	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice); 
	ID3D12RootSignature *GetGraphicsRootSignature();

protected:

	//씬은 셰이더들의 집합이다. 셰이더들은 게임 객체들의 집합이다. 
	CShader **m_ppShaders = NULL;
	int m_nShaders = 0;

	/* 루트 시그니쳐를 나타내는 인터페이스 포인터 * /

		/* 루트 시그니쳐란 ? */
		/* 어떤 리소스(데이터)들이 그래픽스 파이프라인에 연결되는 가를 정의 */
		/* 그래픽 루트 시그니쳐, 계산 루트 시그니쳐가 있는데 우리는 그래픽 루트 시그니쳐를 이용 */

	ID3D12RootSignature *m_pd3dGraphicsRootSignature = NULL;

};

