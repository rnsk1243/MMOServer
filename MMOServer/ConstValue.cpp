#include "ConstValue.h"

RequestCollection* RequestCollection::GetInstance()
{
	if (RequestCollectionPtr == nullptr)
	{
		RequestCollectionPtr = new RequestCollection();
	}
	return RequestCollectionPtr;
}
