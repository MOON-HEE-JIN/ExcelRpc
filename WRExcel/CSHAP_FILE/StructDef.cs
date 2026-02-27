namespace STRUCT_DEF.Models{
public struct st_Header
	{
		 public st_Header(){}
		public Int32		type;
		public Int32		size;
	}
public struct st_Vector
	{
		 public st_Vector(){}
		public float		X;
		public float		Y;
	}
public struct st_Info1
	{
		 public st_Info1(){}
		public Int32		Loop1;
		public Int32		[]ID = new Int32[50];
	}
public struct st_Info2
	{
		 public st_Info2(){}
		public Int32		Loop1;
		public st_Vector		[]vec = new st_Vector[100];
	}
public struct st_Info3
	{
		 public st_Info3(){}
		public Int32		Loop1;
		public st_Vector		[]vec = new st_Vector[100];
		public Int32		Loop2;
		public Int32		[]ID = new Int32[100];
	}
	public struct st_CTS_ChangePid
	{
		public st_CTS_ChangePid(){}
		public Int32		pid;
	}
	public struct st_CTS_LoopBack
	{
		public st_CTS_LoopBack(){}
		public Int32		zone;
		public Int64		data;
	}
	public struct st_STC_ChangePid
	{
		public st_STC_ChangePid(){}
		public Int32		ret;
	}
	public struct st_STC_LoopBack
	{
		public st_STC_LoopBack(){}
		public Int32		ret;
		public Int32		zone;
		public Int64		data;
	}
}