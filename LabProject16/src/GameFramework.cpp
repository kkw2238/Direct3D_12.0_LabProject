#include "stdafx.h"

#include "GameFramework.h"
#include "Scene.h"
#include "Player.h"


CGameFramework::CGameFramework()	// O
{

	m_pdxgiFactory = NULL;
	m_pdxgiSwapChain = NULL;
	m_pd3dDevice = NULL;

	m_pd3dCommandAllocator = NULL;
	m_pd3dCommandQueue = NULL;
	m_pd3dPipelineState = NULL;
	m_pd3dCommandList = NULL;

	for (int i = 0; i < m_nSwapChainBuffers; ++i)
		m_ppd3dRenderTargetBuffers[i] = NULL;

	m_pd3dRtvDescriptorHeap = NULL;
	m_nRtvDescriptorIncrementSize = 0;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDsvDescriptorHeap = NULL;
	m_nDsvDescriptorIncrementSize = 0;

	m_nSwapChainBufferIndex = 0;

	m_hFenceEvent = NULL;
	m_pd3dFence = NULL;
	m_nFenceValue = 0;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	_tcscpy_s(m_pszFrameRate, _T("LapProject ("));

	/*		�����ϱ� 6��		 */
	for (int i = 0; i < m_nSwapChainBuffers; i++) 
		m_nFenceValues[i] = 0; 
	m_pScene = NULL;

}


CGameFramework::~CGameFramework()
{
}


/* ������ ��ũ ���� �Լ� */

/*   ������ ��ũ�� �ʱ�ȭ �ϴ� �Լ� ( �� �����찡 �����Ǹ� ȣ�� )   */
bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)	// O
{

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	/* Direct3D ����̽� , ��� Queue , ��� List , ����ü�� ���� �����ϴ� �Լ��� ȣ���Ѵ�. */
	CreateDirect3DDevice();
	CreateCommandQueueAndList();
	CreateRtvAndDsvDescriptorHeaps();
	CreateSwapChain();
	
	/* Create Event ( lpEventAttributes , bManualReset , bInitialState , lpName ) : */
	/* lpEventAttributes : NULL - ������ �̺�Ʈ �ڵ��� �ڽ� ���μ����� ��� X */
	/* bManualReset : FALSE - �ڵ����� �̺�Ʈ�� �����Ǵ� �̺�Ʈ ��ü�� ���� */
	/* bInitialState : FALSE - �̺�Ʈ�� �߻����� ���� ä�� ���� */
	/* IpName : �̺�Ʈ ��ü�� ����� �̸� ������ */
	/* return : ������ - �̺�Ʈ ��ü�� �ڵ��� ��ȯ , ���н� - NULL ��ȯ */
	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	/* �������� ��ü( ���� ���� ��ü)�� �����Ѵ�. */
	BuildObjects();

	return true;


}


void CGameFramework::OnDestroy()
{

	ReleaseObjects();				// ��ü�� ���ش�.

	::CloseHandle(m_hFenceEvent);	// �Ҵ� ���� �̺�Ʈ �ڵ��� Ǯ���ش�.

#if defined(_DEBUG)
	if (m_pd3dDebugController) m_pd3dDebugController->Release();
#endif

	for (int i = 0; i < m_nSwapChainBuffers; ++i)
		if (m_ppd3dRenderTargetBuffers[i])
			m_ppd3dRenderTargetBuffers[i]->Release();
	if (m_pd3dRtvDescriptorHeap)	m_pd3dRtvDescriptorHeap->Release();

	if (m_pd3dDepthStencilBuffer)	m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDsvDescriptorHeap)	m_pd3dDsvDescriptorHeap->Release();

	if (m_pd3dCommandAllocator)		m_pd3dCommandAllocator->Release();
	if (m_pd3dCommandQueue)			m_pd3dCommandQueue->Release();
	if (m_pd3dCommandList)			m_pd3dCommandList->Release();

	if (m_pd3dFence)				m_pd3dFence->Release();

	m_pdxgiSwapChain->SetFullscreenState(FALSE, NULL);	// Window���� �ٲ��ش�. 
	if (m_pdxgiSwapChain)			m_pdxgiSwapChain->Release();
	if (m_pd3dDevice)				m_pd3dDevice->Release();
	if (m_pdxgiFactory)				m_pdxgiFactory->Release();

}


