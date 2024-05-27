#include "common/won.h"
#include "msg/BadMsgException.h"
#include "msg/MServiceTypes.h"
#include "MMsgTypesRouting.h"
#include "RoutingServerFlagMessage.h"

namespace {
	using WONMsg::RoutingServerMessage;
	using WONMsg::RoutingServerFlagMessage;
};

RoutingServerFlagMessage::RoutingServerFlagMessage(bool flagOnOrOff) : 
	mOnOrOffFlag(flagOnOrOff)
{}

RoutingServerFlagMessage::~RoutingServerFlagMessage(void)
{}

RoutingServerFlagMessage::RoutingServerFlagMessage(const RoutingServerFlagMessage& theMsgR) :
    RoutingServerMessage(theMsgR),
	mOnOrOffFlag(theMsgR.mOnOrOffFlag)
{}

RoutingServerFlagMessage::RoutingServerFlagMessage(const RoutingServerMessage& theMsgR) : 
    RoutingServerMessage(theMsgR)
{
	Unpack();
}

RoutingServerFlagMessage& RoutingServerFlagMessage::operator =(const RoutingServerFlagMessage& theMsgR)
{
	if (this != &theMsgR)
	{
		RoutingServerMessage::operator=(theMsgR);
		mOnOrOffFlag = theMsgR.mOnOrOffFlag;
	}

	return *this;
}

void RoutingServerFlagMessage::Dump(std::ostream& os) const
{
	RoutingServerMessage::Dump(os);	
	
	os << "  On/Off: " << (mOnOrOffFlag ? "on" : "off") << endl;
}

void* RoutingServerFlagMessage::Pack(void)
{
	WTRACE("RoutingServerFlagMessage::Pack");
	RoutingServerMessage::Pack();

	WDBG_LL("RoutingServerFlagMessage::Pack Appending message data");

	AppendBool(mOnOrOffFlag);

	return GetDataPtr();
}

void RoutingServerFlagMessage::Unpack(void)
{
	WTRACE("RoutingServerFlagMessage::Unpack");
	RoutingServerMessage::Unpack();

	WDBG_LL("RoutingServerFlagMessage::Unpack Reading message data");

	mOnOrOffFlag = ReadBool();
}

//
// RoutingServerClientIdFlagMessage
//
WONMsg::RoutingServerClientIdFlagMessage::RoutingServerClientIdFlagMessage(bool flagOnOrOff) :
	RoutingServerFlagMessage(flagOnOrOff),
	mClientId(0)
{}

WONMsg::RoutingServerClientIdFlagMessage::~RoutingServerClientIdFlagMessage(void)
{}

WONMsg::RoutingServerClientIdFlagMessage::RoutingServerClientIdFlagMessage(const RoutingServerClientIdFlagMessage& theMsgR) :
    RoutingServerFlagMessage(theMsgR),
	mClientId(theMsgR.mClientId)
{}

WONMsg::RoutingServerClientIdFlagMessage::RoutingServerClientIdFlagMessage(const RoutingServerMessage& theMsgR) :
    RoutingServerFlagMessage(theMsgR)
{
	Unpack();
}

WONMsg::RoutingServerClientIdFlagMessage& WONMsg::RoutingServerClientIdFlagMessage::operator =(const RoutingServerClientIdFlagMessage& theMsgR)
{
	if (this != &theMsgR)
	{
		RoutingServerFlagMessage::operator=(theMsgR);
	    mClientId  = theMsgR.mClientId;
	}

	return *this;
}

void WONMsg::RoutingServerClientIdFlagMessage::Dump(std::ostream& os) const
{
	RoutingServerFlagMessage::Dump(os);	
	
	os << "  ClientId: " << mClientId << endl;
}

void* WONMsg::RoutingServerClientIdFlagMessage::Pack(void)
{
	WTRACE("RoutingServerClientIdFlagMessage::Pack");
	RoutingServerFlagMessage::Pack();

	WDBG_LL("RoutingServerClientIdFlagMessage::Pack Appending message data");

	AppendClientId(mClientId);

	return GetDataPtr();
}

void WONMsg::RoutingServerClientIdFlagMessage::Unpack(void)
{
	WTRACE("RoutingServerClientIdFlagMessage::Unpack");
	RoutingServerFlagMessage::Unpack();

	WDBG_LL("RoutingServerClientIdFlagMessage::Unpack Reading message data");

	mClientId    = ReadClientId();
}

//
// RoutingServerClientNameFlagMessage
//
WONMsg::RoutingServerClientNameFlagMessage::RoutingServerClientNameFlagMessage(bool flagOnOrOff) :
	RoutingServerFlagMessage(flagOnOrOff)
{}

WONMsg::RoutingServerClientNameFlagMessage::~RoutingServerClientNameFlagMessage(void)
{}

WONMsg::RoutingServerClientNameFlagMessage::RoutingServerClientNameFlagMessage(const RoutingServerClientNameFlagMessage& theMsgR) :
    RoutingServerFlagMessage(theMsgR),
	mClientName(theMsgR.mClientName)
{}

WONMsg::RoutingServerClientNameFlagMessage::RoutingServerClientNameFlagMessage(const RoutingServerMessage& theMsgR) :
    RoutingServerFlagMessage(theMsgR)
{
	Unpack();
}

