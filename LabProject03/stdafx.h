// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
/* stdafx.h에 header를 추가하는 이유 */
/* PCH( PreCompiled Header ) 를 통하여 자주 변경하지 않는  */
/* 헤더 파일에 대한 컴파일을 매번하지 않아도 되는 기능을 제공하기 때문 */
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
/* 1. 라이브러리 헤더 포함하기 */

/* C++ */
#include <string>				// string을 사용하기 위한 Header 
#include <wrl.h>				// COM 객체를 손쉽게 생성하고 사용하기 위한 Header

#include <shellapi.h>			// ? 


#include <d3d12.h>				// Direct3D 12 api 함수를 사용하기 위해 반드시 넣어야 한다.
#include <dxgi1_4.h>			// DXGI 1.4버전을 이용하기 위한 Header
#include <D3Dcompiler.h>		// 

/* DirectXMath 함수 */
#include <DirectXMath.h>		// DirectXMath를 사용하기 위한 Header
#include <DirectXPackedVector.h>// 
#include <DirectXColors.h>		// 
#include <DirectXCollision.h>	// 

#include <Mmsystem.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using Microsoft::WRL::ComPtr;

/* Import Library 추가 */
#pragma comment( lib , "d3dCompiler.lib" )
#pragma comment( lib , "d3d12.lib" )
#pragma comment( lib , "dxgi.lib" )
#pragma comment( lib , "winmm.lib" )

/* 주 윈도우 클라이언트 파일의 크기가 640 * 480이도록 수정한다. */
#define FRAME_BUFFER_WIDTH	640
#define FRAME_BUFFER_HEIGHT	480