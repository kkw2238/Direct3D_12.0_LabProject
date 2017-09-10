#pragma once
#include "Timer.h"

class CGameFramework
{
private:

	/* �ڵ� ���� ���� */
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;

	/* Ŭ���̾�Ʈ ������ ���� */
	int							m_nWndClientWidth;
	int							m_nWndClientHeight;

	/* �������̽� ������ ���� ���� */
	IDXGIFactory4*				m_pdxgiFactory;		// DXGI ���丮 �������̽��� ���� ������
	IDXGISwapChain3*			m_pdxgiSwapChain;	// ���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� ���� �ʿ�
	ID3D12Device*				m_pd3dDevice;		// Direct3D ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.

													/* ���߻��ø� ���� ���� */
													/* MSAA ���� ���ø��� Ȱ��ȭ�ϰ� ���� ���ø� ������ ���� */

													/* ���ø��̶� ? �ػ󵵿� ���� �ʺ� , ���̸� x�� �� ���� ��ȭ������ �׸��� */
													/* Ŭ���̾�Ʈ ������ ������ ���� �� x = 2 ���� 4��� �þ Pixel�� ��� ������ ���Ͽ� */
													/* �ϳ��� ������ִ� ��Ƽ�ٸ���� ��� */
	bool						m_bMsaa4xEnable = false;	// ���ø� ����
	UINT						m_nMsaa4xQualityLevels = 0;	// ���ø� ǰ�� ���� 0 �̸� ��� ���� 

															/* ���� ü�� �ĸ� ���� ���� ���� */
	static const UINT			m_nSwapChainBuffers = 2;	// ���� ü���� �ĸ� ������ ����
	UINT						m_nSwapChainBufferIndex;	// ���� ü���� �ĸ� ���� �ε��� 

															/* ���� Ÿ�� ���� , ������ �������̽� ���� ���� */
															/* ������( Descriptor ) : ���ҽ��� ���� �����ϱ� ���� �� */
															/* DescriptorHeap : ������ �迭 */
	ID3D12Resource*				m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];	// ���� Ÿ�� ����
	ID3D12DescriptorHeap*		m_pd3dRtvDescriptorHeap;							// ������ �� �������̽� ������
	UINT						m_nRtvDescriptorIncrementSize;						// ���� Ÿ�� ������ ������ ũ��

																					/* ����-���ٽ� ���۰��� ���� */
	ID3D12Resource*				m_pd3dDepthStencilBuffer;		// ���� - ���ٽ� ����
	ID3D12DescriptorHeap*		m_pd3dDsvDescriptorHeap;			// ������ �� �������̽� ������
	UINT						m_nDsvDescriptorIncrementSize;	// ���� - ���ٽ� ������ ������ ũ��

																/* ��� ť , ��� �Ҵ��� , ��� ����Ʈ �������̽� ���� ���� */
																/* CommandQueue & List : GPU�� 1���� ��� Queue�� �����ְ� CPU�� CommandList�� �־��ָ� GPU�� 1�����Ӹ��� ������� �����Ѵ�. */
																/* Allocator : ��� �Ҵ��� - ��� ����Ʈ�� �޸𸮸� �Ҵ� �޴´�. */
	ID3D12CommandQueue*			m_pd3dCommandQueue;		// ��� ť �������̽� ������
	ID3D12CommandAllocator*		m_pd3dCommandAllocator;	// ��� �Ҵ��� �������̽� ������
	ID3D12GraphicsCommandList*	m_pd3dCommandList;		// ��� ����Ʈ �������̽� ������

														/* Fence ���� ���� */
														/* Fence( ��Ÿ�� ) : CPU�� GPU�� ����ȭ�� ���ֱ� ���� ��Ÿ�� ������ �Ѵ�. */
														/* Handle : Physical �޸��� �ּҸ� ��Ÿ���� �������� ���� */
	ID3D12Fence*				m_pd3dFence;	// �������̽� ������ 
	UINT64						m_nFenceValue;	// �潺�� �� ( �ʱ� �� )
	HANDLE						m_hFenceEvent;	// �̺�Ʈ �ڵ�

#if defined(_DEBUG)
												/* Debug Layer Interfaces */
	ID3D12Debug*				m_pd3dDebugController;
#endif

	/* ����Ʈ , ���� ���� ���� */
	/* ����Ʈ : �׸��� �׷��� Client������ ���� */
	/* ����   : ������ �ϰ� ���� �κ��� ���� */
	D3D12_VIEWPORT				m_d3dViewport;		// ����Ʈ
	D3D12_RECT					m_d3dScissorRect;	// ���� �簢��

													/* ������ ���������� ���� ���� */
													/* ������ ���������� : GPU�� ����Ͽ� ���ҽ� ( ���� / �ε��� , �ؽ��� ) �� 2D �̹����� ������ �ϴ� ���� */
													/* ��Ȯ���� ����Ϳ� ��� �������� ���� */
	ID3D12PipelineState*		m_pd3dPipelineState;// ������ ������ ���¸� ��Ÿ���� ���� 

	CGameTimer					m_GameTimer;
	_TCHAR						m_pszFrameRate[50];

public:

	CGameFramework();
	~CGameFramework();

	/* ������ ��ũ ���� �Լ� */
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);	// ������ ��ũ�� �ʱ�ȭ �ϴ� �Լ� ( �� �����찡 �����Ǹ� ȣ�� )

	void OnDestroy();									// �����찡 ���� �� ȣ�� �Ǵ� �Լ� 

														/* ����ü�� , ����̽� , ������ �� , ��� ť / �Ҵ��� / ����Ʈ�� �����ϴ� Create �Լ� */
	void CreateSwapChain();					// ���� ü�� ����
	void CreateDirect3DDevice();			// Direct3DDevice �������̽� ���� �Լ� 
											// Rtv : Render Target View   
											// Dsv : Depth Stencil View 
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateCommandQueueAndList();		// CommandQueue & List �������̽��� �����ϴ� �Լ�
	void CreateRenderTargetView();			// ���� Ÿ�� �� �����ڸ� �����ϴ� �Լ� 
	void CreateDepthStencilView();			// ���� - ���ٽ� ���ۿ� �����ڸ� �����ϴ� �Լ�

											/* �������� �޽��� ��ü ���� �Լ� */
	void BuildObjects();	// �������� ��ü�� �����ϴ� �Լ�
	void ReleaseObjects();	// �������� ��ü�� �Ҹ��Ű�� �Լ�

							/* ����� �Է� , �ִϸ��̼� , ������ ���� �Լ� */
	void ProcessInput();	// ����� �Է��� ó���ϴ� �Լ�
	void AnimateObjects();	// ������Ʈ���� �������� ó���ϴ� �Լ�
	void FrameAdvance();	// ������Ʈ���� ������ ���ִ� �Լ�

							/* CPU & GPU ����ȭ ���� �Լ� */
	void WaitForGpuComplete();	// GPU�� ���� ���� �� ���� CPU�� ��ٸ��� �ϴ� �Լ�.

								/* �������� �޽��� ( Ű���� ���콺 �Է� �� ) ó�� ���� �Լ� */

								/* ���콺 �Է��� ó���ϴ� �Լ� */
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	/* Ű���� �Է��� ó���ϴ� �Լ� */
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	/* ������ �Է�( ��üȭ�� ��ȯ �� )�� ó���ϴ� �Լ� */
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

