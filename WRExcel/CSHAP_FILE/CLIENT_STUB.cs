using System.Buffers.Binary;
using System.Text;
using PacketEnums.Models;

namespace STC.Models
{
	public abstract class Stub<Object>
	{
		public readonly Dictionary<int, Func<Object, byte[], int>> m_DicCHATProc = new();
		public void InitRegisterFuncDictionary()
		{
			m_DicCHATProc[(int)CHAT.NORMAL] = DO_CHAT_NORMAL;
			m_DicCHATProc[(int)CHAT.WHISPER] = DO_CHAT_WHISPER;
			m_DicCHATProc[(int)CHAT.GROUP] = DO_CHAT_GROUP;
		}
		public Stub(){InitRegisterFuncDictionary();}
		public virtual int DO_CHAT_Proc(int type, Object pTarget, byte[] cPacket)
		{
			int ret = 0;
			if(!m_DicCHATProc.ContainsKey(type))
			{
				ret = DO_ERROR_PACKET(pTarget, cPacket);
				return ret;
			}
			ret = m_DicCHATProc[type](pTarget, cPacket);
			if(ret != 0)
				DO_ERROR_RESULT(pTarget, ret, type);
			return ret;
		}
		public abstract int DO_CHAT_GROUP(Object pTarget, byte[] pReqPacket);
		public abstract int DO_CHAT_NORMAL(Object pTarget, byte[] pReqPacket);
		public abstract int DO_CHAT_WHISPER(Object pTarget, byte[] pReqPacket);
		public abstract int DO_ERROR_PACKET(Object pTarget, byte[] pReqPacket);
		public abstract int DO_ERROR_RESULT(Object pTarget, int ret, int type);
	}
}