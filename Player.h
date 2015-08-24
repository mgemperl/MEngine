#pragma once

class Input;

#include <set>
#include <map>
#include <string>
#include "Controller.h"
#include "Entity.h"


namespace SpaceSimNS
{

	typedef std::unordered_map<UINT, Action> ControlMap;

class Player : public Controller<Entity>
{
public:
	Player(Input* pInput);
	~Player();

	void UpdateActions();


	inline ClassType GetClass() const { return PLAYER; }

	void HandleMessage(const Telegram& telegram) override;

	void Update(double deltaT) override;

private:

	ControlMap m_pControlMap;
	Input* m_pInput;

};

}