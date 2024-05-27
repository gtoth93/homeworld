#ifndef TITAN_EASYSOCKET_EASYSOCKETENGINE_H
#define TITAN_EASYSOCKET_EASYSOCKETENGINE_H

#include "EasySocket.h"
#include "SocketPipe.h"

namespace WONMisc {
    using PipeList = std::list<SocketPipe *>;

    class EasySocketEngine final {
        PipeList mPipeList; // 12 bytes 4..f
        CRITICAL_SECTION mCrit; // 24 bytes 10..27

        static auto __stdcall mThreadRoutine(void* param) -> uint32_t;

        HANDLE mStopEvent; // 4 bytes 28..2b
        HANDLE mCompletedPipeEvent; // 4 bytes 2c..2f
        HANDLE mAbortGetCompletedPipeEvent; // 4 bytes 30..33
        HANDLE mThread; // 4 bytes 34..37
        int32_t mThreadID; // 4 bytes 38..3b
        uint32_t mPumpLoopCount; // 4 bytes 3c..3f
        // times that Pump will go around before exitting if there are commands left to be processed
        uint32_t mPostProcessingSleepPeriod; // time to sleep after pumping in mThreadRoutine, 4 bytes 40..43

    public:
        explicit EasySocketEngine(uint32_t thePumpLoopCount = 1, uint32_t thePostProcessingSleepPeriod = 20,
                                  bool pumpAutomatically = true);

        ~EasySocketEngine();

        static auto startWinsock() -> ES_ErrorType;

        static auto stopWinsock() -> ES_ErrorType;

        auto GetNumPipes() -> int32_t;

        auto GetNthPipe(int32_t theIndex) -> SocketPipe*; // zero based index
        auto GetErrorPipe() -> SocketPipe*;

        auto GetCompletedPipe(DWORD theTimeout = INFINITE) -> SocketPipe*;

        void AbortGetCompletedPipe() const;

        void AddPipe(SocketPipe* thePipeP);

        void RemovePipe(const SocketPipe* thePipeP);

        static void PurgePipe(SocketPipe* theSocketPipeP);

        void KillPipe(SocketPipe* theSocketPipeP);

        static auto Pump(EasySocketEngine* theEngineP) -> uint32_t;
    };
}; // end namespace WONMisc

#endif // TITAN_EASYSOCKET_EASYSOCKETENGINE_H
