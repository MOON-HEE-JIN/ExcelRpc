using System.Buffers.Binary;
using System.Text;
using CSHAP_SERVER.Models;
namespace CSHAP_SERVER.Models
{
	public abstract class Stub<Object>
	{
		public readonly Dictionary<int, Func<Object, byte[], int>> m_DicGAMEProc = new();
		public void InitRegisterFuncDictionary()
		{
			m_DicGAMEProc[(int)GAME.LOOPBACK] = DO_GAME_LOOPBACK;
			m_DicGAMEProc[(int)GAME.CHANGEPID] = DO_GAME_CHANGEPID;
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
		public abstract int DO_GAME_CHANGEPID(Object pTarget, byte[] pReqPacket);
		public abstract int DO_GAME_LOOPBACK(Object pTarget, byte[] pReqPacket);
		public abstract int DO_ERROR_PACKET(Object pTarget, byte[] pReqPacket);
		public abstract int DO_ERROR_RESULT(Object pTarget, int ret, int type);
	}
}