WONMsg::RoutingServerClientNameFlagMessage& WONMsg::RoutingServerClientNameFlagMessage::operator =(const RoutingServerClientNameFlagMessage& theMsgR)
{
	if (this != &theMsgR)
	{
		RoutingServerFlagMessage::operator=(theMsgR);
	    mClientName  = theMsgR.mClientName;
	}

	return *this;
}

void WONMsg::RoutingServerClientNameFlagMessage::Dump(std::ostream& os) const
{
	RoutingServerFlagMessage::Dump(os);	
	
	os << "  ClientName: " << mClientName << endl;
}

void* WONMsg::RoutingServerClientNameFlagMessage::Pack(void)
{
	WTRACE("RoutingServerClientNameFlagMessage::Pack");
	RoutingServerFlagMessage::Pack();

	WDBG_LL("RoutingServerClientNameFlagMessage::Pack Appending message data");

	AppendClientName(mClientName);

	return GetDataPtr();
}

void WONMsg::RoutingServerClientNameFlagMessage::Unpack(void)
{
	WTRACE("RoutingServerClientNameFlagMessage::Unpack");
	RoutingServerFlagMessage::Unpack();

	WDBG_LL("RoutingServerClientNameFlagMessage::Unpack Reading message data");

	ReadClientName(mClientName);
}

//
// RoutingServerGenericFlagMessage
//
//
// RoutingServerClientNameFlagMessage
//
WONMsg::RoutingServerGenericFlagMessage::RoutingServerGenericFlagMessage(bool flagOnOrOff) :
	RoutingServerFlagMessage(flagOnOrOff),
	mContentType(CONTENTTYPE_UNKNOWN),
	mClientId(0),
	mWONUserId(0)
{}

WONMsg::RoutingServerGenericFlagMessage::~RoutingServerGenericFlagMessage(void)
{}

WONMsg::RoutingServerGenericFlagMessage::RoutingServerGenericFlagMessage(const RoutingServerGenericFlagMessage& theMsgR) :
    RoutingServerFlagMessage(theMsgR),
	mContentType(theMsgR.mContentType),
	mClientId(theMsgR.mClientId),
	mClientName(theMsgR.mClientName),
	mWONUserId(theMsgR.mWONUserId)
{}

WONMsg::RoutingServerGenericFlagMessage::RoutingServerGenericFlagMessage(const RoutingServerMessage& theMsgR) :
    RoutingServerFlagMessage(theMsgR)
{
	Unpack();
}

WONMsg::RoutingServerGenericFlagMessage& WONMsg::RoutingServerGenericFlagMessage::operator =(const RoutingServerGenericFlagMessage& theMsgR)
{
	if (this != &theMsgR)
	{
		RoutingServerFlagMessage::operator=(theMsgR);
		mContentType = theMsgR.mContentType;
		mClientId    = theMsgR.mClientId;
	    mClientName  = theMsgR.mClientName;
		mWONUserId   = theMsgR.mWONUserId;
	}

	return *this;
}

void WONMsg::RoutingServerGenericFlagMessage::Dump(std::ostream& os) const
{
	RoutingServerFlagMessage::Dump(os);	
	
	os << "  ContentType: ";
	switch (mContentType)
	{
		default:
		case CONTENTTYPE_UNKNOWN:
			os << "UNKNOWN" << endl; break;
		case CONTENTTYPE_CLIENTID:
			os << "CLIENTID" << endl;
			os << "  ClientId: " << mClientId << endl;
			break;
		case CONTENTTYPE_CLIENTNAME:
			os << "CLIENTNAME" << endl;
			os << "  ClientName: " << mClientName << endl;
			break;
		case CONTENTTYPE_WONUSERID:
			os << "WONUSERID" << endl;
			os << "  WONUserId:" << mWONUserId << endl;
			break;
	}
}

void* WONMsg::RoutingServerGenericFlagMessage::Pack(void)
{
	WTRACE("RoutingServerGenericFlagMessage::Pack");
	RoutingServerFlagMessage::Pack();

	WDBG_LL("RoutingServerGenericFlagMessage::Pack Appending message data");

	AppendByte(mContentType);
	switch (mContentType)
	{
		case CONTENTTYPE_CLIENTID:
			AppendClientId(mClientId); break;
		case CONTENTTYPE_CLIENTNAME:
			AppendClientName(mClientName); break;
		case CONTENTTYPE_WONUSERID:
			AppendLong(mWONUserId); break;
	}

	return GetDataPtr();
}

void WONMsg::RoutingServerGenericFlagMessage::Unpack(void)
{
	WTRACE("RoutingServerGenericFlagMessage::Unpack");
	RoutingServerFlagMessage::Unpack();

	WDBG_LL("RoutingServerGenericFlagMessage::Unpack Reading message data");

	mContentType = (ContentType)ReadByte();
	switch (mContentType)
	{
		case CONTENTTYPE_CLIENTID:
			mClientId = ReadClientId(); break;
		case CONTENTTYPE_CLIENTNAME:
			ReadClientName(mClientName); break;
		case CONTENTTYPE_WONUSERID:
			mWONUserId = ReadLong(); break;
		default:
			mContentType = CONTENTTYPE_UNKNOWN; break;
	}
}