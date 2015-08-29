#pragma once

#include <set>


namespace MEngineNS
{

struct Telegram;

class MessageDispatcher
{
public:

	static MessageDispatcher* Instance();

	void DispatchTelegram(Telegram& message);

	void Update();

	void ClearMessages();

	void Initialize();

private:
	std::set<Telegram> m_messageQueue;

	void DischargeMessage(const Telegram& message);

	MessageDispatcher() {};
	~MessageDispatcher() {};
};

}