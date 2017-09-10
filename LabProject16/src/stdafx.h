// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
/* stdafx.h�� header�� �߰��ϴ� ���� */
/* PCH( PreCompiled Header ) �� ���Ͽ� ���� �������� �ʴ�  */
/* ��� ���Ͽ� ���� �������� �Ź����� �ʾƵ� �Ǵ� ����� �����ϱ� ���� */
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
/* 1. ���̺귯�� ��� �����ϱ� */

/* C++ */
#include <string>				// string�� ����ϱ� ���� Header 
#include <wrl.h>				// COM ��ü�� �ս��� �����ϰ� ����ϱ� ���� Header
#include <iostream>
#include <shellapi.h>			// ? 


#include <d3d12.h>				// Direct3D 12 api �Լ��� ����ϱ� ���� �ݵ�� �־�� �Ѵ�.
#include <dxgi1_4.h>			// DXGI 1.4������ �̿��ϱ� ���� Header
#include <D3Dcompiler.h>		// 

/* DirectXMath �Լ� */
#include <DirectXMath.h>		// DirectXMath�� ����ϱ� ���� Header
#include <DirectXPackedVector.h>// 
#include <DirectXColors.h>		// 
#include <DirectXCollision.h>	// 

#include <Mmsystem.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;

/* Import Library �߰� */
#pragma comment( lib , "d3dCompiler.lib" )
#pragma comment( lib , "d3d12.lib" )
#pragma comment( lib , "dxgi.lib" )
#pragma comment( lib , "winmm.lib" )

/* �� ������ Ŭ���̾�Ʈ ������ ũ�Ⱑ 640 * 480�̵��� �����Ѵ�. */
#define FRAME_BUFFER_WIDTH	640
#define FRAME_BUFFER_HEIGHT	480

/*������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.*/ 
#define RANDOM_COLOR XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))



				/* ���� ���ҽ��� �����ϴ� �Լ��̴�. */

/* �츮�� ���ε�� ��� ���ҽ��� �÷� ���� ��, �׷��� ī�忡���� �̿��ϰ� �� �� �̴�. */
extern ID3D12Resource* CreateBufferResource(ID3D12Device* pd3dDevice,
	ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nBytes,
	D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD,
	D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
	ID3D12Resource** ppd3dUploadBuffer = NULL
);

//3���� ������ ���� 
namespace Vector3 { 

	inline bool IsZero(XMFLOAT3& xmf3Vector) {
		if (xmf3Vector.x == 0.0f && xmf3Vector.y == 0.0f && xmf3Vector.z == 0.0f)
			return(true);

		return(false);
	}

	/* Vector�� XMFloat3���� �ٲ��ִ� �Լ� */
	inline XMFLOAT3 XMVectorToFloat3(XMVECTOR& xmvVector) { 

		XMFLOAT3 xmf3Result; 

		XMStoreFloat3(&xmf3Result, xmvVector); 
		return(xmf3Result); 

	}

	/* ���Ϳ� ��Į�� �������� ���ش�. */
	inline XMFLOAT3 ScalarProduct(XMFLOAT3& xmf3Vector, float fScalar, bool bNormalize = true) { 
		
		XMFLOAT3 xmf3Result; 
	
		if (bNormalize) 
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)) * fScalar); 
		else 
			XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector) * fScalar);
	
		return(xmf3Result); 
	
	}
	
	/* ������ �տ��� */
	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		
		XMFLOAT3 xmf3Result; 
		
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + XMLoadFloat3(&xmf3Vector2));
	
		return(xmf3Result);
	
	}
	
	inline XMFLOAT3 Add(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, float fScalar) { 
		
		XMFLOAT3 xmf3Result; 
		
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) + (XMLoadFloat3(&xmf3Vector2) * fScalar)); 
		
		return(xmf3Result); 
	
	}
	
	inline XMFLOAT3 Subtract(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2) { 
		
		XMFLOAT3 xmf3Result; 
		
		XMStoreFloat3(&xmf3Result, XMLoadFloat3(&xmf3Vector1) - XMLoadFloat3(&xmf3Vector2));
		
		return(xmf3Result); 
	
	}
	
	/* ������ ���� ���� */
	inline float DotProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2) {
		
		XMFLOAT3 xmf3Result; 
		
		XMStoreFloat3(&xmf3Result, XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))); return(xmf3Result.x); 
	
	}
	
	/* ������ ���� ���� */
	inline XMFLOAT3 CrossProduct(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2, bool bNormalize = true) { 
		
		XMFLOAT3 xmf3Result; 
		
		if (bNormalize) 
			XMStoreFloat3(&xmf3Result, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)))); else XMStoreFloat3(&xmf3Result, XMVector3Cross(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))); 
		
		return(xmf3Result); 
	
	}
	
	/* ���͸� �븻������ �ؼ� XMFLOAT3���� ��ȯ */
	inline XMFLOAT3 Normalize(XMFLOAT3& xmf3Vector) { 
	
		XMFLOAT3 m_xmf3Normal;
	
		XMStoreFloat3(&m_xmf3Normal, XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
		
		return(m_xmf3Normal);
	
	}
	
	/* ������ ���̸� ���ϴ� �Լ� */
	inline float Length(XMFLOAT3& xmf3Vector) { 
		
		XMFLOAT3 xmf3Result;
		
		XMStoreFloat3(&xmf3Result, XMVector3Length(XMLoadFloat3(&xmf3Vector))); 
		
		return(xmf3Result.x); 
	
	}
	
	/* �� Vector ������ ���� ���ϴ� �Լ� */
	inline float Angle(XMVECTOR& xmvVector1, XMVECTOR& xmvVector2)
	{
		XMVECTOR xmvAngle = XMVector3AngleBetweenNormals(xmvVector1, xmvVector2); return(XMConvertToDegrees(acosf(XMVectorGetX(xmvAngle))));
	}
	
	/* �� XMFLOAT3 ������ Vector������ ���� �����ִ� �Լ�*/
	inline float Angle(XMFLOAT3& xmf3Vector1, XMFLOAT3& xmf3Vector2) { 
		
		return(Angle(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))); 
	
	}
	
	/* TransformNormal�� ��� w�� 0���� ������ش� = ���� ��ȯ�� �ַ� ���δ�.*/
	/* ������ ��Ʈ������ ���� ������ 3���� �Ϲ� ���͸� ��ȯ�Ѵ�. */
	inline XMFLOAT3 TransformNormal(XMFLOAT3& xmf3Vector, XMMATRIX& xmmtxTransform) { 
		
		XMFLOAT3 xmf3Result; 
		
		XMStoreFloat3(&xmf3Result, XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmmtxTransform)); 
		
		return(xmf3Result); 
	
	}
	
	/* TransformCoord�� ��� w�� 1�� ������ش� = ����Ʈ(��ǥ) ��ȯ�� �ַ� ���δ�.*/
	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMMATRIX& xmmtxTransform) { 
		
		XMFLOAT3 xmf3Result; 
		
		XMStoreFloat3(&xmf3Result, XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmmtxTransform)); 
		
		return(xmf3Result); 
	
	}
	inline XMFLOAT3 TransformCoord(XMFLOAT3& xmf3Vector, XMFLOAT4X4& xmmtx4x4Matrix) { 
		
		return(TransformCoord(xmf3Vector, XMLoadFloat4x4(&xmmtx4x4Matrix))); 
	
	}
}
//4���� ������ ���� 
namespace Vector4 { 

