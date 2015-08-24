#include "Player.h"
#include "Input.h"
#include "StateMachine.h"
#include "AIShipOrderStates.h"
#include "Data.h"

namespace SpaceSimNS
{

Player::Player(Input* pInput)
{
		m_pInput = pInput;

		m_pControlMap = ControlMap();
		m_pPlayerVessel = m_pEntity;
		m_pPlayerFleet->AddMember(this);

		m_pControlMap.emplace(0x57, ACCEL);
		m_pControlMap.emplace(0x53, BACK);
		m_pControlMap.emplace(0x51, LEFT);
		m_pControlMap.emplace(0x45, RIGHT);
		m_pControlMap.emplace(0x41, TURNCCW);
		m_pControlMap.emplace(0x44, TURNCW);
		m_pControlMap.emplace(VK_SPACE, FIRE);
		m_pControlMap.emplace(VK_TAB, TARGET);
		m_pControlMap.emplace(0x54, BESTTARGET);
		m_pControlMap.emplace(0x43, UNTARGET);

}


Player::~Player()
{
}

void Player::UpdateActions()
{
	for (std::pair<UINT, Action> control : m_pControlMap)
	{
		if (m_pInput->IsKeyDown(control.first))
		{
			m_pEntity->DoAction(control.second);
		}
		else
		{
			m_pEntity->StopAction(control.second);
		}
	}
}

void Player::HandleMessage(const Telegram& telegram)
{

}

void Player::Update(double deltaT)
{

}

void Player::SetVessel(Vessel* pVessel)
{

}

}