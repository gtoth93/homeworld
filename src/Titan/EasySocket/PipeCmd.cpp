#include "PipeCmd.h"

namespace WONMisc {
    PipeCmd::PipeCmd(const PipeCmdType theType, const bool deleteOnCompletion): mProcessing(false), mType(theType),
        mDeleteOnCompletion(deleteOnCompletion), mCompleted(false), mResult(ES_NO_ERROR) {
    }

    PipeCmd::~PipeCmd() = default;

    auto PipeCmd::GetType() const -> PipeCmdType {
        return mType;
    }

    OpenCmd::OpenCmd(const EasySocket::SocketType theSocketType,
                     const bool deleteOnCompletion) : PipeCmd(pctOpen, deleteOnCompletion), mSocketType(theSocketType) {
    }

    BindCmd::BindCmd(const uint16_t thePort, const bool allowReuse,
                     const bool deleteOnCompletion) : PipeCmd(pctBind, deleteOnCompletion), mPort(thePort),
                                                      mAllowReuse(allowReuse) {
    }

    CloseCmd::CloseCmd(const bool setLinger, const bool linger, const uint16_t theLingerSeconds,
                       const bool deleteOnCompletion) : PipeCmd(pctClose, deleteOnCompletion), mSetLinger(setLinger),
                                                        mLinger(linger), mLingerSeconds(theLingerSeconds) {
    }

    CloseNowCmd::CloseNowCmd(const bool setLinger, const bool linger, const uint16_t theLingerSeconds,
                             const bool deleteOnCompletion) : CloseCmd(setLinger, linger, theLingerSeconds,
                                                                       deleteOnCompletion) {
        mType = pctCloseNow;
    }

    ShutdownCmd::ShutdownCmd(const int32_t theHow, const bool deleteOnCompletion) : PipeCmd(pctShutdown,
        deleteOnCompletion), mHow(theHow) {
    }

    ListenCmd::ListenCmd(const int32_t theBacklog, const bool deleteOnCompletion) : PipeCmd(pctListen,
        deleteOnCompletion), mBacklog(theBacklog) {
    }

    AcceptCmd::AcceptCmd(EasySocketEngine* theSocketEngineP, const bool addToEngine,
                         const bool deleteOnCompletion) : PipeCmd(pctAccept, deleteOnCompletion),
                                                          mSocketEngineP(theSocketEngineP), mAddToEngine(addToEngine),
                                                          mAcceptedPipeP(nullptr), mSockAddr({}) {
    }

    ConnectCmd::ConnectCmd(const SOCKADDR&theDestSockAddr, const bool deleteOnCompletion) : PipeCmd(pctConnect,
        deleteOnCompletion), mSockAddr(theDestSockAddr) {
    }

    SmartConnectCmd::SmartConnectCmd(const SOCKADDR&theDestSockAddr, const int32_t theWaitTime,
                                     const int32_t theNumTries,
                                     const bool deleteOnCompletion) : PipeCmd(pctSmartConnect, deleteOnCompletion),
                                                                      mAttemptIndex(0), mStartTime(0),
                                                                      mSockAddr(theDestSockAddr),
                                                                      mWaitTime(theWaitTime), mNumTries(theNumTries) {
    }

    BufferCmd::BufferCmd(char* theBufferP, const int32_t theBufferLength, const bool ownsBuffer,
                         const bool deleteOnCompletion) : PipeCmd(pctNone, deleteOnCompletion), mBufferP(theBufferP),
                                                          mBufferLength(theBufferLength), mOwnsBuffer(ownsBuffer) {
    }

    BufferCmd::~BufferCmd() {
        if (mOwnsBuffer) {
            delete[] mBufferP;
            mBufferP = nullptr;
        }
    }

    SendCmd::SendCmd(char* theBufferP, const int32_t theBufferLength, const bool ownsBuffer,
                     const bool deleteOnCompletion) : BufferCmd(theBufferP, theBufferLength, ownsBuffer,
                                                                deleteOnCompletion), mSentSoFar(0) {
        mType = pctSend;
    }