/* ����ü�� , ����̽� , ������ �� , ��� ť / �Ҵ��� / ����Ʈ�� �����ϴ� Create �Լ� */

/*       ���� ü�� �����ϴ� �Լ�       */
void CGameFramework::CreateSwapChain()						// O
{

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	
	dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

#ifdef _WITH_ONLY_RESIZE_BACKBUFFES
	// ��üȭ�� ��忡�� ����ȭ�� �ػ󵵸� �������� �ʰ� �ĸ� ������ ũ�⸦ ����ȭ�� ũ��� ����
	dxgiSwapChainDesc.Flags = 0;
#else
	// ��üüȭ�� ��忡�� ����ȭ���� �ػ󵵸� ����ü���� ũ�⿡ �°� ����
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#endif

	HRESULT hResult = m_pdxgiFactory->CreateSwapChain(m_pd3dCommandQueue, &dxgiSwapChainDesc,
		(IDXGISwapChain **)&m_pdxgiSwapChain);

	m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);		// ALT + ENTER�� ��üȭ�� ��ȯ���� ���� ����
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();	// ���� ����ü���� �ĸ� ������ �ε����� ���´�..
}

/*   Direct3DDevice �������̽� ���� �Լ�   */
void CGameFramework::CreateDirect3DDevice()		 // O 		
{

#if defined(_DEBUG)
	D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&m_pd3dDebugController);
	//m_pd3dDebugController->EnableDebugLayer();
