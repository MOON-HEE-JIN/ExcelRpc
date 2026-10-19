using System.Buffers.Binary;
using System.Text;
using STRUCT_DEF.Models;

namespace Serialization.Models
{
	public static class Serialization
	{
		public static int WritePacket(Span<byte> dst, st_CTS_GROUP _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.group_ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.msg);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_NORMAL _value)
		{
			int offset = 0;
			offset += WritePacket(dst.Slice(offset), _value.msg);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_WHISPER _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.whisper_ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.msg);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_Msg _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.MsgID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.Message);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_GROUP _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.group_ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.msg);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_NORMAL _value)
		{
			int offset = 0;
			offset += WritePacket(dst.Slice(offset), _value.msg);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_WHISPER _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.whisper_ID); offset += sizeof(Int32);
			offset += WritePacket(dst.Slice(offset), _value.msg);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_String _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt16LittleEndian(dst.Slice(offset, sizeof(Int16)), _value.length); offset += sizeof(Int16);
			Encoding.UTF8.GetBytes(_value.msg, dst.Slice(offset, _value.length));offset += _value.length;
			return offset;
		}
	}
	public static class UnSerialization
	{
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_CTS_GROUP _value)
		{
			int offset = 0;
			_value.group_ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), out _value.msg);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_CTS_NORMAL _value)
		{
			int offset = 0;
			offset += ReadPacket(src.Slice(offset), out _value.msg);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_CTS_WHISPER _value)
		{
			int offset = 0;
			_value.whisper_ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), out _value.msg);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_Msg _value)
		{
			int offset = 0;
			_value.MsgID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), out _value.Message);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_STC_GROUP _value)
		{
			int offset = 0;
			_value.group_ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), out _value.msg);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_STC_NORMAL _value)
		{
			int offset = 0;
			offset += ReadPacket(src.Slice(offset), out _value.msg);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_STC_WHISPER _value)
		{
			int offset = 0;
			_value.whisper_ID = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			offset += ReadPacket(src.Slice(offset), out _value.msg);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, out st_String _value)
		{
			int offset = 0;
			_value.length = BinaryPrimitives.ReadInt16LittleEndian(src.Slice(offset, sizeof(Int16))); offset += sizeof(Int16);
			_value.msg = Encoding.UTF8.GetString(src.Slice(offset, _value.length));offset += _value.length;
			return offset;
		}
	}
}
