#ifndef TITAN_EASYSOCKET_SOCKETPIPE_H
#define TITAN_EASYSOCKET_SOCKETPIPE_H

#include "PipeCmd.h"
#include <list>

namespace WONMisc {
    using PipeCmdQueue = std::list<PipeCmd *>;

    class SocketPipe final {
        PipeCmdQueue mOutCmds; // 12 bytes, 4..f
        PipeCmdQueue mInCmds; // 12 bytes, 10..1b
        CRITICAL_SECTION mOutCrit; // 24 bytes 1c..33
        CRITICAL_SECTION mInCrit; // 24 bytes 34..4b
        PipeCmdQueue::iterator mCurrOutCmd; // 4 bytes 4c..4f
        PipeCmdQueue::iterator mCurrInCmd; // 4 bytes 50..53
        int32_t mLabel; // 4 bytes 54..57

        static auto HasCompletedCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> bool;

        static auto GetNumCompletedCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> int32_t;

        static auto HasPendingCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> bool;

        static auto GetNumPendingCmds(const PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> int32_t;

        static auto RemoveCmd(PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> PipeCmd*;

        static auto RemoveCompletedCmd(PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR) -> PipeCmd*;

        static void AddCmd(PipeCmd* thePipeCmdP, PipeCmdQueue&theQueueR, CRITICAL_SECTION&theCritR);

        auto ProcessCmd(PipeCmd* thePipeCmdP) -> bool; // returns true if completed the command
        void MarkInError();

        explicit SocketPipe(EasySocket* theEasySocketP);

    public:
        EasySocket* mEasySocketP; // access with caution, 4 bytes 58..5b
        ES_ErrorType mError; // 4 bytes 5c..5f
        bool mInErrorState; // 1 byte with 3 byte padding 60
        HANDLE mCompletedPipeEvent; // 4 bytes 64..67

        SocketPipe();

        ~SocketPipe();

        void SetCompletedPipeEventH(HANDLE theCompletedPipeEventH);

        auto HasCompletedIncomingCmds() -> bool;

        auto HasCompletedOutgoingCmds() -> bool;

        auto HasCompletedCmds() -> bool;

        auto GetNumCompletedIncomingCmds() -> int32_t;

        auto GetNumCompletedOutgoingCmds() -> int32_t;

        auto HasPendingIncomingCmds() -> bool;

        auto HasPendingOutgoingCmds() -> bool;

        auto GetNumPendingIncomingCmds() -> int32_t;

        auto GetNumPendingOutgoingCmds() -> int32_t;

        auto RemoveCompletedIncomingCmd() -> PipeCmd*;

        auto RemoveCompletedOutgoingCmd() -> PipeCmd*;

        auto RemoveCompletedCmd() -> PipeCmd*;

        auto RemoveIncomingCmd() -> PipeCmd*;

        auto RemoveOutgoingCmd() -> PipeCmd*;

        void AddIncomingCmd(PipeCmd* thePipeCmdP);

        void AddOutgoingCmd(PipeCmd* thePipeCmdP);

        void Pump();

        void ClearErrorState(bool forceCompleteCurrentInCommand, bool forceCompleteCurrentOutCommand);

        [[nodiscard]] auto GetLabel() const -> int32_t;

        void SetLabel(int32_t theLabel);
    };
}; // end namespace WONMisc

#endif // TITAN_EASYSOCKET_SOCKETPIPE_H