#endif

	/*                     DXGI ���丮�� �����Ѵ�.                     */

	/* CreateDXGIFactory1( REFIID riid , void** ppFactory ) */
	/* REFIID : ppFactory �Ű� �������� �����ϴ� DXGIFactory��ü�� GUID */
	/* void** : IDXGIFactory��ü�� ���� ����Ʈ�� �ּҰ� => [Out] ���� ����Ǵ� �� */
	/* GUID : �������̽� Ŭ���� �ĺ���(ID) �� ��Ÿ���� 128-��Ʈ ���� ���ڿ� */
	/* __uuidof : �������̽� �ڷ��� , Ŭ���� �̸� , �������̽� �����͸� ��ȯ���ִ� ������ */
	/* IDXGIFactory : DXGI�� ����� ���� �������̽��� ������ �������̽��̴�. �׷��� ��ȯ ���� **�� �� �ִ�. */
	::CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&m_pdxgiFactory);


	/*     ��� �ϵ���� ����Ϳ� ���Ͽ� Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� �����Ѵ�.     */

	IDXGIAdapter1* pd3dAdapter = NULL;

	/* EnumAdapters1( UINT Adapter , IDXGIAdapter1 **ppAdapter ) */
	/* UINT : ����ȭ�� Adapter�� �ε��� */
	/* IDXGIAdapter1 : [Out] UINT��°�� Adatper �����͸� ���� IDXGIAdapter1 �������̽��� ������ �ּ� �� . ���� NULL ��� �Ұ� */
	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != m_pdxgiFactory->EnumAdapters1(i, &pd3dAdapter); ++i)
	{
		/* DXGI_ADAPTER_DESC1 : DXGI�� ����� Video Card ������*/
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc;

		/* GetDesc1 : ������� DXGI 1.1 �����ڸ� ���´�. */
		pd3dAdapter->GetDesc1(&dxgiAdapterDesc);

		/* & : ��Ʈ ������ and */
		/* DXGI_ADPATER_FLAG_SOFTWARE : �׷��� ī�尡 �ƴ� �ü������ �⺻������ CPU�� ���� �����ϴ� ������ ����� */
		if (dxgiAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)	// dxgiAdapterDesc.Flags�� DXGI_ADPATER_FLAG_SOFTWARE ��Ʈ�� ���ԵǾ� �־�? 
			continue;

		/* CreateDevice( IUnknown* pAdapter , D3D_FEATURE_LEVEL MinimumFeaturelevel , const IID& riid , void** ppDevice ) */
		/* IUnknown : ����̽��� ���� �� �� ���� ����� �������̴�. NULL���� �⺻ Adapter ��� */
		/* D3D_FEATURE_LEVEL : Adapter �����Ͱ� �����ؾ� �ϴ� �ּ� FEATURE LEVEL ���̴�. DirectX 12.0�� �̿��� ���̱⿡ 12�� ���� */
		/* REFIID : ����̽� �������̽��� ���� GUID ( Globally Unique IDentifier ) */
		/* void** : [Out] ��ȯ�Ǵ� Device �����͸� �ޱ����� �޸� �� */
		/* Adapter�� DirectX3D�� �����ϰ� ID3D12Device�� ���������� ����� m_pd3dDevice�� �����ߴٸ� for������ ������  */
		/* Device : ? */
		if (SUCCEEDED(D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_12_0,
			__uuidof(ID3D12Device), (void**)&m_pd3dDevice)))
			break;
	}

	/*          Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ WARP ����̽��� �����Ѵ�.          */
	/* WARP Device : �ϵ���������� D3D�� ������ ���� �ƴ����� �ü������ Adapter�� Emulater �ϰ� �׷��ش�. */

	if (!pd3dAdapter)
	{
		/* EnumWarpAdapter(const RFIID& riid , void** ppAdapter) */
		/* RIID : ppAdapter���� ������ IDXGIFactory4 ��ü�� GUID */
		/* ppAdapter : [Out] Adapter�� �������̽� �������� �ּҰ� . ���� NULL��� �Ұ� */
		m_pdxgiFactory->EnumWarpAdapter(__uuidof(IDXGIFactory4), (void**)&pd3dAdapter);
		D3D12CreateDevice(pd3dAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&m_pd3dDevice);
	}

	/*          ����̽��� �����ϴ� ���� ������ ǰ�� ������ Ȯ��          */

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dMsaaQualityLevels.SampleCount = 4;

	/* D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS : ǰ�� ������ �����ϱ� ���� �ɼ��� �����Ѵ�. */
	/* D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE : �ƹ� �ɼ��� �������� �ʰڴ�. */
	/* D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_TILED_RESOURCE : Ÿ�ϸ��� ���ҽ��� ǰ�� ��ġ�� ���� �� �ִ�. */
	d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	d3dMsaaQualityLevels.NumQualityLevels = 0;						// QualityLevel�� 0���� �ʱ�ȭ

																	/* CheckFeatureSupport(D3D_FEATURE feature, void* pFeatureSupportData, UINT FeatureSupportDataSize) */
																	/* D3D_FEATURE : ������ �䱸�ϴ� ����� ������ */
																	/* void* : [In, Out] ��� �䱸 �����ڷ� ä���� ����ü */
																	/* UNIT : pFeatureSupportData�� Size */
																	/* ���� �׷��� ī�忡�� �����ϴ� ��ɵ鿡 ���� ������ ����� */
	m_pd3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
	m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;	// �׷��� ī�尡 �����ϴ� ���� ���� ǰ�� ������ Ȯ���Ѵ�.
																	// ���⼭�� â�� ���°� ��ǥ�̱� ������ 0���� ���� ��?
	m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;

	/*          �潺�� �����ϰ� �潺 ���� 1�� �����Ѵ�.          */

	/* CreateFence(UINT64 InitialValue, D3D_FENCE_FLAGS Flags, const IID& riid, void** ppFence) */
	/* UINT64 : Fence�� �ʱⰪ */
	/* Flags : ��Ʈ OR �����ڸ� ����Ͽ� ������ Fence�� �ɼǰ� */
	/* RIID (Reference Interface ID) : ID3D12Fence �������̽��� GUID */
	/* void** : fence�� ���� �ϱ����� ��� �� ID3D12Fence �������̽��� �����͸� �ޱ����� �޸� �� ������ */
	m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&m_pd3dFence);
	m_nFenceValue = 1;

	if (pd3dAdapter) pd3dAdapter->Release();

}

/* CommandQueue & List : GPU�� 1���� ��� Queue�� �����ְ� CPU�� CommandList�� �־��ָ� GPU�� 1�����Ӹ��� ������� �����Ѵ�. */

