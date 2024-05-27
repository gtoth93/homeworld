#ifndef TITAN_EASYSOCKET_PIPECMD_H
#define TITAN_EASYSOCKET_PIPECMD_H

#include "EasySocket.h"

namespace WONMisc {
    class EasySocketEngine;
    ////////////////////

    enum PipeCmdType {
        pctNone,
        pctOpen,
        pctBind,
        pctClose,
        pctCloseNow,
        pctShutdown,
        pctListen,
        pctAccept,
        pctConnect,
        pctSmartConnect,
        pctSend,
        pctSendTo,
        pctBroadcast,
        pctRecv,
        pctRecvPrefixed,
        pctRecvFrom,
        pctCallBack,
        pctSetEvent,
        pctWaitForEvent,
        pctTimer,
        pctNoOpPayload
    };


    ////////////////////

    class PipeCmd {
        friend class SocketPipe;
        bool mProcessing; // 4 1 byte with 3 byte padding

    protected:
        PipeCmdType mType; // internal use, 8 4 bytes

    public:
        bool mDeleteOnCompletion; // input, c 1 byte
        bool mCompleted; // output, d 1 byte 2 bytes padding
        ES_ErrorType mResult; // output, 10 4 bytes

        PipeCmd(PipeCmdType theType, bool deleteOnCompletion);

        virtual ~PipeCmd();

        [[nodiscard]] auto GetType() const -> PipeCmdType;
    };

    ////////////////////

    class OpenCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        EasySocket::SocketType mSocketType; // input

