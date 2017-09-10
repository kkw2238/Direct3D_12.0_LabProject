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