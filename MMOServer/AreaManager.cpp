#include "AreaManager.h"
#include"ErrorHandler.h"
#include<thread>


//void PatrolErrorLinkWorkThread(LPVOID areasPtr, LPVOID hREv, LPVOID hBEv)
//{
//	printf("ErrorLink���� ������ ��ŸƮ");
//	AreaVector* areas = (AreaVector*)areasPtr;
//	HANDLE hRemoveEvent = *((HANDLE*)hREv);
//	HANDLE hBroadcastEvent = *((HANDLE*)hBEv);
//	while (true)
//	{
//		if (ErrorHandlerPtr->IsErrorLinkRemoveStart())
//		{
//			printf("������ �۵�!\n");
//			//printf("//1111//\n");
//			ResetEvent(hRemoveEvent);
//			WaitForSingleObject(hBroadcastEvent, INFINITE);
//			//printf("//1111���//\n");
//
//			AreaVectorIt areaiterBegin = areas->begin();
//			for (; areaiterBegin != areas->end(); ++areaiterBegin)
//			{
//				(*areaiterBegin).get()->SearchEndRemoveErrorLink();
//			}
//			SetEvent(hRemoveEvent);
//		}
//		//Sleep(500);
//	}
//}

CAreaManager::CAreaManager(int areaAmount)
{
	mAreas.reserve(areaAmount);
	for (int i = 0; i < areaAmount; ++i)
	{
		AreaPtr newArea(new CArea("test", i));
		mAreas.push_back(newArea);
	}
	//hRemoveEvent = CreateEvent(NULL, TRUE, TRUE, NULL);	// �Ŵ��� ����, ��ȣ ����
	//hBroadcastEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // ���� ����, ���ȣ ����

	//std::thread errorLinkRemoveWork(PatrolErrorLinkWorkThread, &mAreas, &hRemoveEvent, & hBroadcastEvent);
	//errorLinkRemoveWork.detach();
	//CloseHandle(errorLinkRemoveWork.native_handle());
}


CAreaManager::~CAreaManager()
{
	//CloseHandle(hRemoveEvent);
	//CloseHandle(hBroadcastEvent);
}

void CAreaManager::EnterArea(int areaNumber, LinkPtr linkPtr)
{
	mAreas[areaNumber].get()->PushClient(linkPtr);
}

//void CAreaManager::EraseClient(LinkPtr linkPtr)
//{
//	int myArea = linkPtr.get()->GetCurArea();
//	mAreas[myArea].get()->EraseClient(linkPtr.get()->GetMyDistinguishCode());
//}

void CAreaManager::Broadcast(LinkPtr link, const PacketKindEnum PacketKind, LPVOID packet)
{
//	printf("//2222//\n");
	//WaitForSingleObject(hRemoveEvent, INFINITE);
	//ResetEvent(hBroadcastEvent);
//	printf("//2222���//\n");
	int myArea = link.get()->GetCurArea();
	mAreas[myArea].get()->Broadcast(PacketKind, packet);

//	SetEvent(hBroadcastEvent);
}
