#pragma once

const ULONG MAX_SAMPLE_COUNT = 50; // 50ȸ�� ������ ó���ð��� �����Ͽ� ����Ѵ�.

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

	void Tick(float fLockFPS = 0.0f);					// Ÿ�̸��� �ð��� �����Ѵ�.
	void Start();
	void Stop();
	void Reset();

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	
	float GetTimeElapsed();							// �������� ��� ��� �ð��� ��ȯ�Ѵ�.
	float GetTotalTime();								// ������ ����Ʈ�� ��ȯ�Ѵ�.

private:

	/* Performance Counter */
	/* ī���� �����ʹ� �ý��� ���� ������ �ľ��ϰ� �ý��� �� �������α׷� ������ ���������� ���� */
	/* � ü��, ��Ʈ��ũ �� ��ġ�� ���� ���α׷����� �Һ� �� �� �ִ� ī���� �����͸� ���� */
	/* �ý����� ������ �׷������� �����ش�. */

	bool			m_bHardwareHasPerformanceCounter;	// ��ǻ�Ͱ� Performance Counter�� ���� �ִ°�?
	bool			m_bStopped;

	float			m_fTimeScale;						// Scale Counter�� ��
	float			m_fTimeElapsed;						// ������ ������ ���� ������ �ð�
	float			m_fFPSTimeElapsed;					// ������ ����Ʈ ��� �ҿ� �ð�
	float			m_fFrameTime[MAX_SAMPLE_COUNT];		// ������ �ð��� �����ϱ� ���� �迭

	__int64			m_nCurrentTime;						// ������ �ð�
	__int64			m_nLastTime;						// ������ �������� �ð�
	__int64			m_nBaseTime;						
	__int64			m_nPausedTime;						
	__int64			m_nStopTime;				
	__int64			m_nPerformanceFrequency;			// ��ǻ���� Performance ��

	ULONG			m_nSampleCount;						// ������ �������� Ƚ��

	unsigned long	m_nCurrentFrameRate;				// ������ ������ ����Ʈ
	unsigned long	m_nFramesPerSecond;					// �ʴ� ������ ��
};

