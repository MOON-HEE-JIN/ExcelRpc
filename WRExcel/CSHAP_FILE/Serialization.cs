using System.Buffers.Binary;
using System.Text;
using STRUCT_DEF.Models;

namespace CSHAP_SERVER.Models
{
	public static class Serialization
	{
		public static int WritePacket(Span<byte> dst, st_CTS_ChangePid _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.pid); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_CTS_LoopBack _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.zone); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt64LittleEndian(dst.Slice(offset, sizeof(Int64)), _value.data); offset += sizeof(Int64);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_Header _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.type); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.size); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_ChangePid _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_STC_LoopBack _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.ret); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.zone); offset += sizeof(Int32);
			BinaryPrimitives.WriteInt64LittleEndian(dst.Slice(offset, sizeof(Int64)), _value.data); offset += sizeof(Int64);
			return offset;
		}
	}
	public static class UnSerialization
	{
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_ChangePid _value)
		{
			int offset = 0;
			_value.pid = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_CTS_LoopBack _value)
		{
			int offset = 0;
			_value.zone = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.data = BinaryPrimitives.ReadInt64LittleEndian(src.Slice(offset, sizeof(Int64))); offset += sizeof(Int64);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Header _value)
		{
			int offset = 0;
			_value.type = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.size = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_ChangePid _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_STC_LoopBack _value)
		{
			int offset = 0;
			_value.ret = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.zone = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			_value.data = BinaryPrimitives.ReadInt64LittleEndian(src.Slice(offset, sizeof(Int64))); offset += sizeof(Int64);
			return offset;
		}
	}
}
