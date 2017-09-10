#include "stdafx.h"
#include "Timer.h"


CGameTimer::CGameTimer()
{
	/* QueryPerformanceFrequency(LARGE_INTERGER* lpFrequency */
	/* lpFrequency : ���� �ʴ� Performance Counter �󵵸� �޴´�. */
	/* Performance Counter : ���ļ�, �ý��� ���ý� ���� �Ǿ� ������ ��� ���α׷��� �ϰ��ϴ�. */
	/* ���� �󵵴� ���� ���α׷� �ʱ�ȭ �ÿ��� �����ؾ� ����� ���� �� �ִ�. */
	if (::QueryPerformanceFrequency((LARGE_INTEGER* )&m_nPerformanceFrequency))
	{
		m_bHardwareHasPerformanceCounter = TRUE;
		::QueryPerformanceCounter((LARGE_INTEGER* )&m_nLastTime);
		m_fTimeScale = 1.0f / m_nPerformanceFrequency;
	}

	else
	{
		m_bHardwareHasPerformanceCounter = FALSE;
		m_nLastTime		= ::timeGetTime(); // �����찡 �����ϰ� ������ ����� �ð��� �޴´�.
		m_fTimeScale	= 0.001f;
	}

	m_nSampleCount		= 0;
	m_nCurrentFrameRate = 0;
	m_nFramesPerSecond	= 0;
	m_fFPSTimeElapsed	= 0.0f;

}


CGameTimer::~CGameTimer()
{
}


/*           Tick           */

/* ��ǻ�Ͱ� ���� ī���� �ϵ��� ������ ������ ī���Ϳ� ���ļ��� �̿��Ͽ� ���� �ð��� ����, */
/* ������ ��Ƽ�̵�� Ÿ�̸Ӹ� �̿��Ѵ�. */

void CGameTimer::Tick(float fLockFPS)
{

	if (m_bHardwareHasPerformanceCounter)
		::QueryPerformanceCounter((LARGE_INTEGER* )&m_nCurrentTime);	// ī���Ϳ� ���ļ��� �̿��Ͽ� ���� �ð��� ����

	else
		m_nCurrentTime = ::timeGetTime();

	/* ���������� �� �Լ��� ȣ���� ���� ����� �ð��� ����Ѵ�. */
	/* ���� �ð� - ������ �������� �ð� * ? */
	float fTimeElapsed = ( m_nCurrentTime - m_nLastTime ) * m_fTimeScale;

	if (fLockFPS > 0.0f)
	{
		/* fLockFPS�� 0���� ũ�� �� �ð���ŭ ȣ���� �Լ��� ��ٸ��� �Ұž� : FPS�� ���߱� ���ؼ� */
		while (fTimeElapsed < (1.0f / fLockFPS))
		{
			if (m_bHardwareHasPerformanceCounter)
				::QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);

			else
				m_nCurrentTime = ::timeGetTime();

			fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;
		}

	}

	/* ������ �������� ������ ����Ʈ ó���� ���� �ð��� ������ �Ѱž�. */
	m_nLastTime = m_nCurrentTime;

	if (fabsf(fTimeElapsed - m_fTimeElapsed) < 1.0f)
	{
		/* memmove( void* dest, const void* src, size_t n*/
		/* dest : ������ �޸� ���� */
		/* src  : ������ �޸� ���� */
		/* n    : ������ �������� ����Ʈ �� */
		
		::memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float)); // 0 ~ 49 -> 1 ~ 50
		m_fFrameTime[0] = fTimeElapsed;	// 0�� ����

		if (m_nSampleCount < MAX_SAMPLE_COUNT) 
			m_nSampleCount++;	// ������ �������� ���� �ִ� ������ ���� ������ ++
	}

	/* �ʴ� ������ ���� 1���� ��Ű�� ���� ������ ó�� �ð��� �����Ͽ� �����Ѵ�. */

	m_nFramesPerSecond++;
	m_fFPSTimeElapsed += fTimeElapsed;
	
	if (m_fFPSTimeElapsed > 1.0f)	// 1�ʰ� ���� ���
	{
		m_nCurrentFrameRate = m_nFramesPerSecond;
		m_nFramesPerSecond	= 0;
		m_fFPSTimeElapsed	= 0.0f;
	}

	/* ������ ������ ó�� �ð��� ����� ���Ͽ� ������ ó�� �ð��� ���Ѵ�. */
	
	m_fTimeElapsed = 0.0f;

	for (ULONG i = 0; i < m_nSampleCount; i++)
		m_fTimeElapsed += m_fFrameTime[i];

	if (m_nSampleCount > 0) 
		m_fTimeElapsed /= m_nSampleCount;

}


/*           GetFrameRate           */
/* ������ ������ ����Ʈ�� ���ڿ��� ���������� ��ȯ */

unsigned long CGameTimer::GetFrameRate(LPTSTR lpszString, int nCharacters)
{

	if (lpszString)
	{
		/* _itow_s(int value, wchar_t* buffer, size_t sizeInCharacters, int radix */
		/* value  : ��ȯ�� ���� */
		/* buffer : ��ȯ�� ����� ä���� �� */
		/* sizeInCharacters : ���� ������ ũ�� */
		/* radix : �⺻ value, 2 ~ 36���� */
		/* �������� ���������� ��ȯ ���ִ� �Լ� */
		_itow_s(m_nCurrentFrameRate, lpszString, nCharacters, 10);

		/* wcscat_s(char* strDestination, size_t numberOfElements, const wchar_t* strSource) */
		/* strDestination : null�� ������ ���ڿ� ���� */
		/* numberOfElements : ��� ���ڿ� ������ ũ�� */
		/* strSource : null ���� �ҽ� ���ڿ� ���� */
		/* ���ڿ��� �� �ϴ� �Լ� */
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return(m_nCurrentFrameRate);

}

float CGameTimer::GetTimeElapsed()
{

	return(m_fTimeElapsed);

}

void CGameTimer::Reset()
{
	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER* )&nPerformanceCounter);

	/* ���� , ������ �ð��� ���� �ð����� �ʱ�ȭ */
	m_nBaseTime = nPerformanceCounter;
	m_nLastTime = nPerformanceCounter;
	m_nStopTime = 0;
	m_bStopped  = false;

}

void CGameTimer::Start()
{

	__int64 nPerformanceCounter;
	::QueryPerformanceCounter((LARGE_INTEGER *)&nPerformanceCounter);

	if (m_bStopped)
	{
		m_nPausedTime	+= (nPerformanceCounter - m_nStopTime);
		m_nLastTime		= nPerformanceCounter;
		m_nStopTime		= 0;
		m_bStopped		= false;
	}

}

void CGameTimer::Stop()
{

	if (!m_bStopped)
	{
		::QueryPerformanceCounter((LARGE_INTEGER *)&m_nStopTime);
		m_bStopped = true;
	}

}

/* ������ ����� �� �ð��� ��� �Լ� */

float CGameTimer::GetTotalTime()
{
	
	if (m_bStopped) 
		return(float(((m_nStopTime - m_nPausedTime) - m_nBaseTime) * m_fTimeScale));
	
	return (float(((m_nCurrentTime - m_nPausedTime) - m_nBaseTime) * m_fTimeScale));

}

