#pragma once

#include <stdlib.h>

namespace MEngineNS
{

	struct Telegram;

template <class T>
class Controller
{

public:

	Controller() {}
	~Controller() {}

	virtual void Update(double deltaT) = 0;

	virtual void HandleMessage(const Telegram& telegram) {}

	virtual T* GetEntity() { return m_pEntity; }

	virtual void SetOrder(int command) {}

	virtual void Unassign()
	{
		m_pEntity = NULL;
	}

	virtual void Assign(T* pEntity)
	{
		m_pEntity = pEntity;
	}

protected:

	T* m_pEntity;

};

}