	inline XMFLOAT4 Add(XMFLOAT4& xmf4Vector1, XMFLOAT4& xmf4Vector2) { 

		XMFLOAT4 xmf4Result; 

		XMStoreFloat4(&xmf4Result, XMLoadFloat4(&xmf4Vector1) + XMLoadFloat4(&xmf4Vector2)); 
		
		return(xmf4Result); 
	} 

	inline XMFLOAT4 Multiply(float fScalar, XMFLOAT4& xmf4Vector) { 

		XMFLOAT4 xmf4Result; XMStoreFloat4(&xmf4Result, fScalar * XMLoadFloat4(&xmf4Vector)); 

		return(xmf4Result);

	}

}
//����� ���� 
namespace Matrix4x4 { 

	/* ���� ����� ��ȯ�Ѵ�. */
	inline XMFLOAT4X4 Identity() { 
		
		XMFLOAT4X4 xmmtx4x4Result;
		
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixIdentity()); 
		
		return(xmmtx4x4Result); 

	}

	/* �� ����� ���ϰ� ����� ��ȯ�Ѵ�. */
	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMFLOAT4X4& xmmtx4x4Matrix2)
	{
		XMFLOAT4X4 xmmtx4x4Result; 
		
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * XMLoadFloat4x4(&xmmtx4x4Matrix2)); 
		
		return(xmmtx4x4Result);
	}

	inline XMFLOAT4X4 Multiply(XMFLOAT4X4& xmmtx4x4Matrix1, XMMATRIX& xmmtxMatrix2) { 
		
		XMFLOAT4X4 xmmtx4x4Result;
		
		XMStoreFloat4x4(&xmmtx4x4Result, XMLoadFloat4x4(&xmmtx4x4Matrix1) * xmmtxMatrix2); 
		
		return(xmmtx4x4Result); 
	
	}

	inline XMFLOAT4X4 Multiply(XMMATRIX& xmmtxMatrix1, XMFLOAT4X4& xmmtx4x4Matrix2) { 
		
		XMFLOAT4X4 xmmtx4x4Result;
		
		XMStoreFloat4x4(&xmmtx4x4Result, xmmtxMatrix1 * XMLoadFloat4x4(&xmmtx4x4Matrix2)); 
		
		return(xmmtx4x4Result); 
	
	}

	/* ������� �����ִ� �Լ� */
	inline XMFLOAT4X4 Inverse(XMFLOAT4X4& xmmtx4x4Matrix) { 
		
		XMFLOAT4X4 xmmtx4x4Result;
		
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixInverse(NULL, XMLoadFloat4x4(&xmmtx4x4Matrix)));
		
		return(xmmtx4x4Result);
	
	}

	/* ��Ŀ� ���� ��ġ ����� ��ȯ�Ѵ�. */
	inline XMFLOAT4X4 Transpose(XMFLOAT4X4& xmmtx4x4Matrix) { 
		
		XMFLOAT4X4 xmmtx4x4Result;
		
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixTranspose(XMLoadFloat4x4(&xmmtx4x4Matrix)));
		
		return(xmmtx4x4Result); 
	
	}

	/* ���� ����� ������ �ش�. */
	inline XMFLOAT4X4 PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ) { 
		
		XMFLOAT4X4 xmmtx4x4Result;

		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, NearZ, FarZ)); 
	
		return(xmmtx4x4Result); 
	
	}

	/* ī�޶� ��ȯ ����� ������ �ش�. */
	inline XMFLOAT4X4 LookAtLH(XMFLOAT3& xmf3EyePosition, XMFLOAT3& xmf3LookAtPosition, XMFLOAT3& xmf3UpDirection) { 
		
		XMFLOAT4X4 xmmtx4x4Result;
		
		XMStoreFloat4x4(&xmmtx4x4Result, XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
		
		return(xmmtx4x4Result); 

	}
}

