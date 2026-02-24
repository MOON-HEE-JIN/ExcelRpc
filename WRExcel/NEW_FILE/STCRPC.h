#pragma once

#include "CPacket.h"
#include "CPlayer.h"
#include <map>
#include <functional>
class Stub
{
public:
	Stub(){InitRegisterFuncPointer();}
private:
	std::map<int, std::function<int(CPlayer*,CPacket&)> m_mapProc;
	virtual void DO_GAME_ERROR_Result(CPlayer* pTarget, CPacket& _value) = 0;
	virtual void DO_LOGIN_ERROR_Result(CPlayer* pTarget, CPacket& _value) = 0;

};