/* CommandQueue & List �������̽��� �����ϴ� �Լ� */
void CGameFramework::CreateCommandQueueAndList()	// O
{
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc;
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	/* D3D12_COMMAND_QUEUE_FLAGS : GPU�� ���ϴ� �ð��� �ʹ� �ʾ�( Time Out )! Queue�� ���� ���� Frame�� ��� List�� ä�ﲨ��? */
	/* D3D12_COMMAND_QUEUE_FLAG_NONE : Time Out �̸� Queue�� ���� ���� Frame�� ��� List�� ä�ﲨ�� */
	/* D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT : �ƴ� GPU�� ���� ������ ���� ��ٸ��ž� */
	d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	/* D3D12_COMMAND_LIST_TYPE_DIRECT : GPU�� ���� ��� ������ Queue�� ����ž� */
	d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	/* HRESULT : �Լ��� ����� ��Ÿ���� 32��Ʈ �� */
	/* Command Queue : GPU�� ���� �ִ� 1�� �̻��� CommandList�� ���� */
	HRESULT hResult = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc,	// ���� (Direct) ��� Queue�� �����Ѵ�.
		__uuidof(ID3D12CommandQueue), (void**)&m_pd3dCommandQueue);

	/* Command Allocator : ��� �Ҵ��� , GPU ����� �����ϱ� ���� �޸𸮸� �Ҵ��Ѵ�. ����̽��� ��� �Ҵ��ڸ� ���Ͽ� ��� List�� �����Ѵ�. */
	hResult = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		__uuidof(ID3D12CommandAllocator), (void**)&m_pd3dCommandAllocator);

	/* Command List : GPU�� ������ ��ɵ��� ����ȭ�� ����, ������ �� Open���·� �����ȴ�. Open������ ������ ����� ���� ���� �� �ִ�. */
	/* 1���� Allocator ���� 2�� �̻��� List�� Open�� �� ����. */
	/* CrateCommandList(UINT nodeMask, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* pCommandAllocator, */
	/* ID3D12PipelineState* pInitialState, RIID riid, void** ppCommandList */
	/* UINT : ��ɾ� ����Ʈ�� ������ ��� ����� �ϳ��� GPU�� ��� 0 */
	/* D3D12_COMMAND_LIST_TYPE : ��ɾ� ����Ʈ�� ���� */
	/* ID3D12CommandAllocator : ��ɾ� ����Ʈ�� ������ ��� �Ҵ��� */
	/* ID3D12PipelineState : ���� ������ ������ ���� , NULL ���� ���̺귯���� �ڵ����� ���¸� ���� */
	/* RIID : CommandList�� GUID */
	/* void** : [Out] CommandList�� �����ϱ����� ����� ID3DICommandList �������̽� �����͸� �ޱ� ���� �޸� �� ������ */
	hResult = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator, NULL,
		__uuidof(ID3D12CommandList), (void**)&m_pd3dCommandList);

	hResult = m_pd3dCommandList->Close();	// �ʱ� Open �Ǿ��ִ� �����̹Ƿ� Close �ʾ� ! 

}

/*       RTV , RSV ������ ���� �����ϴ� �Լ�       */
void CGameFramework::CreateRtvAndDsvDescriptorHeaps()	// O
{
	// Rtv : Render Target View   
	// Dsv : Depth Stencil View 
	// DescriptorHeap : View , Descriptor�� �����ϱ� ���� �������� �޸� ( �迭 ) 
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

	/*       ���� Ÿ�� ������ ��( �������� ������ ����ü�� ������ ����)�� �����Ѵ�.       */

	d3dDescriptorHeapDesc.NumDescriptors = m_nSwapChainBuffers;				// �������� ���� = ����ü�� ������ ���� 
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// RenderTargetView Ÿ���� Heap 
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// D3D12_DESCRIPTOR_HEAP_FLAG_NONE : CPU�� �ʰ� ������
	d3dDescriptorHeapDesc.NodeMask = 0;								// �� ��° GPU��? �̱� GPU = 0

	HRESULT hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc,// ������ Heap�� �����Ѵ�.
		__uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dRtvDescriptorHeap);

	/*       ���� Ÿ�� ������ ���� ������ ũ�⸦ ����       */

	/* GPU���� �������� ũ�Ⱑ �ٸ��� ! ���� ���ҷ� �Ѿ�� ���� ���� ���� ũ��(IncrementSize)�� ����(Get) �ؾ��Ѵ�. */
	m_nRtvDescriptorIncrementSize =
		m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	/*       ����-���ٽ� ������ ��( �������� ������ 1 )�� �����Ѵ�.       */

	/* ����-���ٽ��� 1���� �ʱ�ȭ - ��� - �ʱ�ȭ - ��� ������ ����ϱ� ������ 1���� ����ϴ�. */
	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc,
		__uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);

	/*       ����-���ٽ� ������ ���� ������ ũ�⸦ ����       */

	m_nDsvDescriptorIncrementSize =
		m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);



}

