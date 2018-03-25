#include "AreaManager.h"
#include"ErrorHandler.h"
#include<thread>


//void PatrolErrorLinkWorkThread(LPVOID areasPtr, LPVOID hREv, LPVOID hBEv)
//{
//	printf("ErrorLink�̏����̃X���b�h�X�^�[�g");
//	AreaVector* areas = (AreaVector*)areasPtr;
//	HANDLE hRemoveEvent = *((HANDLE*)hREv);
//	HANDLE hBroadcastEvent = *((HANDLE*)hBEv);
//	while (true)
//	{
//		if (ErrorHandlerPtr->IsErrorLinkRemoveStart())
//		{
//			 printf("�E�K�[�x�[�W�E�쓮!\n");
//			//printf("//1111//\n");
//			ResetEvent(hRemoveEvent);
//			WaitForSingleObject(hBroadcastEvent, INFINITE);
//			//printf("//1111�p�X//\n");
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
	//hRemoveEvent = CreateEvent(NULL, TRUE, TRUE, NULL);	//�}�j���A�����Z�b�g�A�M�����
	//hBroadcastEvent = CreateEvent(NULL, FALSE, FALSE, NULL); //�I�[�g���Z�b�g�A�r�V���z���

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
	mAreas[oldAreaNumber].get()->SendDeleteCommand(linkPtr); //�N���C�A���g�ɍ폜����
	ErrorHandlerPtr->PushDeletePlayerDisCode(oldAreaNumber, linkPtr.get()->GetMyDistinguishCode()); //�T�[�o�R���e�i�ɕ������̂��������Ɠo�^
	bool result = EnterArea(areaNumber, linkPtr);
	printf("[%d]�� client�� [%d]�� Area�ňړ����܂����B\n", linkPtr.get()->GetMyDistinguishCode(), areaNumber);
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
//	/ printf("//2222�ʉ�//\n");
	int myArea = link.get()->GetCurArea();
	mAreas[myArea].get()->Broadcast(PacketKind, packet);

//	SetEvent(hBroadcastEvent);
}