        explicit OpenCmd(EasySocket::SocketType theSocketType, bool deleteOnCompletion = true);
    };

    ////////////////////

    class BindCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        uint16_t mPort; // input/output.  If you set mPort to 0 as input, check the output for the actual assigned port
        bool mAllowReuse; // input

        explicit BindCmd(uint16_t thePort, bool allowReuse = false, bool deleteOnCompletion = true);
    };

    ////////////////////

    class CloseCmd : public PipeCmd {
        friend class SocketPipe;

    public:
        bool mSetLinger; // input.  EasySockets don't linger by default, but set this to true to use the mLinger and mLingerSeconds args
        bool mLinger; // input
        uint16_t mLingerSeconds; // input

        explicit CloseCmd(bool setLinger = false, bool linger = false, uint16_t theLingerSeconds = 0, bool deleteOnCompletion = true);
    };

    ////////////////////

    class CloseNowCmd final : public CloseCmd {
    public:
        explicit CloseNowCmd(bool setLinger = false, bool linger = false, uint16_t theLingerSeconds = 0, bool deleteOnCompletion = true);
    };

    ////////////////////

    class ShutdownCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        int32_t mHow; // input, 0 for receives, 1 for sends, and 2 for both

        explicit ShutdownCmd(int32_t theHow = 2, bool deleteOnCompletion = true);
    };

    ////////////////////

    class ListenCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        int32_t mBacklog; // input

        explicit ListenCmd(int32_t theBacklog = 5, bool deleteOnCompletion = true);
    };

    ////////////////////

    class AcceptCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        EasySocketEngine* mSocketEngineP; // input
        bool mAddToEngine; // input
        SocketPipe* mAcceptedPipeP; // output
        SOCKADDR mSockAddr; // output

        AcceptCmd(EasySocketEngine* theSocketEngineP, bool addToEngine, bool deleteOnCompletion);

        // not setting any defaults here.  Make people make a concious decision about how to use this command
    };

    ////////////////////

    class ConnectCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        SOCKADDR mSockAddr; // input

        ConnectCmd(const SOCKADDR&theDestSockAddr, bool deleteOnCompletion);

        // not setting any defaults here.  Make people make a concious decision about how to use this command
    };

    ////////////////////

    // See the "patented BrianR" connect method in EasySocket.h to understand what this command does
    class SmartConnectCmd final : public PipeCmd {
        friend class SocketPipe;
        int32_t mAttemptIndex; // internal use
        int32_t mStartTime; // internal use

    public:
        SOCKADDR mSockAddr; // input
        int32_t mWaitTime; // input
        int32_t mNumTries; // input

        SmartConnectCmd(const SOCKADDR&theDestSockAddr, int32_t theWaitTime, int32_t theNumTries, bool deleteOnCompletion);

        // not setting any defaults here.  Make people make a concious decision about how to use this command
    };

    ////////////////////

    class BufferCmd : public PipeCmd {
    public:
        char* mBufferP; // input 14
        int32_t mBufferLength; // input 18
        bool mOwnsBuffer; // input.  will delete buffer if deleted 1c

        BufferCmd(char* theBufferP, int32_t theBufferLength, bool ownsBuffer, bool deleteOnCompletion);

        // not setting any defaults here.  Make people make a concious decision about how to use this command
        ~BufferCmd() override; // to facilitate deletion of buffers
    };

    ////////////////////

    class SendCmd : public BufferCmd {
        friend class SocketPipe;
        int32_t mSentSoFar; // internal use, 20

    public:
        SendCmd(char* theBufferP, int32_t theBufferLength, bool ownsBuffer, bool deleteOnCompletion);

        // not setting any defaults here.  Make people make a concious decision about how to use this command
    };

    ////////////////////

    class SendToCmd final : public SendCmd {
        friend class SocketPipe;

    public:
        SOCKADDR mSockAddr; // input, 24

        SendToCmd(const SOCKADDR&theDestSockAddr, char* theBufferP, int32_t theBufferLength, bool ownsBuffer, bool deleteOnCompletion);

        // not setting any defaults here.  Make people make a concious decision about how to use this command
    };

    ////////////////////

    class BroadcastCmd final : public SendCmd {
        friend class SocketPipe;
        uint16_t mPort; // 24

    public:
        BroadcastCmd(uint16_t theDestPort, char* theBufferP, int32_t theBufferLength, bool ownsBuffer, bool deleteOnCompletion);

        // not setting any defaults here.  Make people make a concious decision about how to use this command
    };

    ////////////////////

    class RecvCmd final : public BufferCmd {
        friend class SocketPipe;

    public:
        int32_t mBytesReceived; // output, 20
        int32_t mBytesToReceive; // input, 24

        RecvCmd(char* theBufferP, int32_t theBytesToReceive, bool ownsBuffer, bool deleteOnCompletion);
    };

    ////////////////////

    enum RecvLengthPrefixType {
        ptByte,
        ptUnsignedByte,
        ptShort,
        ptUnsignedShort,
        ptLong,
        ptUnsignedLong
    };

    ////////////////////

    // useful for receiving length prefixed messages
    // I wonder if there ought to be a max receivable
    // length arg

    class RecvPrefCmd final : public BufferCmd {
        friend class SocketPipe;
        char mPrefixLength; // internal use, 20
        int32_t mBytesToReceive; // 24
        bool mReceivedLength; // 28

    public:
        RecvLengthPrefixType mPrefixType; // input 2c
        int32_t mBytesReceived; // output 30

        RecvPrefCmd(bool ownsBuffer, RecvLengthPrefixType thePrefixType, bool deleteOnCompletion);
    };

    ////////////////////

    class RecvFromCmd final : public BufferCmd {
        friend class SocketPipe;

    public:
        SOCKADDR mSockAddr; // output, 20
        int32_t mBytesReceived; // output, 30

        RecvFromCmd(char* theBufferP, int32_t theBufferLength, bool ownsBuffer, bool deleteOnCompletion);
    };

    ////////////////////

    /*
    // this would probably be a useful cmd, but has not been implemented yet.
    // there might be a need for variations on this wherein the length ptr
    // is a char* or a short* or a uchar* or a ushort*

    class RecvLengthPtrCmd : public PipeCmd {
        char* mBuffer; // input/output
        long* mBytesToReceiveP; // input
        long mBytesReceived; // output
    };
    */

    ////////////////////

    using CallBackCmdPtr = void (*)(void *);

    class CallBackCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        CallBackCmdPtr mCallBackCmdPtr; // input
        void* mCallBackParamP; // input

        CallBackCmd(CallBackCmdPtr theCallBackCmdPtr, void* theCallBackParamP, bool deleteOnCompletion = true);
    };


    ////////////////////

    class SetEventCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        HANDLE mEvent; // input

        explicit SetEventCmd(HANDLE theEvent, bool deleteOnCompletion = true);
    };

    ////////////////////

    class WaitForEventCmd final : public PipeCmd {
        friend class SocketPipe;

    public:
        HANDLE mEvent; // input
        bool mResetEvent; // input

        WaitForEventCmd(HANDLE theEvent, bool resetEvent, bool deleteOnCompletion = true);
    };

    ////////////////////

    class TimerCmd final : public PipeCmd {
        friend class SocketPipe;
        uint32_t mStartTime; // internal use

    public:
        uint32_t mMillisecondsToWait; // input

        explicit TimerCmd(uint32_t theMillisecondsToWait, bool deleteOnCompletion = true);
    };

    ////////////////////

    class NoOpPayloadCmd final : public PipeCmd {
        friend class SocketPipe;
        uint32_t mPayload;

    public:
        explicit NoOpPayloadCmd(uint32_t thePayloadP, bool deleteOnCompletion = true);

        [[nodiscard]] auto GetPayload() const -> uint32_t { return mPayload; }
    };

    // Do I need a generic wait for object?
    // What about mutexes, semaphores, etc?
    // I think events are good enough to start with.
}; // end namespace WONMisc

#endif // TITAN_EASYSOCKET_PIPECMD_H