/* ���� Ÿ�� �� �����ڸ� �����ϴ� �Լ� */
void CGameFramework::CreateRenderTargetView()	// O
{
	HRESULT hResult;

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle =
		m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	/*       ����ü���� �� �ĸ� ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.       */

	for (UINT i = 0; i < m_nSwapChainBuffers; ++i)
	{
		hResult = m_pdxgiSwapChain->GetBuffer(i,
			__uuidof(ID3D12Resource), (void**)&m_ppd3dRenderTargetBuffers[i]);
		m_pd3dDevice->CreateRenderTargetView(m_ppd3dRenderTargetBuffers[i], NULL,
			d3dRtvCPUDescriptorHandle);
		d3dRtvCPUDescriptorHandle.ptr += m_nRtvDescriptorIncrementSize;
	}

}

/* ���� - ���ٽ� ���ۿ� �����ڸ� �����ϴ� �Լ� */
void CGameFramework::CreateDepthStencilView()		// X
{

	/*       D3D12_RESOURCE �����ڸ� ä���ش�.       */

	D3D12_RESOURCE_DESC d3dResourceDesc;
	/*	���ҽ��� 2���� �ؽ��ķ� ��� �Ұž� .	*/
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = m_nWndClientWidth;
	d3dResourceDesc.Height = m_nWndClientHeight;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dResourceDesc.SampleDesc.Count = 1;
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	/*       */

	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	/* Type : ���ҽ��� CPU , GPU ���� ���� ���θ� ���� */
	d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;						// DEFAULT : GPU�� ���� ���� CPU�� ���� �Ұ�
																			/* CPUPageProperty : ���� ���� CPU-������ �Ӽ� */
	d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;	// UNKNOWN : �ü���� �ʰ� �˾Ƽ� ���� ��
																			/* HHeap�� ���ҽ��� ���� CPU�� GPU�� �뿪�� �Ӽ� */
	d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;		// UNKNOWN : �ü���� �ʰ� �˾Ƽ� ����
																			/* CreationNodeMask : GPU ����� Index */
	d3dHeapProperties.CreationNodeMask = 1;									// 0 , 1 : ���� GPU �����
																			/* ���� ������ �߿��� ���ҽ��� �� �� �ִ� ������ ��Ÿ���� ��Ʈ */
	d3dHeapProperties.VisibleNodeMask = 1;

	/*       ���� - ���ٽ� ���۸� �����Ѵ�.       */

	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;
	/* CreateCommittedResource(const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAG Heapflags, const D3D12_RESOURCE_DESC* pDesc,
	D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, const IID& riidSource, void** ppvResouce) */

	/* CreateCommittedResource : Heap�� ���ҽ��� �����ϴ� �Լ� Heap�� ������ ���ҽ��� ������ �� �ִ� ����� �޸𸮸� ���� �־�� �Ѵ�. */
	/* D3D12_HEAP_PROPERTIES : Heap�� ���� ���� ������ */
	/* D3D12_HEAP_FLAGS : ��Ʈ OR�������� ���յ� Heap �ɼ� */
	/* D3D12_RESOURCE_DESC : ���ҽ� ������ ������ */
	/* D3D12_RESOURCE_STATE : ��Ʈ OR�������� ���յ� �ڿ��� �ʱ���� */
	/* D3D12_CREAR_VALUE : ���� - ���ٽ� ������ �ʱⰪ�� �����Ѵ�. */
	/* REFIID : Resource �������̽��� GUID */
	/* void** : �䱸�Ͽ� ������ ���ҽ� ��ü�� �����͸� ���� �޸� ������ */
	m_pd3dDevice->CreateCommittedResource(&d3dHeapProperties, D3D12_HEAP_FLAG_NONE,
		&d3dResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue,
		__uuidof(ID3D12Resource), (void**)&m_pd3dDepthStencilBuffer);

	/*       ���� - ���ٽ� �並 �����Ѵ�.       */

	/* GetCPUDescriptorHandleForHeapStart : ���� ������ ��Ÿ���� CPU ������ �ڵ��� �����´�. */
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle =
		m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	/* CreateDepthStencilView( ID3D12Resource* pResource, const D3D12_DEPTH_STENCIL_VIEW_DESC* pDesc,
	D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor */
	/* ID3D12Resource : ���� - ���ٽ��� ��Ÿ���� ���� Resource ������Ʈ�� ������ */
	/* D3D12_DEPTH_STENCIL_VIEW_DESC : ���� - ���ٽ� View �������������̴�. NULL���� �ʱ� default������ ��� */
	/* D3D12_CPU_DESCRIPTOR_HANDLE : ���� - ���ٽ� View�� ������ ���� ������ ��Ÿ���� CPU ������ �ڵ� */
	/* ���ҽ� �����Ϳ� ���� �ϱ����� ���� - ���ٽ� View�� �����Ѵ�. */

	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, NULL, d3dDsvCPUDescriptorHandle);


}




