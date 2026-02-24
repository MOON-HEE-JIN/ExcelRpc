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
	std::map<int, std::function<int(CPlayer*, CPacket&)>> m_mapGameProc;
	std::map<int, std::function<int(CSession*, CPacket&)>> m_mapLoginProc;
	void InitRegisterFuncPointer();
private:
	virtual int DO_ERROR_PACKET(void* pTarget, CPacket& pReqPacket) = 0;
	virtual int DO_ERROR_RESULT(void* pTarget, int ret, int type) = 0;

};