using System.Buffers.Binary;
using System.Text;
using CSHAP_SERVER.Models;
namespace CSHAP_SERVER.Models
{
	public abstract class Stub<Object>
	{
		public readonly Dictionary<int, Func<Object, byte[], int>> m_DicGAMEProc = new();
		public readonly Dictionary<int, Func<Object, byte[], int>> m_DicOBSERVERProc = new();
		public void InitRegisterFuncDictionary()
		{
			m_DicGAMEProc[(int)GAME.LOOPBACK] = DO_GAME_LOOPBACK;
			m_DicGAMEProc[(int)GAME.CONNECTINFO] = DO_GAME_CONNECTINFO;
			m_DicGAMEProc[(int)GAME.CHANGEZONE] = DO_GAME_CHANGEZONE;
			m_DicGAMEProc[(int)GAME.AOI_IN_PLAYER] = DO_GAME_AOI_IN_PLAYER;
			m_DicGAMEProc[(int)GAME.AOI_IN_PLAYERS] = DO_GAME_AOI_IN_PLAYERS;
			m_DicGAMEProc[(int)GAME.AOI_OUT_PLAYER] = DO_GAME_AOI_OUT_PLAYER;
			m_DicGAMEProc[(int)GAME.AOI_OUT_PLAYERS] = DO_GAME_AOI_OUT_PLAYERS;
			m_DicGAMEProc[(int)GAME.MOVESTART] = DO_GAME_MOVESTART;
			m_DicGAMEProc[(int)GAME.MOVESTOP] = DO_GAME_MOVESTOP;
			m_DicGAMEProc[(int)GAME.CHANGEINGZONE] = DO_GAME_CHANGEINGZONE;
			m_DicGAMEProc[(int)GAME.TELEPORT] = DO_GAME_TELEPORT;
			m_DicGAMEProc[(int)GAME.OTHERMOVESTART] = DO_GAME_OTHERMOVESTART;
			m_DicGAMEProc[(int)GAME.AOI_IN_PLAYERS_MOVE] = DO_GAME_AOI_IN_PLAYERS_MOVE;
			m_DicOBSERVERProc[(int)OBSERVER.CONNET_OBSERVER] = DO_OBSERVER_CONNET_OBSERVER;
		}
		public Stub(){InitRegisterFuncDictionary();}
		public virtual int DO_GAME_Proc(int type, Object pTarget, byte[] cPacket)
		{
			int ret = 0;
			if(!m_DicGAMEProc.ContainsKey(type))
			{
				ret = DO_ERROR_PACKET(pTarget, cPacket);
				return ret;
			}
			ret = m_DicGAMEProc[type](pTarget, cPacket);
			if(ret != 0)
				DO_ERROR_RESULT(pTarget, ret, type);
			return ret;
		}
		public virtual int DO_OBSERVER_Proc(int type, Object pTarget, byte[] cPacket)
		{
			int ret = 0;
			if(!m_DicOBSERVERProc.ContainsKey(type))
			{
				ret = DO_ERROR_PACKET(pTarget, cPacket);
				return ret;
			}
			ret = m_DicOBSERVERProc[type](pTarget, cPacket);
			if(ret != 0)
				DO_ERROR_RESULT(pTarget, ret, type);
			return ret;
		}
		public abstract int DO_GAME_AOI_IN_PLAYER(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_AOI_IN_PLAYERS(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_AOI_IN_PLAYERS_MOVE(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_AOI_OUT_PLAYER(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_AOI_OUT_PLAYERS(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_CHANGEINGZONE(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_CHANGEZONE(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_CONNECTINFO(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_LOOPBACK(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_MOVESTART(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_MOVESTOP(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_OTHERMOVESTART(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_TELEPORT(Object pTarget, byte[] pReqPacket);
		public abstract int DO_OBSERVER_CONNET_OBSERVER(Object pTarget, byte[] pReqPacket);
		public abstract int DO_ERROR_PACKET(Object pTarget, byte[] pReqPacket);
		public abstract int DO_ERROR_RESULT(Object pTarget, int ret, int type);
	}
}