/* �������� �޽��� ��ü ���� �Լ� */

/*   �������� ��ü�� �����ϴ� �Լ�   */
void CGameFramework::BuildObjects()
{

	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	m_pScene = new CScene(); 
	
	if (m_pScene) 
		m_pScene->BuildObjects(m_pd3dDevice, m_pd3dCommandList);

	m_pPlayer = new CTerrainPlayer(m_pd3dDevice, m_pd3dCommandList, m_pScene->GetGraphicsRootSignature(), m_pScene->GetTerrain(), 1); 
	
	m_pCamera = m_pPlayer->GetCamera();
	m_pd3dCommandList->Close(); 
	
	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList }; 
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete(); 
	if (m_pScene) 
		m_pScene->ReleaseUploadBuffers();

	m_GameTimer.Reset();

}

/*   �������� ��ü�� �Ҹ��Ű�� �Լ�   */
void CGameFramework::ReleaseObjects()
{
	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;

}


/* ����� �Է� , �ִϸ��̼� , ������ ���� �Լ� */

/*    ����� �Է��� ó���ϴ� �Լ�    */
void CGameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256]; 
	
	DWORD dwDirection = 0; 
	
	/*
		Ű������ ���� ������ ��ȯ�Ѵ�. ȭ��ǥ Ű(���桯, ���硯, ���衯, ���顯)�� ������ �÷��̾ ������/����(���� x-��), ��/ ��(���� z-��)�� �̵��Ѵ�. 
		��Page Up���� ��Page Down�� Ű�� ������ �÷��̾ ��/�Ʒ�(���� y-��)�� �̵��Ѵ�.
	*/ 
	if (::GetKeyboardState(pKeyBuffer)) 
	{ 
		if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD; 
		if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD; 
		if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT; 
		if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT; 
		if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN; 
	} 
	
	float cxDelta = 0.0f, cyDelta = 0.0f; 
	POINT ptCursorPos; 
	
	/*
		���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��� ���� ����Ѵ�. 
		���콺 ���� �Ǵ� ������ ��ư�� ������ ���� �޽���(WM_LBUTTONDOWN, WM_RBUTTONDOWN)�� ó���� �� ���콺�� ĸ���Ͽ���. 
		�׷��Ƿ� ���콺�� ĸ�ĵ� ���� ���콺 ��ư�� ������ ���¸� �ǹ��Ѵ�. 
		���콺 ��ư�� ������ ���¿��� ���콺�� �¿� �Ǵ� ���Ϸ� �����̸� �� ���̾ x-�� �Ǵ� y-������ ȸ���Ѵ�.
	*/ 
	if (::GetCapture() == m_hWnd) { 
		//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�). 
		::SetCursor(NULL); 
		
		//���� ���콺 Ŀ���� ��ġ�� �����´�. 
		::GetCursorPos(&ptCursorPos); 
		
		//���콺 ��ư�� ���� ���¿��� ���콺�� ������ ���� ���Ѵ�. 
		cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f; 
		cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f; 
		
		//���콺 Ŀ���� ��ġ�� ���콺�� �������� ��ġ�� �����Ѵ�. 
		::SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y); 
	} 
	
	//���콺 �Ǵ� Ű �Է��� ������ �÷��̾ �̵��ϰų�(dwDirection) ȸ���Ѵ�(cxDelta �Ǵ� cyDelta). 
	if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f)) 
	{ 
		//��ŷ���� ������ ���� ��ü�� ������ Ű���带 �����ų� ���콺�� �����̸� ���� ��ü�� �̵� �Ǵ� ȸ���Ѵ�. 
		if (m_pSelectedObject) 
		{ 
			ProcessSelectedObject(dwDirection, cxDelta, cyDelta);
		} 
		else 
		{
			if (cxDelta || cyDelta)
			{ 

				if (pKeyBuffer[VK_RBUTTON] & 0xF0)
					m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
				else 
					m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);

			}
			if (dwDirection)
				m_pPlayer->Move(dwDirection, 50.0f * m_GameTimer.GetTimeElapsed(),true); 
		} 
	}
		
	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());

}

