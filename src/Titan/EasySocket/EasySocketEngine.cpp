#include "EasySocketEngine.h"

#include <array>
#include <process.h>

namespace WONMisc {
    EasySocketEngine::EasySocketEngine(const uint32_t thePumpLoopCount, const uint32_t thePostProcessingSleepPeriod,
                                       const bool pumpAutomatically): mCrit(), mStopEvent(nullptr),
                                                                      mCompletedPipeEvent(
                                                                          CreateEventA(nullptr,TRUE,FALSE, nullptr)),
                                                                      mAbortGetCompletedPipeEvent(
                                                                          CreateEventA(nullptr, FALSE, FALSE, nullptr)),
                                                                      mThread(nullptr), mThreadID(0),
                                                                      mPumpLoopCount(thePumpLoopCount),
                                                                      mPostProcessingSleepPeriod(
                                                                          thePostProcessingSleepPeriod) {
        InitializeCriticalSection(&mCrit);
        if (pumpAutomatically) {
            mStopEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);
            const auto thread_handle = _beginthreadex(nullptr, 0, mThreadRoutine, this, CREATE_SUSPENDED,
                                                      reinterpret_cast<uint32_t *>(&mThreadID));
            mThread = reinterpret_cast<HANDLE>(thread_handle);
            if (mThread != nullptr) {
                ResumeThread(mThread);
            }
        }
    }

    EasySocketEngine::~EasySocketEngine() {
        SetEvent(mAbortGetCompletedPipeEvent);
        if (mThread != nullptr) {
            SetEvent(mStopEvent);
            WaitForSingleObject(mThread, INFINITE);
            CloseHandle(mThread);
            CloseHandle(mStopEvent);
            mThread = nullptr;
            mStopEvent = nullptr;
        }
        CloseHandle(mCompletedPipeEvent);
        CloseHandle(mAbortGetCompletedPipeEvent);
        mCompletedPipeEvent = nullptr;
        mAbortGetCompletedPipeEvent = nullptr;
        EnterCriticalSection(&mCrit);
        while (!mPipeList.empty()) {
            KillPipe(GetNthPipe(0));
        }
        DeleteCriticalSection(&mCrit);
    }

    void EasySocketEngine::PurgePipe(SocketPipe* theSocketPipeP) {
        const PipeCmd* pipe_cmd = theSocketPipeP->RemoveOutgoingCmd();
        while (pipe_cmd != nullptr) {
            delete pipe_cmd;
            pipe_cmd = theSocketPipeP->RemoveOutgoingCmd();
        }
        pipe_cmd = theSocketPipeP->RemoveIncomingCmd();
        while (pipe_cmd != nullptr) {
            delete pipe_cmd;
            pipe_cmd = theSocketPipeP->RemoveIncomingCmd();
        }
    }

    void EasySocketEngine::KillPipe(SocketPipe* theSocketPipeP) {
        RemovePipe(theSocketPipeP);
        PurgePipe(theSocketPipeP);
        delete theSocketPipeP;
    }

    auto EasySocketEngine::startWinsock() -> ES_ErrorType {
        return EasySocket::startWinsock();
    }

    auto EasySocketEngine::stopWinsock() -> ES_ErrorType {
        return EasySocket::stopWinsock();
    }

    auto EasySocketEngine::mThreadRoutine(void* param) -> uint32_t {
        auto* engine = static_cast<EasySocketEngine *>(param);
        uint32_t result;
        do {
            Pump(engine);
            result = WaitForSingleObject(engine->mStopEvent, 0);
            if (result != WAIT_OBJECT_0 && result != WAIT_FAILED) {
                Sleep(engine->mPostProcessingSleepPeriod);
            }
        }
        while (result != WAIT_OBJECT_0 && result != WAIT_FAILED);
        _endthreadex(1);
        return 1;
    }

    auto EasySocketEngine::Pump(EasySocketEngine* theEngineP) -> uint32_t {
        EnterCriticalSection(&theEngineP->mCrit);
        if (theEngineP->mPumpLoopCount > 0) {
            for (auto i = 0; i < theEngineP->mPumpLoopCount; i++) {
                for (auto* pipe: theEngineP->mPipeList) {
                    pipe->Pump();
                }
            }
        }
        LeaveCriticalSection(&theEngineP->mCrit);
        return 1;
    }

    auto EasySocketEngine::GetNumPipes() -> int32_t {
        EnterCriticalSection(&mCrit);
        const auto pipe_count = mPipeList.size();
        LeaveCriticalSection(&mCrit);
        return static_cast<int32_t>(pipe_count);
    }

    auto EasySocketEngine::GetNthPipe(const int32_t theIndex) -> SocketPipe* {
        EnterCriticalSection(&mCrit);
        SocketPipe* pipe = nullptr;
        if (theIndex + 1 <= mPipeList.size()) {
            auto iter = mPipeList.begin();
            std::advance(iter, theIndex);
            pipe = *iter;
        }
        LeaveCriticalSection(&mCrit);
        return pipe;
    }

    auto EasySocketEngine::GetErrorPipe() -> SocketPipe* {
        EnterCriticalSection(&mCrit);
        SocketPipe* error_pipe = nullptr;
        auto iter = mPipeList.begin();
        while (iter != mPipeList.end()) {
            if ((*iter)->mInErrorState) {
                error_pipe = *iter;
                mPipeList.erase(iter);
                mPipeList.push_back(error_pipe);
                break;
            }
            ++iter;
        }
        LeaveCriticalSection(&mCrit);
        return error_pipe;
    }

    auto EasySocketEngine::GetCompletedPipe(const DWORD theTimeout) -> SocketPipe* {
        ResetEvent(mCompletedPipeEvent);
        auto waitedForCompletedPipeEvent = false;
        while (true) {
            if (auto *error_pipe = GetErrorPipe(); error_pipe != nullptr) {
                return error_pipe;
            }
            EnterCriticalSection(&mCrit);
            auto iter = mPipeList.begin();
            while(iter != mPipeList.end()) {
                if ((*iter)->HasCompletedOutgoingCmds() || (*iter)->HasCompletedIncomingCmds()) {
                    auto* pipe = *iter;
                    mPipeList.erase(iter);
                    mPipeList.push_back(pipe);
                    LeaveCriticalSection(&mCrit);
                    return pipe;
                }
            }
            LeaveCriticalSection(&mCrit);
            if (waitedForCompletedPipeEvent) {
                return nullptr;
            }
            auto handles = std::array{mCompletedPipeEvent, mAbortGetCompletedPipeEvent};
            if (WaitForMultipleObjects(handles.size(), handles.data(), FALSE, theTimeout) != WAIT_OBJECT_0) {
                return nullptr;
            }
            waitedForCompletedPipeEvent = true;
        }
    }

    void EasySocketEngine::AbortGetCompletedPipe() const {
        SetEvent(mAbortGetCompletedPipeEvent);
    }

    void EasySocketEngine::AddPipe(SocketPipe* thePipeP) {
        EnterCriticalSection(&mCrit);
        thePipeP->SetCompletedPipeEventH(mCompletedPipeEvent);
        mPipeList.push_back(thePipeP);
        LeaveCriticalSection(&mCrit);
    }

    void EasySocketEngine::RemovePipe(const SocketPipe* thePipeP) {
        EnterCriticalSection(&mCrit);
        auto iter = mPipeList.begin();
        while (iter != mPipeList.end()) {
            if (*iter == thePipeP) {
                (*iter)->SetCompletedPipeEventH(nullptr);
                mPipeList.erase(iter);
                break;
            }
            ++iter;
        }
        LeaveCriticalSection(&mCrit);
    }
}
