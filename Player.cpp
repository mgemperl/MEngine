#include "Player.h"
#include "Input.h"
#include "StateMachine.h"
#include "Data.h"

namespace MEngineNS
{

Player::Player() : Controller<Entity>()
{
		m_pControlMap = ControlMap();

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

void Player::HandleMessage(const Telegram& telegram)
{

}

void Player::Update(double deltaT)
{

}

}