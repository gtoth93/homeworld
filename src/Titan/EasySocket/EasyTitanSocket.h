/******************************************************************************/
/*                                                                            */
/*                                 EASYTITANSOCKET.H                          */
/*                               WONMisc Socket Class                         */
/*                                   Include File                             */
/*                                                                            */
/******************************************************************************/

/*

    Class:              EasyTitanSocket

    Description:        This class implements methods for easilly sending and
                        receiving Titan messages.

    Author:             Brian Rothstein
    Last Modified:      28 Sept 98

    Base Classes:       EasySocket

    Contained Classes:  none

    Friend Classes:     none

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    Contained Data Types: none

    Private Data Members: none

    Protected Data Members: None

    Public Data Members: None

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    Constructors:
        EasyTitanSocket(SocketType theType)
            This is the main constructor that you want to use since most of the
            time you know what kind of socket you want when you declare it.  The
            constructor doesn't do much.  It doesn't actually get a new socket
            descriptor.  It just initializes the type so that when a connect or
            sendto is performed, the correct kind of socket can be opened.

        EasyTitanSocket()
            The default constructor sets the socket's type to NO_TYPE.  Before
            using the socket you must call the setType method.  This constructor
            is useful if you don't initially know what kind of socket you need.

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    Private Methods: none

    Protected Methods: None

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    Public Methods:
        ES_ErrorType sendTMessage(TMessage *theMsg, int theTotalTime = 1000);
        ES_ErrorType recvTMessage(TMessage *theMsg, int theTotalTime = 1000);
            Send and receive Titan messages.  These functions provide convenient
            ways of sending and receiving Titan messages on connected sockets.
            They handle packing and unpacking of the message and receiving the
            header and assuring that the header types match and that the total
            length is received.  If a base class TMessage is passes to the
            recvTMessage method, then no check is performed for matching types
            and unpack is not called.

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    Accessors: none
*/


#ifndef TITAN_EASYSOCKET_EASYTITANSOCKET_H
#define TITAN_EASYSOCKET_EASYTITANSOCKET_H

#include "EasySocket.h"
#include "auth/Auth1Certificate.h"
#include "auth/Auth1PublicKeyBlock.h"
#include "auth/CryptFlags.h"
#include "crypt/BFSymmetricKey.h"
#include "crypt/EGPrivateKey.h"
#include "msg/TMessage.h"

using namespace WONMsg;

namespace WONMisc {
    struct TagInfo {
        int32_t mTag{-1};
        int16_t mTagDuration{-1};
    };

    class EasyTitanSocket final : public EasySocket {
    protected:
        uint32_t mLength, mCurLen;
        char* mBuffer;
        uint8_t mLengthFieldSize, mTCPLengthFieldSize;

        WONAuth::Auth1PublicKeyBlock mPubKeyBlock;
        WONAuth::Auth1Certificate mCertificate;
        WONCrypt::EGPrivateKey mPrivateKey;
        WONCrypt::BFSymmetricKey mSessionKey;
        bool mIsSequenced, mIsEncrypted, mIsSessioned;
        uint16_t mInSeq, mOutSeq, mSessionId;

        void GetServiceAndMessageTypes(const BaseMessage* theMsgP, uint32_t* theServiceTypeP,
                                       uint32_t* theMessageTypeP);

        auto recvLength(uint32_t theHeaderLength, int32_t theTotalTime) -> ES_ErrorType;

        auto recvMessage(uint32_t theHeaderLength, int32_t theTotalTime) -> ES_ErrorType;

        auto ExtractMsg(BaseMessage** theMsgPP, int32_t theOffset, TagInfo&theTag) -> ES_ErrorType;

        auto ExtractTag(BaseMessage** theMsgPP, int32_t theOffset, TagInfo&theTag) -> ES_ErrorType;

        void init();

    public:
        EasyTitanSocket();

        explicit EasyTitanSocket(SocketType theType);

        ~EasyTitanSocket() override;

        void setType(SocketType theType) override;

        void SetLengthFieldSize(uint8_t theLengthFieldSize);

        auto GetLengthFieldSize() const -> uint8_t;

        auto sendMessage(BaseMessage* theMsg, int32_t theTotalTime = 1000) -> ES_ErrorType;

        auto sendMessage(BaseMessage* theMsg, const TagInfo&theTag, int32_t theTotalTime = 1000) -> ES_ErrorType;

        auto recvMessage(BaseMessage* theMsg, int32_t theTotalTime = 5000) -> ES_ErrorType;

        auto recvMessage(BaseMessage** theMsgPP, int32_t theTotalTime = 5000) -> ES_ErrorType;

        auto recvMessage(BaseMessage** theMsgPP, TagInfo&theTag, int32_t theTotalTime = 5000) -> ES_ErrorType;

        auto GetCertificate(const string&theUserName, const string&theCommunity, const string&thePassword,
                            int32_t theTotalTime = 15000) -> ES_ErrorType;

        auto CreateAccount(const string&theUserName, const string&theCommunity, const string&thePassword,
                           int32_t theTotalTime = 15000) -> ES_ErrorType;

        auto ChangePassword(const string&theUserName, const string&theCommunity, const string&thePassword,
                            const string&theNewPassword, int32_t theTotalTime = 15000) -> ES_ErrorType;

        auto DoLogin(bool createAccount, const string&theUserName, const string&theCommunity,
                     const string&theNickNameKey, const string&thePassword, const string&theNewPassword,
                     const string&theNickName, int32_t theTotalTime = 10000) -> ES_ErrorType;

        auto Authenticate(WONAuth::AuthenticationMode theAuthMode, WONAuth::EncryptionMode theEncryptMode,
                          uint16_t theEncryptFlags, int32_t theTotalTime = 10000) -> ES_ErrorType;

        void ResetAuth();

        auto GetSessionKey() const -> const WONCrypt::BFSymmetricKey&;
    };
}; //end namespace WONMisc
#endif
