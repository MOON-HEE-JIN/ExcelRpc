namespace STRUCT_DEF.Models
{
	public struct st_String
	{
		public Int16		length;
		public string		msg;
	}
	public struct st_Msg
	{
		public Int32		MsgID;
		public st_String		Message;
	}
	public struct st_CTS_GROUP
	{
		public st_CTS_GROUP(){}
		public Int32		group_ID;
		public st_Msg		msg;
	}
	public struct st_CTS_NORMAL
	{
		public st_CTS_NORMAL(){}
		public st_Msg		msg;
	}
	public struct st_CTS_WHISPER
	{
		public st_CTS_WHISPER(){}
		public Int32		whisper_ID;
		public st_Msg		msg;
	}
	public struct st_STC_GROUP
	{
		public st_STC_GROUP(){}
		public Int32		group_ID;
		public st_Msg		msg;
	}
	public struct st_STC_NORMAL
	{
		public st_STC_NORMAL(){}
		public st_Msg		msg;
	}
	public struct st_STC_WHISPER
	{
		public st_STC_WHISPER(){}
		public Int32		whisper_ID;
		public st_Msg		msg;
	}
}