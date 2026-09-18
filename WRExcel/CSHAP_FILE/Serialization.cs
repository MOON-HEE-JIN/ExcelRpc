using System.Buffers.Binary;
using System.Text;
using STRUCT_DEF.Models;

namespace CSHAP_SERVER.Models
{
	public static class Serialization
	{
		public static int WritePacket(Span<byte> dst, st_CTS_ChangeZone _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.channel); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.zone); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_LoopBack _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt64LittleEndian(dst.Slice(offset, sizeof(Int64)), _value.data); offset += sizeof(Int64);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_MoveStart _value)
		{
			int offset = 0;
			offset += WritePacket(dst.Slice(offset), _value.dir);
			offset += WritePacket(dst.Slice(offset), _value.goal);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_MoveStop _value)
		{
			int offset = 0;
			offset += WritePacket(dst.Slice(offset), _value.pos);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_ObserverConnect _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.zone); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_Teleport _value)
		{
			int offset = 0;
			offset += WritePacket(dst.Slice(offset), _value.pos);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_ConnectInfo _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_EntityInfo _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.type); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_Header _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.type); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.size); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_Msg _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.type); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.Message);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_PlayerInfo _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(float)), BitConverter.SingleToInt32Bits(_value.speed)); offset += sizeof(float);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_PlayerOtherMove _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			offset += WritePacket(dst.Slice(offset), _value.dir);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_AoiInPlayer _value)
		{
			int offset = 0;
			offset += WritePacket(dst.Slice(offset), _value.info);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_AoiInPlayerMoves _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.Loop1); offset += sizeof(Int32);
			foreach(var v in _value.move)
			{
				offset += WritePacket(dst.Slice(offset), v);
			}
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_AoiInPlayers _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.Loop1); offset += sizeof(Int32);
			foreach(var v in _value.info)
			{
				offset += WritePacket(dst.Slice(offset), v);
			}
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_AoiOutPlayer _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_AoiOutPlayers _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.Loop1); offset += sizeof(Int32);
			foreach(var v in _value.info)
			{
				offset += WritePacket(dst.Slice(offset), v);
			}
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_ChangeZone _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.channel); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.zone); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.spawn);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_ChangeingZone _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.type); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_ConnectInfo _value)
		{
			int offset = 0;
			offset += WritePacket(dst.Slice(offset), _value.info);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_LoopBack _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt64LittleEndian(dst.Slice(offset, sizeof(Int64)), _value.data); offset += sizeof(Int64);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_MoveStart _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_MoveStop _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.type); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_ObserverConnect _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_OtherMoveStart _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.type); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			offset += WritePacket(dst.Slice(offset), _value.dir);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_Teleport _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.pos);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_String _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt16LittleEndian(dst.Slice(offset, sizeof(Int16)), _value.length); offset += sizeof(Int16);
			Encoding.UTF8.GetBytes(_value.msg, dst.Slice(offset, _value.length));offset += _value.length;
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_Vector3F _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(float)), BitConverter.SingleToInt32Bits(_value.X)); offset += sizeof(float);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(float)), BitConverter.SingleToInt32Bits(_value.Y)); offset += sizeof(float);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(float)), BitConverter.SingleToInt32Bits(_value.Z)); offset += sizeof(float);
			return offset;
		}
	}
	public static class UnSerialization
	{
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_ChangeZone _value)
		{
			int offset = 0;
			_value.channel = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.zone = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_LoopBack _value)
		{
			int offset = 0;
			_value.data = BinaryPrimitives.ReadInt64LittleEndian(src.Slice(offset, sizeof(Int64))); offset += sizeof(Int64);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_MoveStart _value)
		{
			int offset = 0;
			offset += ReadPacket(src.Slice(offset), ref _value.dir);
			offset += ReadPacket(src.Slice(offset), ref _value.goal);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_MoveStop _value)
		{
			int offset = 0;
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_ObserverConnect _value)
		{
			int offset = 0;
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.zone = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_Teleport _value)
		{
			int offset = 0;
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_ConnectInfo _value)
		{
			int offset = 0;
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_EntityInfo _value)
		{
			int offset = 0;
			_value.type = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Header _value)
		{
			int offset = 0;
			_value.type = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.size = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Msg _value)
		{
			int offset = 0;
			_value.type = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.Message);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_PlayerInfo _value)
		{
			int offset = 0;
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			{
			int bits = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(float))); offset += sizeof(float);
			_value.speed = BitConverter.Int32BitsToSingle(bits);
			}
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_PlayerOtherMove _value)
		{
			int offset = 0;
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			offset += ReadPacket(src.Slice(offset), ref _value.dir);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_AoiInPlayer _value)
		{
			int offset = 0;
			offset += ReadPacket(src.Slice(offset), ref _value.info);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_AoiInPlayerMoves _value)
		{
			int offset = 0;
			_value.Loop1 = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			for(int i = 0; i < 50; i++)
			{
				offset += ReadPacket(src.Slice(offset), ref _value. move[i]);
			}
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_AoiInPlayers _value)
		{
			int offset = 0;
			_value.Loop1 = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			for(int i = 0; i < 50; i++)
			{
				offset += ReadPacket(src.Slice(offset), ref _value. info[i]);
			}
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_AoiOutPlayer _value)
		{
			int offset = 0;
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_AoiOutPlayers _value)
		{
			int offset = 0;
			_value.Loop1 = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			for(int i = 0; i < 50; i++)
			{
				offset += ReadPacket(src.Slice(offset), ref _value. info[i]);
			}
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_ChangeZone _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.channel = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.zone = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.spawn);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_ChangeingZone _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.type = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_ConnectInfo _value)
		{
			int offset = 0;
			offset += ReadPacket(src.Slice(offset), ref _value.info);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_LoopBack _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.data = BinaryPrimitives.ReadInt64LittleEndian(src.Slice(offset, sizeof(Int64))); offset += sizeof(Int64);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_MoveStart _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_MoveStop _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.type = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_ObserverConnect _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_OtherMoveStart _value)
		{
			int offset = 0;
			_value.type = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			offset += ReadPacket(src.Slice(offset), ref _value.dir);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_Teleport _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), ref _value.pos);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_String _value)
		{
			int offset = 0;
			_value.length = BinaryPrimitives.ReadInt16LittleEndian(src.Slice(offset, sizeof(Int16))); offset += sizeof(Int16);
			_value.msg = Encoding.UTF8.GetString(src.Slice(offset, _value.length));offset += _value.length;
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Vector3F _value)
		{
			int offset = 0;
			{
			int bits = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(float))); offset += sizeof(float);
			_value.X = BitConverter.Int32BitsToSingle(bits);
			}
			{
			int bits = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(float))); offset += sizeof(float);
			_value.Y = BitConverter.Int32BitsToSingle(bits);
			}
			{
			int bits = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(float))); offset += sizeof(float);
			_value.Z = BitConverter.Int32BitsToSingle(bits);
			}
			return offset;
		}
	}
}
