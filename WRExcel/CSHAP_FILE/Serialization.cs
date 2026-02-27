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
		public static int WritePacket(Span<byte> dst, st_Info1 _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.Loop1); offset += sizeof(Int32);
			foreach(var v in _value.ID)
			{
				BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), v); offset += sizeof(Int32);
			}
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_Info2 _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.Loop1); offset += sizeof(Int32);
			foreach(var v in _value.vec)
			{
				offset += WritePacket(dst.Slice(offset), v);
			}
			return offset;
		}
		public static int WritePacket(Span<byte> dst, st_Info3 _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.Loop1); offset += sizeof(Int32);
			foreach(var v in _value.vec)
			{
				offset += WritePacket(dst.Slice(offset), v);
			}
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), _value.Loop2); offset += sizeof(Int32);
			foreach(var v in _value.ID)
			{
				BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(Int32)), v); offset += sizeof(Int32);
			}
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
		public static int WritePacket(Span<byte> dst, st_Vector _value)
		{
			int offset = 0;
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(float)), BitConverter.SingleToInt32Bits(_value.X)); offset += sizeof(float);
			BinaryPrimitives.WriteInt32LittleEndian(dst.Slice(offset, sizeof(float)), BitConverter.SingleToInt32Bits(_value.Y)); offset += sizeof(float);
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
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Info1 _value)
		{
			int offset = 0;
			_value.Loop1 = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			for(int i = 0; i < 50; i++)
			{
				_value.ID[i] = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			}
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Info2 _value)
		{
			int offset = 0;
			_value.Loop1 = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			for(int i = 0; i < 100; i++)
			{
				offset += ReadPacket(src.Slice(offset), ref _value. vec[i]);
			}
			return offset;
		}
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Info3 _value)
		{
			int offset = 0;
			_value.Loop1 = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			for(int i = 0; i < 100; i++)
			{
				offset += ReadPacket(src.Slice(offset), ref _value. vec[i]);
			}
			_value.Loop2 = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			for(int i = 0; i < 100; i++)
			{
				_value.ID[i] = BinaryPrimitives.ReadInt32LittleEndian(src.Slice(offset, sizeof(Int32))); offset += sizeof(Int32);
			}
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
		public static int ReadPacket(ReadOnlySpan<byte> src, ref st_Vector _value)
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
			return offset;
		}
	}
}
