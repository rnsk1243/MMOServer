#include "AreaManager.h"
#include"ErrorHandler.h"
#include<thread>


//void PatrolErrorLinkWorkThread(LPVOID areasPtr, LPVOID hREv, LPVOID hBEv)
//{
//	printf("ErrorLink제거 스레드 스타트");
//	AreaVector* areas = (AreaVector*)areasPtr;
//	HANDLE hRemoveEvent = *((HANDLE*)hREv);
//	HANDLE hBroadcastEvent = *((HANDLE*)hBEv);
//	while (true)
//	{
//		if (ErrorHandlerPtr->IsErrorLinkRemoveStart())
//		{
//			printf("가비지 작동!\n");
//			//printf("//1111//\n");
//			ResetEvent(hRemoveEvent);
//			WaitForSingleObject(hBroadcastEvent, INFINITE);
//			//printf("//1111통과//\n");
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
	//hRemoveEvent = CreateEvent(NULL, TRUE, TRUE, NULL);	// 매뉴얼 리셋, 신호 상태
	//hBroadcastEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // 오토 리셋, 비신호 상태

	//std::thread errorLinkRemoveWork(PatrolErrorLinkWorkThread, &mAreas, &hRemoveEvent, & hBroadcastEvent);
	//errorLinkRemoveWork.detach();
	//CloseHandle(errorLinkRemoveWork.native_handle());
}


CAreaManager::~CAreaManager()
{
	//CloseHandle(hRemoveEvent);
	//CloseHandle(hBroadcastEvent);
}

bool CAreaManager::EnterArea(int areaNumber, const LinkPtr & linkPtr)
{
	return mAreas[areaNumber].get()->PushClient(linkPtr);
}

bool CAreaManager::MoveArea(int areaNumber, const LinkPtr & linkPtr)
{
	if (linkPtr == nullptr)
		return false;
	int oldAreaNumber = linkPtr.get()->GetCurArea();
	mAreas[oldAreaNumber].get()->SendDeleteCommand(linkPtr); // 클라이언트들에게 지우라 명령
	ErrorHandlerPtr->PushDeletePlayerDisCode(oldAreaNumber, linkPtr.get()->GetMyDistinguishCode()); // 서버 컨테이너에 들은거 지우기 등록
	bool result = EnterArea(areaNumber, linkPtr);
	return result;
}

const LinkPtr CAreaManager::GetLinkPtr(int areaNumber, int disCode)
{
	return mAreas[areaNumber].get()->GetLinkPtr(disCode);
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
//	printf("//2222통과//\n");
	int myArea = link.get()->GetCurArea();
	mAreas[myArea].get()->Broadcast(PacketKind, packet);

//	SetEvent(hBroadcastEvent);
}
