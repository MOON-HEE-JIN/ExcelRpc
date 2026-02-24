namespace STRUCT_DEF.Models{
public struct st_Header
	{
		 public st_Header(){}
		public Int32		type;
		public Int32		size;
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