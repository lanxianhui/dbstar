package com.dbstar.model;

public class EventData {
	public static final int EVENT_DELETE = 0xa0001;
	public static final int EVENT_DATASIGNAL = 0xa0002;
	public static final int EVENT_UPDATE_PROPERTY = 0xa0003;
	public static final int EVENT_SMARTCARD_STATUS = 0xa0004;
	public static final int EVENT_NEWMAIL = 0xa0005;
	public static final int EVENT_NOTIFICATION = 0xa0006;
	
	public static final int EVENT_LOGIN_SUCCESSED = 0xa0007;
	public static final int EVENT_GUODIAN_DATA = 0xa0008;
	
	public static class DeleteEvent {
		public String PublicationId;
		public String PublicationSetId;
	}
	
	public static class DataSignalEvent {
		public boolean hasSignal;
	}
	
	public static class UpdatePropertyEvent {
		public String PublicationId;
		public String PropertyName;
		public Object PropertyValue;
	}
	
	public static class SmartcardStatus {
		public int State;
		
		public SmartcardStatus() {
			State = GDCommon.SMARTCARD_STATE_NONE;
		}
	}

	public static class GuodianEvent {
		public int Type;
		public Object Data;
	}
}
