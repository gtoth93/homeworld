#include "SocketPipe.h"
#include "EasySocketEngine.h"

namespace WONMisc {
    SocketPipe::SocketPipe(): mOutCrit(), mInCrit(), mLabel(-1), mEasySocketP(new EasySocket()), mError(ES_NO_ERROR),
                              mInErrorState(false), mCompletedPipeEvent(nullptr) {
        InitializeCriticalSection(&mOutCrit);
        InitializeCriticalSection(&mInCrit);
    }

    SocketPipe::SocketPipe(EasySocket* theEasySocketP): mOutCrit(), mInCrit(), mLabel(-1), mEasySocketP(theEasySocketP),
                                                        mError(ES_NO_ERROR), mInErrorState(false),
                                                        mCompletedPipeEvent(nullptr) {
        InitializeCriticalSection(&mOutCrit);
        InitializeCriticalSection(&mInCrit);
    }

    SocketPipe::~SocketPipe() {
        EnterCriticalSection(&mOutCrit);
        EnterCriticalSection(&mInCrit);
        while (!mOutCmds.empty()) {
            if (const auto* pipe_cmd = mOutCmds.front(); pipe_cmd != nullptr && pipe_cmd->mDeleteOnCompletion) {
                delete pipe_cmd;
            }
            mOutCmds.pop_front();
        }
        while (!mInCmds.empty()) {
            if (const auto* pipe_cmd = mInCmds.front(); pipe_cmd != nullptr && pipe_cmd->mDeleteOnCompletion) {
                delete pipe_cmd;
            }
            mInCmds.pop_front();
        }
        delete mEasySocketP;
        DeleteCriticalSection(&mOutCrit);
        DeleteCriticalSection(&mInCrit);
        mCompletedPipeEvent = nullptr;
    }

    void SocketPipe::SetLabel(const int32_t theLabel) {
        mLabel = theLabel;
    }

    auto SocketPipe::GetLabel() const -> int32_t {
        return mLabel;
    }

    void SocketPipe::SetCompletedPipeEventH(void* theCompletedPipeEventH) {
        mCompletedPipeEvent = theCompletedPipeEventH;
    }

    auto SocketPipe::HasCompletedCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> bool {
        EnterCriticalSection(&theCritR);
        bool is_completed = false;
        if (!theQueueR.empty()) {
            is_completed = theQueueR.front()->mCompleted;
        }
        LeaveCriticalSection(&theCritR);
        return is_completed;
    }

    auto SocketPipe::GetNumCompletedCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> int32_t {
        EnterCriticalSection(&theCritR);
        int32_t count = 0;
        for (const auto* pipe_cmd: theQueueR) {
            if (pipe_cmd->mCompleted) {
                count++;
            }
            else {
                break;
            }
        }
        LeaveCriticalSection(&theCritR);
        return count;
    }

    auto SocketPipe::HasPendingCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> bool {
        return GetNumPendingCmds(theQueueR, theCritR) > 0;
    }

    auto SocketPipe::GetNumPendingCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> int32_t {
        EnterCriticalSection(&theCritR);
        const auto count = static_cast<int32_t>(theQueueR.size() - GetNumCompletedCmds(theQueueR, theCritR));
        LeaveCriticalSection(&theCritR);
        return count;
    }

