#if !defined(MMsgRoutingClientChange_H)
#define MMsgRoutingClientChange_H

// MMsgRoutingClientChange.h

#include "RoutingServerMessage.h"

namespace WONMsg {

//
// ClientChangeEx
//
class MMsgRoutingClientChange : public RoutingServerMessage {
public:
	enum { CLIENT_ENTERED = 0x80 };
	enum Reason { NEW_CLIENT = 0x80, VOLUNTARY_DISCONNECT = 0x00, CONNECTION_FAILURE = 0x01, BOOTED = 0x02 };

    // Default ctor
    MMsgRoutingClientChange(void);

    // RoutingServerMessage ctor
    explicit MMsgRoutingClientChange(const RoutingServerMessage& theMsgR, bool doUnpack =true);

    // Copy ctor
    MMsgRoutingClientChange(const MMsgRoutingClientChange& theMsgR);

    // Destructor
    virtual ~MMsgRoutingClientChange(void);

    // Assignment
    MMsgRoutingClientChange& operator=(const MMsgRoutingClientChange& theMsgR);

    // Virtual Duplicate from RoutingServerMessage
    virtual TRawMsg* Duplicate(void) const { return new MMsgRoutingClientChange(*this); }

	// Debug output
	virtual void Dump(std::ostream& os) const;

    // Pack and Unpack the message
    // Unpack will throw a BadMsgException is message is not of this type
    virtual void* Pack(void); 
    virtual void  Unpack(void);

    // Member access
	ClientId       GetClientId() const      { return mClientId; }
	unsigned char  GetReason() const        { return mReason; }
	bool           DidClientLeave() const   { return !DidClientEnter(); } 
	bool           DidClientEnter() const   { return (GetReason() & CLIENT_ENTERED) != 0; }

	void SetClientId(ClientId theClientId)  { mClientId = theClientId; }
	void SetReason(unsigned char theReason) { mReason = theReason; }
private:
	ClientId      mClientId;
	unsigned char mReason;
protected:
	virtual void SimplePack();
	virtual void SimpleUnpack();
};

//
// ChangeInfo
//
class ChangeInfo {
public:
	// Default ctor
	ChangeInfo();

	// Copy ctor
	ChangeInfo(const ChangeInfo& theCopyR);

	// Assignment
	ChangeInfo& operator=(const ChangeInfo& theCopyR);

	// Debug output
	virtual void Dump(std::ostream& os) const;

	// Pack and Unpack the data to/from a message object
	void Pack(RoutingServerMessage* theMsgP);
	void Unpack(RoutingServerMessage* theMsgP);

	const ClientName& GetClientName() const              { return mClientName; }
	unsigned long     GetIPAddress() const               { return mIPAddress; }
	unsigned long     GetWONUserId() const               { return mWONUserId; }
	unsigned long     GetCommunityId() const             { return mCommunityId; }
	unsigned short    GetTrustLevel() const              { return mTrustLevel; }
	bool              IsModerator() const                { return mIsModerator; }
	bool              IsMuted() const                    { return mIsMuted; }
	bool              IncludesIPs() const                { return mIncludesIPs; }
	bool              IncludesAuthInfo() const           { return mIncludesAuthInfo; } 
	bool              IncludesModeratorFlag() const      { return mIncludesModeratorFlag; }
	bool              IncludesMuteFlag() const           { return mIncludesMuteFlag; }

	void SetClientName(const ClientName& theClientNameR)      { mClientName = theClientNameR; }
	void SetIPAddress(unsigned long theIPAddress)             { mIPAddress = theIPAddress; }
	void SetWONUserId(unsigned long theWONUserId)             { mWONUserId = theWONUserId; }
	void SetCommunityId(unsigned long theCommunityId)         { mCommunityId = theCommunityId; }
	void SetTrustLevel(unsigned short theTrustLevel)          { mTrustLevel = theTrustLevel; }
	void SetIsModerator(bool isModerator)                     { mIsModerator = isModerator; }
	void SetIsMuted(bool isMuted)                             { mIsMuted = isMuted; }
	void SetIncludesIPs(bool includesIPs)                     { mIncludesIPs = includesIPs; }
	void SetIncludesAuthInfo(bool includesAuthInfo)           { mIncludesAuthInfo = includesAuthInfo; }
	void SetIncludesModeratorFlag(bool includesModeratorFlag) { mIncludesModeratorFlag = includesModeratorFlag; }
	void SetIncludesMuteFlag(bool includesMuteFlag)           { mIncludesMuteFlag = includesMuteFlag; }
private:
	ClientName     mClientName;
	unsigned long  mIPAddress;
	unsigned long  mWONUserId;
	unsigned long  mCommunityId;
	unsigned short mTrustLevel;
	bool           mIsModerator;
	bool           mIsMuted;
	bool           mIncludesIPs;
	bool           mIncludesAuthInfo;
	bool           mIncludesModeratorFlag;
	bool           mIncludesMuteFlag;
};

//
// ClientChangeEx
//
class MMsgRoutingClientChangeEx : public MMsgRoutingClientChange, public ChangeInfo {
public:
    // Default ctor
    MMsgRoutingClientChangeEx(void);

    // RoutingServerMessage ctor
    explicit MMsgRoutingClientChangeEx(const RoutingServerMessage& theMsgR, bool doUnpack =true);

    // Copy ctor
    MMsgRoutingClientChangeEx(const MMsgRoutingClientChangeEx& theMsgR);

    // Destructor
    virtual ~MMsgRoutingClientChangeEx(void);

    // Assignment
    MMsgRoutingClientChangeEx& operator=(const MMsgRoutingClientChangeEx& theMsgR);

    // Virtual Duplicate from RoutingServerMessage
    virtual TRawMsg* Duplicate(void) const { return new MMsgRoutingClientChangeEx(*this); }

	// Debug output
	virtual void Dump(std::ostream& os) const;

    // Pack and Unpack the message
    // Unpack will throw a BadMsgException is message is not of this type
    virtual void* Pack(void); 
    virtual void  Unpack(void);
};

};  // Namespace WONMsg

#endif // MMsgRoutingClientChange_H