/* ������Ʈ���� �������� ó���ϴ� �Լ� */
void CGameFramework::AnimateObjects()
{
	if (m_pScene) m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

/*  ������Ʈ���� ������ ���ִ� �Լ�  */
void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(0.0f);
	
	ProcessInput();
	AnimateObjects();
	
	HRESULT hResult = m_pd3dCommandAllocator->Reset(); 
	
	hResult = m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);
	
	D3D12_RESOURCE_BARRIER d3dResourceBarrier; 

	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER)); 
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION; 
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = m_ppd3dRenderTargetBuffers[m_nSwapChainBufferIndex]; 
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; 
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES; 
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(); 
	
	d3dRtvCPUDescriptorHandle.ptr += (m_nSwapChainBufferIndex * m_nRtvDescriptorIncrementSize);

	float pfClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; 
	m_pd3dCommandList->ClearRenderTargetView(d3dRtvCPUDescriptorHandle, pfClearColor/*Colors::Azure*/, 0, NULL);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(); 
	
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
	m_pd3dCommandList->OMSetRenderTargets(1, &d3dRtvCPUDescriptorHandle, TRUE, &d3dDsvCPUDescriptorHandle);
	
	if (m_pScene) 
		m_pScene->Render(m_pd3dCommandList, m_pCamera);

	//3��Ī ī�޶��� �� �÷��̾ �׻� ���̵��� �������Ѵ�.
#ifdef _WITH_PLAYER_TOP 
	//���� Ÿ���� �״�� �ΰ� ���� ���۸� 1.0���� ����� �÷��̾ �������ϸ� �÷��̾�� ������ �׷��� ���̴�. 
	m_pd3dCommandList->ClearDepthStencilView(d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
#endif 
	//3��Ī ī�޶��� �� �÷��̾ �������Ѵ�. 
	if (m_pPlayer) m_pPlayer->Render(m_pd3dCommandList, m_pCamera);

	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; 
	d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; 
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES; 
	
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	hResult = m_pd3dCommandList->Close();

	ID3D12CommandList *ppd3dCommandLists[] = { m_pd3dCommandList }; 
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGpuComplete();

	m_pdxgiSwapChain->Present(0, 0);

	MoveToNextFrame();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37); ::SetWindowText(m_hWnd, m_pszFrameRate);

}


/* CPU & GPU ����ȭ ���� �Լ� */

/*        GPU�� ���ҽ��� �� ����� �� ���� CPU�� ��ٸ��� �ϴ� �Լ�.        */
void CGameFramework::WaitForGpuComplete()
{
	UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue); 
	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent); 
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}

}


/* �������� �޽��� ( Ű���� ���콺 �Է� ) ó�� ���� �Լ� */

/*         ���콺 �Է��� ó���ϴ� �Լ�        */
void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN: 
		//���콺 ĸ�ĸ� �ϰ� ���� ���콺 ��ġ�� �����´�. 
		m_pSelectedObject = m_pScene->PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pCamera);
		::SetCapture(hWnd); 
		::GetCursorPos(&m_ptOldCursorPos); 
		break; 
	
	case WM_LBUTTONUP: 
	case WM_RBUTTONUP: 
		//���콺 ĸ�ĸ� �����Ѵ�. 
		::ReleaseCapture(); 
		break;

	}

}

