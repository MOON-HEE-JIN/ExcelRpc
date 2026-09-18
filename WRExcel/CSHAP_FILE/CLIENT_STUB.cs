using BaseStub;
namespace BaseStub
{
	public abstract class Stub<TObject>
	{
		public Stub(){InitRegisterFuncDictionary();}
		public delegate int CallBack(TObject obj, Span<byte> buffer);
		public readonly Dictionary<int, CallBack> m_DicGAMEProc = new();
		public readonly Dictionary<int, CallBack> m_DicOBSERVERProc = new();
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
		public int DO_GAME_Proc(int type, TObject pTarget, Span<byte> cPacket)
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
		public int DO_OBSERVER_Proc(int type, TObject pTarget, Span<byte> cPacket)
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
	protected abstract int DO_GAME_AOI_IN_PLAYER(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_AOI_IN_PLAYERS(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_AOI_IN_PLAYERS_MOVE(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_AOI_OUT_PLAYER(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_AOI_OUT_PLAYERS(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_CHANGEINGZONE(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_CHANGEZONE(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_CONNECTINFO(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_LOOPBACK(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_MOVESTART(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_MOVESTOP(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_OTHERMOVESTART(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_GAME_TELEPORT(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_OBSERVER_CONNET_OBSERVER(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_ERROR_PACKET(TObject pTarget, Span<byte> pReqPacket);
	protected abstract int DO_ERROR_RESULT(TObject pTarget, int ret, int type);
	}
}