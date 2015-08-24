#pragma once

class Input;

#include <set>
#include <map>
#include <string>
#include "Controller.h"
#include "Entity.h"


namespace MEngineNS
{

	typedef std::unordered_map<UINT, Action> ControlMap;

class Player : public Controller<Entity>
{
public:
	Player();
	~Player();

	void Initialize(Input* pInput) { m_pInput = pInput; }

	inline ClassType GetClass() const { return PLAYER; }

	void HandleMessage(const Telegram& telegram) override;

	void Update(double deltaT) override;

private:

	ControlMap m_pControlMap;
	Input* m_pInput;

};

}