    SendToCmd::SendToCmd(const SOCKADDR&theDestSockAddr, char* theBufferP, const int32_t theBufferLength,
                         const bool ownsBuffer, const bool deleteOnCompletion): SendCmd(theBufferP, theBufferLength,
        ownsBuffer, deleteOnCompletion), mSockAddr(theDestSockAddr) {
        mType = pctSendTo;
    }

    BroadcastCmd::BroadcastCmd(const uint16_t theDestPort, char* theBufferP, const int32_t theBufferLength,
                               const bool ownsBuffer,
                               const bool deleteOnCompletion): SendCmd(theBufferP, theBufferLength, ownsBuffer,
                                                                       deleteOnCompletion), mPort(theDestPort) {
        mType = pctBroadcast;
    }

    RecvCmd::RecvCmd(char* theBufferP, const int32_t theBytesToReceive, const bool ownsBuffer,
                     const bool deleteOnCompletion): BufferCmd(theBufferP, theBytesToReceive, ownsBuffer,
                                                               deleteOnCompletion), mBytesReceived(0),
                                                     mBytesToReceive(theBytesToReceive) {
        mType = pctRecv;
    }

    RecvPrefCmd::RecvPrefCmd(const bool ownsBuffer, const RecvLengthPrefixType thePrefixType,
                             const bool deleteOnCompletion): BufferCmd(nullptr, 0, ownsBuffer, deleteOnCompletion),
                                                             mPrefixLength(0), mBytesToReceive(0),
                                                             mReceivedLength(false), mPrefixType(thePrefixType),
                                                             mBytesReceived(0) {
        mType = pctRecvPrefixed;
        switch (mPrefixType) {
            case ptByte:
            case ptUnsignedByte:
                mPrefixLength = sizeof(uint8_t);
                break;
            case ptShort:
            case ptUnsignedShort:
                mPrefixLength = sizeof(uint16_t);
                break;
            case ptLong:
            case ptUnsignedLong:
                mPrefixLength = sizeof(uint32_t);
                break;
            default:
                break;
        }
        if (mPrefixLength > 0) {
            auto* buf = new char[mPrefixLength];
            mBufferP = buf;
            mBufferLength = static_cast<int32_t>(static_cast<uint8_t>(mPrefixLength));
        }
    }

    RecvFromCmd::RecvFromCmd(char* theBufferP, const int32_t theBufferLength, const bool ownsBuffer,
                             const bool deleteOnCompletion): BufferCmd(theBufferP, theBufferLength, ownsBuffer,
                                                                       deleteOnCompletion), mSockAddr({}),
                                                             mBytesReceived(0) {
        mType = pctRecvFrom;
    }

    CallBackCmd::CallBackCmd(const CallBackCmdPtr theCallBackCmdPtr, void* theCallBackParamP,
                             const bool deleteOnCompletion): PipeCmd(pctCallBack, deleteOnCompletion),
                                                             mCallBackCmdPtr(theCallBackCmdPtr),
                                                             mCallBackParamP(theCallBackParamP) {
    }

    SetEventCmd::SetEventCmd(void* theEvent, const bool deleteOnCompletion): PipeCmd(pctSetEvent, deleteOnCompletion),
                                                                             mEvent(theEvent) {
    }

    WaitForEventCmd::WaitForEventCmd(void* theEvent, const bool resetEvent,
                                     const bool deleteOnCompletion): PipeCmd(pctWaitForEvent, deleteOnCompletion),
                                                                     mEvent(theEvent), mResetEvent(resetEvent) {
    }

    TimerCmd::TimerCmd(const uint32_t theMillisecondsToWait,
                       const bool deleteOnCompletion): PipeCmd(pctTimer, deleteOnCompletion), mStartTime(0),
                                                       mMillisecondsToWait(theMillisecondsToWait) {
    }

    NoOpPayloadCmd::NoOpPayloadCmd(const uint32_t thePayloadP, const bool deleteOnCompletion): PipeCmd(pctNoOpPayload,
        deleteOnCompletion), mPayload(thePayloadP) {
    }
}