/*         Ű���� �Է��� ó���ϴ� �Լ�         */
void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;

		case VK_RETURN:
			break;

		case VK_F8:
			break;

		case VK_F9:
		{
			BOOL bFullScreenState = FALSE;
			
			m_pdxgiSwapChain->GetFullscreenState(&bFullScreenState, NULL);

			if (!bFullScreenState)
			{
				DXGI_MODE_DESC dxgiTargetParameters;

				dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				dxgiTargetParameters.Width	= m_nWndClientWidth;
				dxgiTargetParameters.Height = m_nWndClientHeight;
				dxgiTargetParameters.RefreshRate.Numerator = 60;
				dxgiTargetParameters.RefreshRate.Denominator = 1;
				dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				m_pdxgiSwapChain->ResizeTarget(&dxgiTargetParameters);
			}
			m_pdxgiSwapChain->SetFullscreenState(!bFullScreenState, NULL);

			OnResizeBackBuffers();
			
			break;
		}
		/*
		��F1�� Ű�� ������ 1��Ī ī�޶�, 
		��F2�� Ű�� ������ �����̽�-�� ī�޶�� �����Ѵ�,
		��F3�� Ű�� ������ 3��Ī ī�޶� �� �����Ѵ�.
		*/   
		case VK_F1:    
		case VK_F2:    
		case VK_F3:
			m_pCamera = m_pPlayer->ChangeCamera((DWORD)(wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
}

void CGameFramework::OnResizeBackBuffers()
{
	WaitForGpuComplete();	// ����۴� ������ ������ , Ǯ��ũ������ �Ǿ��ֱ� ������ GPU�� ��� �׸��� �� �׸� �� ���� ��ٸ���.

	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);	// CommandList�� ���� ���� �Ұž�
	
	/* Ȥ�� ����Ÿ�� ���۰� �����ִٸ� �޸𸮸� �����Ѵ�. */
	for (int i = 0; i < m_nSwapChainBuffers; i++)
		if (m_ppd3dRenderTargetBuffers[i])
			m_ppd3dRenderTargetBuffers[i]->Release(); 
	
	if (m_pd3dDepthStencilBuffer)
		m_pd3dDepthStencilBuffer->Release();

#ifdef _WITH_ONLY_RESIZE_BACKBUFFERS
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	m_nSwapChainBufferIndex = 0;
#else
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(m_nSwapChainBuffers, m_nWndClientWidth,
		m_nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);
#endif

	CreateRenderTargetView();
	CreateDepthStencilView();

	m_pd3dCommandList->Close();

	/* ���⼭ ���� Ŀ�ǵ� ���� ����Ʈȭ ��Ų��. */
	ID3D12CommandList* ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	/* ȭ�� ��ȯ�� �̷������ �ٽ� �׸��°� ��ٸ���. */
	WaitForGpuComplete();
}


/*  */

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	/* �����찡 ���� �� �� , ������ ũ�Ⱑ ����� �� ȣ��ȴ�. */
	case WM_SIZE:
		m_nWndClientWidth  = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);
	
		OnResizeBackBuffers();
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}

	return(0);

}

/*		�����ϱ� 6��		 */

void CGameFramework::MoveToNextFrame()
{
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();
	UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);
	if (m_pd3dFence->GetCompletedValue() < nFenceValue) 
	{ 
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent); 
		::WaitForSingleObject(m_hFenceEvent, INFINITE); 
	}


}

void CGameFramework::ProcessSelectedObject(DWORD dwDirection, float cxDelta, float cyDelta) { 
	//��ŷ���� ������ ���� ��ü�� ������ Ű���带 �����ų� ���콺�� �����̸� ���� ��ü�� �̵� �Ǵ� ȸ���Ѵ�. 
	if (dwDirection != 0)
	{
		if (dwDirection & DIR_FORWARD)
			m_pSelectedObject->MoveForward(+1.0f);
		if (dwDirection & DIR_BACKWARD) 
			m_pSelectedObject->MoveForward(-1.0f);
		if (dwDirection & DIR_LEFT)
			m_pSelectedObject->MoveStrafe(+1.0f);
		if (dwDirection & DIR_RIGHT)
			m_pSelectedObject->MoveStrafe(-1.0f);
		if (dwDirection & DIR_UP)
			m_pSelectedObject->MoveUp(+1.0f);
		if (dwDirection & DIR_DOWN)
			m_pSelectedObject->MoveUp(-1.0f); 
	} 
	else if ((cxDelta != 0.0f) || (cyDelta != 0.0f)) 
	{ 
		m_pSelectedObject->Rotate(cyDelta, cxDelta, 0.0f); 
	} 
}