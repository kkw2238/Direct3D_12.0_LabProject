#pragma once

#include "Timer.h"

class CScene
{
public:
	CScene();
	~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device* pd3dDevice);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);

			/* 루트 시그니쳐를 나타내는 인터페이스 포인터 */
	
	/* 루트 시그니쳐란 ? */
	/* 어떤 리소스(데이터)들이 그래픽스 파이프라인에 연결되는 가를 정의 */
	/* 그래픽 루트 시그니쳐, 계산 루트 시그니쳐가 있는데 우리는 그래픽 루트 시그니쳐를 이용 */

	ID3D12RootSignature* m_pd3dGraphicsRootSignature;

	/* 파이프 라인 상태를 나타내는 인터페이스 포인터 */
	ID3D12PipelineState* m_pd3dPipelineState;
};