    auto SocketPipe::RemoveCmd(PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> PipeCmd* {
        EnterCriticalSection(&theCritR);
        PipeCmd* pipe_cmd = nullptr;
        if (!theQueueR.empty()) {
            pipe_cmd = theQueueR.front();
            theQueueR.pop_front();
        }
        LeaveCriticalSection(&theCritR);
        return pipe_cmd;
    }

    auto SocketPipe::RemoveCompletedCmd(PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> PipeCmd* {
        EnterCriticalSection(&theCritR);
        PipeCmd* pipe_cmd = nullptr;
        if (!theQueueR.empty() && HasCompletedCmds(theQueueR, theCritR)) {
            pipe_cmd = theQueueR.front();
            theQueueR.pop_front();
        }
        LeaveCriticalSection(&theCritR);
        return pipe_cmd;
    }

    void SocketPipe::AddCmd(PipeCmd* thePipeCmdP, PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) {
        EnterCriticalSection(&theCritR);
        theQueueR.push_back(thePipeCmdP);
        LeaveCriticalSection(&theCritR);
    }

    auto SocketPipe::HasCompletedIncomingCmds() -> bool {
        return HasCompletedCmds(mInCmds, mInCrit);
    }

    auto SocketPipe::HasCompletedOutgoingCmds() -> bool {
        return HasCompletedCmds(mOutCmds, mOutCrit);
    }

    auto SocketPipe::HasCompletedCmds() -> bool {
        return HasCompletedIncomingCmds() || HasCompletedOutgoingCmds();
    }

    auto SocketPipe::GetNumCompletedIncomingCmds() -> int32_t {
        return GetNumCompletedCmds(mInCmds, mInCrit);
    }

    auto SocketPipe::GetNumCompletedOutgoingCmds() -> int32_t {
        return GetNumCompletedCmds(mOutCmds, mOutCrit);
    }

    auto SocketPipe::HasPendingIncomingCmds() -> bool {
        return HasPendingCmds(mInCmds, mInCrit);
    }

    auto SocketPipe::HasPendingOutgoingCmds() -> bool {
        return HasPendingCmds(mOutCmds, mOutCrit);
    }

    auto SocketPipe::GetNumPendingIncomingCmds() -> int32_t {
        return GetNumPendingCmds(mInCmds, mInCrit);
    }

    auto SocketPipe::GetNumPendingOutgoingCmds() -> int32_t {
        return GetNumPendingCmds(mOutCmds, mOutCrit);
    }

    auto SocketPipe::RemoveCompletedIncomingCmd() -> PipeCmd* {
        return RemoveCompletedCmd(mInCmds, mInCrit);
    }

    auto SocketPipe::RemoveCompletedOutgoingCmd() -> PipeCmd* {
        return RemoveCompletedCmd(mOutCmds, mOutCrit);
    }

    auto SocketPipe::RemoveCompletedCmd() -> PipeCmd* {
        PipeCmd* pipe_cmd = RemoveCompletedOutgoingCmd();
        if (pipe_cmd == nullptr) {
            pipe_cmd = RemoveCompletedIncomingCmd();
        }
        return pipe_cmd;
    }

    auto SocketPipe::RemoveIncomingCmd() -> PipeCmd* {
        return RemoveCmd(mInCmds, mInCrit);
    }

    auto SocketPipe::RemoveOutgoingCmd() -> PipeCmd* {
        return RemoveCmd(mOutCmds, mOutCrit);
    }

    void SocketPipe::AddIncomingCmd(PipeCmd* thePipeCmdP) {
        if (thePipeCmdP->mType == pctCloseNow) {
            EnterCriticalSection(&mInCrit);
            ProcessCmd(thePipeCmdP);
            LeaveCriticalSection(&mInCrit);
            return;
        }
        AddCmd(thePipeCmdP, mInCmds, mInCrit);
    }

    void SocketPipe::AddOutgoingCmd(PipeCmd* thePipeCmdP) {
        if (thePipeCmdP->mType == pctCloseNow) {
            EnterCriticalSection(&mOutCrit);
            ProcessCmd(thePipeCmdP);
            LeaveCriticalSection(&mOutCrit);
            return;
        }
        AddCmd(thePipeCmdP, mOutCmds, mOutCrit);
    }

    void SocketPipe::Pump() {
        if (!mInErrorState) {
            EnterCriticalSection(&mOutCrit);
            if (!mOutCmds.empty()) {
                auto iter = mOutCmds.begin();
                while (iter != mOutCmds.end()) {
                    if (auto* pipe_cmd = *iter; !pipe_cmd->mCompleted) {
                        if (ProcessCmd(pipe_cmd)) {
                            pipe_cmd->mCompleted = true;
                            if (pipe_cmd->mDeleteOnCompletion) {
                                delete pipe_cmd;
                                iter = mOutCmds.erase(iter);
                            }
                            else {
                                if (mCompletedPipeEvent != nullptr) {
                                    SetEvent(mCompletedPipeEvent);
                                }
                                ++iter;
                            }
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        ++iter;
                    }
                }
            }
            LeaveCriticalSection(&mOutCrit);
        }
        if (!mInErrorState) {
            EnterCriticalSection(&mInCrit);
            if (!mInCmds.empty()) {
                auto iter = mInCmds.begin();
                while (iter != mInCmds.end()) {
                    if (auto* pipe_cmd = *iter; !pipe_cmd->mCompleted) {
                        if (ProcessCmd(pipe_cmd)) {
                            pipe_cmd->mCompleted = true;
                            if (pipe_cmd->mDeleteOnCompletion) {
                                delete pipe_cmd;
                                iter = mInCmds.erase(iter);
                            }
                            else {
                                if (mCompletedPipeEvent != nullptr) {
                                    SetEvent(mCompletedPipeEvent);
                                }
                                ++iter;
                            }
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        ++iter;
                    }
                }
            }
            LeaveCriticalSection(&mInCrit);
        }
    }

    void SocketPipe::ClearErrorState(const bool forceCompleteCurrentInCommand,
                                     const bool forceCompleteCurrentOutCommand) {
        if (forceCompleteCurrentOutCommand) {
            EnterCriticalSection(&mOutCrit);
            if (!mOutCmds.empty()) {
                PipeCmd* pipe_cmd = nullptr;
                for (auto* cmd: mOutCmds) {
                    if (!cmd->mCompleted) {
                        pipe_cmd = cmd;
                        break;
                    }
                }
                if (pipe_cmd != nullptr) {
                    if (pipe_cmd->mProcessing) {
                        pipe_cmd->mProcessing = false;
                        pipe_cmd->mCompleted = true;
                    }
                }
            }
            LeaveCriticalSection(&mOutCrit);
        }
        if (forceCompleteCurrentInCommand) {
            EnterCriticalSection(&mInCrit);
            if (!mInCmds.empty()) {
                PipeCmd* pipe_cmd = nullptr;
                for (auto* cmd: mInCmds) {
                    if (!cmd->mCompleted) {
                        pipe_cmd = cmd;
                        break;
                    }
                }
                if (pipe_cmd != nullptr) {
                    if (pipe_cmd->mProcessing) {
                        pipe_cmd->mProcessing = false;
                        pipe_cmd->mCompleted = true;
                    }
                }
            }
            LeaveCriticalSection(&mInCrit);
        }
        mInErrorState = false;
    }

    void SocketPipe::MarkInError() {
        mInErrorState = true;
        if (mCompletedPipeEvent != nullptr) {
            SetEvent(mCompletedPipeEvent);
        }
    }

    auto SocketPipe::ProcessCmd(PipeCmd* thePipeCmdP) -> bool {
        switch (thePipeCmdP->mType) {
            case pctOpen: {
                const auto* cmd = dynamic_cast<OpenCmd *>(thePipeCmdP);
                mEasySocketP->setType(cmd->mSocketType);
                return true;
            }
            case pctBind: {
                auto* cmd = dynamic_cast<BindCmd *>(thePipeCmdP);
                const auto result = mEasySocketP->bind(cmd->mPort, cmd->mAllowReuse);
                mError = result;
                cmd->mResult = result;
                if (result == ES_NO_ERROR) {
                    if (cmd->mPort == 0) {
                        cmd->mPort = mEasySocketP->getLocalPort();
                    }
                    return true;
                }
                MarkInError();
                return false;
            }
            case pctClose:
            case pctCloseNow: {
                auto* cmd = dynamic_cast<CloseCmd *>(thePipeCmdP);
                if (cmd->mSetLinger) {
                    mEasySocketP->close(cmd->mLinger, cmd->mLingerSeconds);
                }
                else {
                    mEasySocketP->close();
                }
                cmd->mResult = ES_NO_ERROR;
                return true;
            }
            case pctShutdown: {
                auto* cmd = dynamic_cast<ShutdownCmd *>(thePipeCmdP);
                const auto result = mEasySocketP->shutdown(cmd->mHow);
                mError = result;
                cmd->mResult = result;
                if (result == ES_NO_ERROR) {
                    return true;
                }
                MarkInError();
                return false;
            }
            case pctListen: {
                auto* cmd = dynamic_cast<ListenCmd *>(thePipeCmdP);
                const auto result = mEasySocketP->listen(cmd->mBacklog);
                mError = result;
                cmd->mResult = result;
                if (result == ES_NO_ERROR) {
                    return true;
                }
                MarkInError();
                return false;
            }
            case pctAccept: {
                auto* cmd = dynamic_cast<AcceptCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    cmd->mProcessing = true;
                    cmd->mAcceptedPipeP = new SocketPipe();
                }
                const auto result = mEasySocketP->accept(cmd->mAcceptedPipeP->mEasySocketP, 0);
                mError = result;
                cmd->mResult = result;
                if (result == ES_TIMED_OUT) {
                    return false;
                }
                if (result == ES_NO_ERROR) {
                    cmd->mAcceptedPipeP->mEasySocketP->getDestAddr(&cmd->mSockAddr);
                    if (cmd->mAddToEngine) {
                        cmd->mSocketEngineP->AddPipe(cmd->mAcceptedPipeP);
                    }
                    return true;
                }
                delete cmd->mAcceptedPipeP;
                cmd->mAcceptedPipeP = nullptr;
                MarkInError();
                return false;
            }
            case pctConnect: {
                auto* cmd = dynamic_cast<ConnectCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    const auto result = mEasySocketP->connect(cmd->mSockAddr, 0);
                    mError = result;
                    cmd->mResult = result;
                    if (result != ES_NO_ERROR) {
                        MarkInError();
                    }
                    cmd->mProcessing = true;
                    return false;
                }
                const auto result = mEasySocketP->checkAsynchConnect(0);
                if (result == ES_NO_ERROR) {
                    cmd->mProcessing = false;
                    return true;
                }
                if (result == ES_TIMED_OUT) {
                    return false;
                }
                mError = result;
                cmd->mResult = result;
                MarkInError();
                return false;
            }
            case pctSmartConnect: {
                auto* cmd = dynamic_cast<SmartConnectCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    const auto connect_result = mEasySocketP->connect(cmd->mSockAddr, 0);
                    mError = connect_result;
                    cmd->mResult = connect_result;
                    if (connect_result != ES_NO_ERROR) {
                        MarkInError();
                    }
                    cmd->mProcessing = true;
                    cmd->mAttemptIndex = 1;
                    cmd->mStartTime = static_cast<int32_t>(GetTickCount());
                    return false;
                }
                const auto check_result = mEasySocketP->checkAsynchConnect(0);
                if (check_result == ES_NO_ERROR) {
                    cmd->mProcessing = false;
                    return true;
                }
                if (check_result == ES_TIMED_OUT) {
                    if (const auto time = cmd->mStartTime + cmd->mWaitTime * cmd->mAttemptIndex;
                        time >= GetTickCount()) {
                        return false;
                    }
                    cmd->mAttemptIndex++;
                    if (cmd->mAttemptIndex > cmd->mNumTries) {
                        mEasySocketP->disconnect();
                        mError = check_result;
                        cmd->mResult = check_result;
                        MarkInError();
                        return false;
                    }
                    cmd->mStartTime = static_cast<int32_t>(GetTickCount());
                    const auto connect_result = mEasySocketP->connect(cmd->mSockAddr, 0);
                    mError = connect_result;
                    cmd->mResult = connect_result;
                    if (connect_result != ES_NO_ERROR) {
                        MarkInError();
                    }
                    return false;
                }
                mError = check_result;
                cmd->mResult = check_result;
                MarkInError();
                return false;
            }
            case pctSend: {
                auto* cmd = dynamic_cast<SendCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    cmd->mSentSoFar = 0;
                    cmd->mProcessing = true;
                }
                int32_t sent_len = 0;
                const auto result = mEasySocketP->sendBuffer(&cmd->mBufferP[cmd->mSentSoFar],
                                                             cmd->mBufferLength - cmd->mSentSoFar, &sent_len, 0);
                mError = result;
                cmd->mResult = result;
                cmd->mSentSoFar += sent_len;
                if (result == ES_NO_ERROR) {
                    cmd->mProcessing = false;
                    return true;
                }
                if (result == ES_INCOMPLETE_SEND) {
                    return false;
                }
                MarkInError();
                return false;
            }
            case pctSendTo: {
                auto* cmd = dynamic_cast<SendToCmd *>(thePipeCmdP);
                const auto result = mEasySocketP->sendBufferTo(cmd->mBufferP, cmd->mBufferLength, cmd->mSockAddr, 0);
                mError = result;
                cmd->mResult = result;
                if (result == ES_NO_ERROR || result == ES_PARTIAL_SENDTO || result == ES_WSAENOBUFS) {
                    return true;
                }
                MarkInError();
                return false;
            }
            case pctBroadcast: {
                auto* cmd = dynamic_cast<BroadcastCmd *>(thePipeCmdP);
                const auto result = mEasySocketP->broadcastBuffer(cmd->mBufferP, cmd->mBufferLength, cmd->mPort, 0);
                mError = result;
                cmd->mResult = result;
                if (result == ES_NO_ERROR || result == ES_PARTIAL_SENDTO || result == ES_WSAENOBUFS) {
                    return true;
                }
                MarkInError();
                return false;
            }
            case pctRecv: {
                auto* cmd = dynamic_cast<RecvCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    cmd->mBytesReceived = 0;
                    cmd->mProcessing = true;
                }
                int32_t recv_len = 0;
                const auto result = mEasySocketP->recvBuffer(&cmd->mBufferP[cmd->mBytesReceived],
                                                             cmd->mBytesToReceive - cmd->mBytesReceived, &recv_len, 0);
                mError = result;
                cmd->mResult = result;
                cmd->mBytesReceived += recv_len;
                if (result == ES_NO_ERROR) {
                    cmd->mProcessing = false;
                    return true;
                }
                if (result == ES_TIMED_OUT) {
                    return false;
                }
                if (result == ES_INCOMPLETE_RECV) {
                    if (recv_len <= 0) {
                        MarkInError();
                    }
                    return false;
                }
                MarkInError();
                return false;
            }
            case pctRecvPrefixed: {
                auto* cmd = dynamic_cast<RecvPrefCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    cmd->mProcessing = true;
                }
                const auto len =
                        (cmd->mReceivedLength ? cmd->mBytesToReceive : static_cast<uint8_t>(cmd->mPrefixLength)) - cmd->
                        mBytesReceived;
                int32_t recv_len = 0;
                const auto result = mEasySocketP->recvBuffer(cmd->mBufferP, len, &recv_len, 0);
                mError = result;
                cmd->mResult = result;
                cmd->mBytesReceived += recv_len;
                if (result == ES_NO_ERROR) {
                    if (cmd->mReceivedLength) {
                        cmd->mProcessing = false;
                        return true;
                    }
                    switch (cmd->mPrefixType) {
                        case ptByte:
                        case ptUnsignedByte: {
                            cmd->mBytesToReceive = static_cast<uint8_t>(cmd->mBufferP[0]);
                            break;
                        }
                        case ptShort:
                            cmd->mBytesToReceive = *reinterpret_cast<int16_t *>(cmd->mBufferP);
                            break;
                        case ptUnsignedShort:
                            cmd->mBytesToReceive = *reinterpret_cast<uint16_t *>(cmd->mBufferP);
                            break;
                        case ptLong:
                            cmd->mBytesToReceive = *reinterpret_cast<int32_t *>(cmd->mBufferP);
                            break;
                        case ptUnsignedLong:
                            cmd->mBytesToReceive = static_cast<int32_t>(*reinterpret_cast<uint32_t *>(cmd->mBufferP));
                            break;
                    }
                    cmd->mBytesToReceive -= cmd->mPrefixLength;
                    if (cmd->mBytesToReceive < 0 || cmd->mBytesToReceive > 128000) {
                        mError = ES_TMSG_TOO_LARGE;
                        MarkInError();
                        cmd->mProcessing = false;
                        return false;
                    }
                    auto* new_buf = new char[cmd->mBytesToReceive];
                    delete[] cmd->mBufferP;
                    cmd->mBufferP = new_buf;
                    cmd->mBufferLength = cmd->mBytesToReceive;
                    cmd->mBytesReceived = 0;
                    cmd->mReceivedLength = true;
                    return false;
                }
                if (result == ES_TIMED_OUT) {
                    return false;
                }
                if (result == ES_INCOMPLETE_RECV) {
                    if (recv_len <= 0) {
                        MarkInError();
                    }
                    return false;
                }
                MarkInError();
                return false;
            }
            case pctRecvFrom: {
                auto* cmd = dynamic_cast<RecvFromCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    cmd->mProcessing = true;
                }
                const auto result = mEasySocketP->recvBufferFrom(cmd->mBufferP, cmd->mBufferLength, &cmd->mSockAddr,
                                                                 &cmd->mBytesReceived, 0);
                mError = result;
                cmd->mResult = result;
                if (result == ES_TIMED_OUT) {
                    return false;
                }
                if (result == ES_NO_ERROR) {
                    cmd->mProcessing = false;
                    return true;
                }
                MarkInError();
                return false;
            }
            case pctCallBack: {
                const auto* cmd = dynamic_cast<CallBackCmd *>(thePipeCmdP);
                cmd->mCallBackCmdPtr(cmd->mCallBackParamP);
                return true;
            }
            case pctSetEvent: {
                const auto* cmd = dynamic_cast<SetEventCmd *>(thePipeCmdP);
                SetEvent(cmd->mEvent);
                return true;
            }
            case pctWaitForEvent: {
                const auto* cmd = dynamic_cast<WaitForEventCmd *>(thePipeCmdP);
                const auto result = WaitForSingleObject(cmd->mEvent, 0);
                if (result == WAIT_OBJECT_0) {
                    if (cmd->mResetEvent) {
                        ResetEvent(cmd->mEvent);
                    }
                    return true;
                }
                if (result == WAIT_TIMEOUT) {
                    return false;
                }
                MarkInError();
                mError = ES_ERROR_NO_TYPE;
                return false;
            }
            case pctTimer: {
                auto* cmd = dynamic_cast<TimerCmd *>(thePipeCmdP);
                if (!cmd->mProcessing) {
                    cmd->mStartTime = GetTickCount();
                    cmd->mProcessing = true;
                    return false;
                }
                return GetTickCount() > cmd->mStartTime + cmd->mMillisecondsToWait;
            }
            default: {
                return true;
            }
        }
    }
}
