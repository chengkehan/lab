#include <Windows.h>

HANDLE thread1;
DWORD thread1ID;

HANDLE thread2;
DWORD thread2ID;

DWORD WINAPI threadTest1(LPVOID data)
{
	for (INT i = 0; i < 10; i++)
	{
		OutputDebugString(L"thread1\n");
	}

	return (DWORD)data;
}

DWORD WINAPI threadTest2(LPVOID data)
{
	for (INT i = 0; i < 10; i++)
	{
		OutputDebugString(L"thread2\n");
	}

	return (DWORD)data;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	thread1 = CreateThread(NULL, 0, threadTest1, NULL, CREATE_SUSPENDED, &thread1ID);
	ResumeThread(thread1);

	thread2 = CreateThread(NULL, 0, threadTest2, NULL, CREATE_SUSPENDED, &thread2ID);
	ResumeThread(thread2);

	for (INT i = 0; i < 5; i++)
	{
		OutputDebugString(L"main\n");
		Sleep(1000);
	}

	CloseHandle(thread1);
	CloseHandle(thread2);

	return 0;
}