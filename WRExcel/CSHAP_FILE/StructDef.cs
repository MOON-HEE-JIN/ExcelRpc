using ProjectDefineStruct;

namespace BaseStub
{
	public struct st_EntityInfo
	{
		public Int32		type;
		public Int32		ID;
		public st_Vector3F		pos;
	}
	public struct st_ConnectInfo
	{
		public Int32		ID;
	}
	public struct st_String
	{
		public Int16		length;
		public string		comment;
	}
	public struct st_Msg
	{
		public Int32		type;
		public st_String		Message;
	}
	public struct st_PlayerInfo
	{
		public Int32		ID;
		public st_Vector3F		pos;
		public float		speed;
	}
	public struct st_PlayerOtherMove
	{
		public Int32		ID;
		public st_Vector3F		pos;
		public st_Vector3F		dir;
	}
	public struct st_CTS_ChangeZone
	{
		public Int32		channel;
		public Int32		zone;
	}
	public struct st_CTS_LoopBack
	{
		public Int64		data;
	}
	public struct st_CTS_MoveStart
	{
		public st_Vector3F		dir;
		public st_Vector3F		goal;
		public st_Vector3F		pos;
	}
	public struct st_CTS_MoveStop
	{
		public st_Vector3F		pos;
	}
	public struct st_CTS_ObserverConnect
	{
		public Int32		ID;
		public Int32		zone;
	}
	public struct st_CTS_Teleport
	{
		public st_Vector3F		pos;
	}
	public struct st_STC_AoiInPlayer
	{
		public st_PlayerInfo		info;
	}
	public struct st_STC_AoiInPlayerMoves
	{
		public Int32		Loop1;
		public st_PlayerOtherMove		[]move; //Size::50
	}
	public struct st_STC_AoiInPlayers
	{
		public Int32		Loop1;
		public st_PlayerInfo		[]info; //Size::50
	}
	public struct st_STC_AoiOutPlayer
	{
		public Int32		ID;
	}
	public struct st_STC_AoiOutPlayers
	{
		public Int32		Loop1;
		public st_PlayerInfo		[]info; //Size::50
	}
	public struct st_STC_ChangeZone
	{
		public Int32		ret;
		public Int32		channel;
		public Int32		zone;
		public st_Vector3F		spawn;
	}
	public struct st_STC_ChangeingZone
	{
		public Int32		ret;
		public Int32		type;
	}
	public struct st_STC_ConnectInfo
	{
		public st_ConnectInfo		info;
	}
	public struct st_STC_LoopBack
	{
		public Int32		ret;
		public Int64		data;
	}
	public struct st_STC_MoveStart
	{
		public Int32		ret;
		public st_Vector3F		pos;
	}
	public struct st_STC_MoveStop
	{
		public Int32		ret;
		public Int32		type;
		public Int32		ID;
		public st_Vector3F		pos;
	}
	public struct st_STC_ObserverConnect
	{
		public Int32		ret;
	}
	public struct st_STC_OtherMoveStart
	{
		public Int32		type;
		public Int32		ID;
		public st_Vector3F		pos;
		public st_Vector3F		dir;
	}
	public struct st_STC_Teleport
	{
		public Int32		ret;
		public st_Vector3F		pos;
	}
}