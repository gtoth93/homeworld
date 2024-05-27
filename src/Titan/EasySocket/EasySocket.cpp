

#include "EasySocket.h"
#include <algorithm>
#include <array>
#include <bit>
#include <format>

namespace WONMisc {
    InitWinsock winsock{};

    EasySocket::EasySocket(): mSocket(INVALID_SOCKET), mType(NO_TYPE), mDestAddr(), mConnected(false) {
    }

    EasySocket::EasySocket(const SocketType theType): mSocket(INVALID_SOCKET), mType(theType), mDestAddr(),
                                                      mConnected(false) {
    }

    EasySocket::~EasySocket() {
        close();
    }

    auto EasySocket::ESGetLastError() -> int32_t {
        return WSAGetLastError();
    }

    auto EasySocket::GetTickCount() -> uint32_t {
        return ::GetTickCount();
    }

    auto EasySocket::isInvalid() const -> bool {
        return mSocket == INVALID_SOCKET;
    }

    void EasySocket::getDestAddr(SOCKADDR* theSockAddr) const {
        if (isInvalid() || !mConnected) {
            theSockAddr->sa_family = AF_UNSPEC;
            std::ranges::fill(theSockAddr->sa_data, 0);
            return;
        }
        *theSockAddr = mDestAddr;
    }

    auto EasySocket::getDestAddr() -> int32_t {
        if (isInvalid() || !mConnected || mType == IPX || mType == SPX) {
            return 0;
        }
        const auto* sa_in = reinterpret_cast<sockaddr_in *>(&mDestAddr);
        return static_cast<int32_t>(sa_in->sin_addr.S_un.S_addr);
    }

#ifdef HAVE_IPX
    void EasySocket::getDestAddr(std::array<uint8_t, 6>&theAddr) {
        if (isInvalid() || !mConnected || mType == TCP || mType == UDP) {
            theAddr.fill(0);
        }
        else {
            auto* sa_ipx = reinterpret_cast<sockaddr_ipx *>(&mDestAddr);
            std::ranges::copy(sa_ipx->sa_nodenum, theAddr.begin());
        }
    }
#endif // HAVE_IPX

    auto EasySocket::getDestAddrString() -> std::string {
        if (isInvalid() || !mConnected) {
            return {};
        }
        if (mType == IPX || mType == SPX) {
            std::array<uint8_t, 6> node_num{};
            auto* sa_ipx = reinterpret_cast<sockaddr_ipx *>(&mDestAddr);
            std::ranges::copy(sa_ipx->sa_nodenum, node_num.begin());
            return getAddrFromNodeNum(node_num);
        }
        const auto* sa_in = reinterpret_cast<sockaddr_in *>(&mDestAddr);
        return inet_ntoa(sa_in->sin_addr);
    }

    auto EasySocket::getDestPort() -> int32_t {
        if (isInvalid() || !mConnected) {
            return 0;
        }
        if (mType == IPX || mType == SPX) {
            const auto* sa_ipx = reinterpret_cast<sockaddr_ipx *>(&mDestAddr);
            return ntohs(sa_ipx->sa_socket);
        }
        const auto* sa_in = reinterpret_cast<sockaddr_in *>(&mDestAddr);
        return ntohs(sa_in->sin_port);
    }

    auto EasySocket::getLocalAddr() const -> int32_t {
        if (mType == IPX || mType == SPX) {
            return 0;
        }
        return getAddrFromString(getLocalAddrString());
    }

#ifdef HAVE_IPX
    void EasySocket::getLocalAddr(std::array<uint8_t, 6>&theAddr) const {
        if (mType == TCP || mType == UDP) {
            theAddr.fill(0);
        }
        else {
            GUID guid;
            if (const HRESULT result = CoCreateGuid(&guid); result == 0) {
                std::copy_n(&guid.Data4[2], theAddr.size(), theAddr.begin());
            }
            else {
                theAddr.fill(0);
            }
        }
    }
#endif // HAVE_IPX

    auto EasySocket::getLocalAddrString() const -> std::string {
        if (mType == IPX || mType == SPX) {
            std::array<uint8_t, 6> addr{};
            getLocalAddr(addr);
            return getAddrFromNodeNum(addr);
        }
        std::array<char, 255> buf{};
        if (const int32_t result = gethostname(buf.data(), buf.size()); result == 0) {
            return {buf.data(), buf.size()};
        }
        return {};
    }

    auto EasySocket::getLocalPort() -> int32_t {
        if (isInvalid()) {
            return 0;
        }
        sockaddr name = {};
        int32_t nameLen = sizeof(sockaddr);
        if (const int32_t result = getsockname(mSocket, &name, &nameLen); result != 0) {
            return 0;
        }
        if (mType == IPX || mType == SPX) {
            const auto* sa_ipx = reinterpret_cast<sockaddr_ipx *>(&mDestAddr);
            return ntohs(sa_ipx->sa_socket);
        }
        const auto* sa_in = reinterpret_cast<sockaddr_in *>(&mDestAddr);
        return ntohs(sa_in->sin_port);
    }

    auto EasySocket::startWinsock() -> ES_ErrorType {
        WSADATA data{};
        if (const int32_t result = WSAStartup(MAKEWORD(1, 1), &data); result == 0) {
            return ES_NO_ERROR;
        }
        return WSAErrorToEnum(WSAGetLastError());
    }

    auto EasySocket::stopWinsock() -> ES_ErrorType {
        if (const int32_t result = WSACleanup(); result == 0) {
            return ES_NO_ERROR;
        }
        return WSAErrorToEnum(WSAGetLastError());
    }

    auto EasySocket::getSockAddrIn(SOCKADDR_IN&theSockAddr, const std::string&theAddress,
                                   const int32_t thePort) -> ES_ErrorType {
        const int32_t addr = getAddrFromString(theAddress);
        if (addr == 0) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        theSockAddr.sin_family = AF_INET;
        theSockAddr.sin_port = htons(thePort);
        theSockAddr.sin_addr = {{.S_addr = static_cast<uint32_t>(addr)}};
        std::fill_n(theSockAddr.sin_zero, 8, 0);
        return ES_NO_ERROR;
    }

    auto EasySocket::getSockAddrIn(SOCKADDR_IN&theSockAddr, const std::string&theAddressAndPort) -> ES_ErrorType {
        if (const size_t pos = theAddressAndPort.find(':'); pos != std::string::npos) {
            const int32_t port = stoi(theAddressAndPort.substr(pos + 1));
            const std::string address = theAddressAndPort.substr(0, pos);
            return getSockAddrIn(theSockAddr, address, port);
        }
        return ES_INVALID_ADDR;
    }

    auto EasySocket::getSockAddrInFast(SOCKADDR_IN&theSockAddr, const std::string&theAddress,
                                       const int32_t thePort) -> ES_ErrorType {
        const uint32_t addr = inet_addr(theAddress.data());
        if (addr == 0) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        theSockAddr.sin_family = AF_INET;
        theSockAddr.sin_port = htons(thePort);
        theSockAddr.sin_addr = {{.S_addr = addr}};
        std::fill_n(theSockAddr.sin_zero, 8, 0);
        return ES_NO_ERROR;
    }

    auto EasySocket::getSockAddrInFast(SOCKADDR_IN&theSockAddr, const std::string&theAddressAndPort) -> ES_ErrorType {
        if (const size_t pos = theAddressAndPort.find(':'); pos != std::string::npos) {
            const int32_t port = stoi(theAddressAndPort.substr(pos + 1));
            const std::string address = theAddressAndPort.substr(0, pos);
            return getSockAddrInFast(theSockAddr, address, port);
        }
        return ES_INVALID_ADDR;
    }

#ifdef HAVE_IPX
    auto EasySocket::getSockAddrIpx(SOCKADDR_IPX&theSockAddr, std::array<uint8_t, 6>&theAddress,
                                    const int32_t thePort) -> ES_ErrorType {
        theSockAddr.sa_family = AF_IPX;
        std::fill_n(theSockAddr.sa_netnum, 4, 0);
        std::ranges::copy(theAddress, theSockAddr.sa_nodenum);
        theSockAddr.sa_socket = htons(thePort);

        return ES_NO_ERROR;
    }
#endif // HAVE_IPX

    void EasySocket::getBroadcastSockAddrIn(SOCKADDR_IN&theSockAddr, const int32_t thePort) {
        theSockAddr.sin_family = AF_INET;
        theSockAddr.sin_port = htons(thePort);
        theSockAddr.sin_addr = {{.S_addr = INADDR_BROADCAST}};
        std::fill_n(theSockAddr.sin_zero, 8, 0);
    }

#ifdef HAVE_IPX
    void EasySocket::getBroadcastSockAddrIpx(SOCKADDR_IPX&theSockAddr, const int32_t thePort) {
        theSockAddr.sa_family = AF_IPX;
        std::fill_n(theSockAddr.sa_netnum, 4, 0);
        std::fill_n(theSockAddr.sa_nodenum, 6, 0xFF);
        theSockAddr.sa_socket = htons(thePort);
    }
#endif HAVE_IPX

    auto EasySocket::getAddrFromString(const std::string&theAddress) -> int32_t {
        if (const uint32_t addr = inet_addr(theAddress.data()); addr != INADDR_NONE) {
            return static_cast<int32_t>(addr);
        }
        const hostent* host = gethostbyname(theAddress.data());
        if (host == nullptr) {
            return 0;
        }
        std::array<char, 4> buf{};
        std::copy_n(host->h_addr_list[0], buf.size(), buf.begin());
        return std::bit_cast<int32_t>(buf);
    }

#ifdef HAVE_IPX
    auto EasySocket::getAddrFromNodeNum(std::array<uint8_t, 6>&theNodeNum) -> std::string {
        return std::format(
            "{:X}{:X}{:X}{:X}{:X}{:X}",
            theNodeNum[0],
            theNodeNum[1],
            theNodeNum[2],
            theNodeNum[3],
            theNodeNum[4],
            theNodeNum[5]
        );
    }
#endif // HAVE_IPX

    auto EasySocket::getAddrFromLong(const int32_t theAddress) -> std::string {
        return inet_ntoa({{.S_addr = static_cast<uint32_t>(theAddress)}});
    }

    auto EasySocket::getType() const -> SocketType {
        return mType;
    }

    void EasySocket::setType(const SocketType theType) {
        if (mType != theType) {
            close();
        }
        mType = theType;
    }

    auto EasySocket::getNewDescriptor() -> ES_ErrorType {
        if (!isInvalid()) {
            close();
        }
        switch (mType) {
            case NO_TYPE:
                return ES_ERROR_NO_TYPE;
            case TCP:
                mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                break;
            case UDP:
                mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                break;
            case IPX:
                mSocket = socket(AF_IPX, SOCK_DGRAM, NSPROTO_IPX);
                break;
            case SPX:
                mSocket = socket(AF_IPX, SOCK_STREAM, NSPROTO_SPX);
                break;
            default:
                break;
        }
        if (isInvalid()) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        if (mType == IPX || mType == SPX) {
            const auto value = std::bit_cast<std::array<char, 4>>(0x0000FFFF);
            if (const int32_t result = setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, value.data(), value.size());
                result == SOCKET_ERROR) {
                return WSAErrorToEnum(WSAGetLastError());
            }
            if (const int32_t result = setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, value.data(), value.size());
                result == SOCKET_ERROR) {
                return WSAErrorToEnum(WSAGetLastError());
            }
        }
        if (mType == TCP || mType == SPX) {
            const auto value = std::bit_cast<std::array<char, 4>>(linger{0, 0});
            if (const int32_t result = setsockopt(mSocket, SOL_SOCKET, SO_LINGER, value.data(), value.size());
                result == SOCKET_ERROR) {
                return WSAErrorToEnum(WSAGetLastError());
            }
        }
        if (mType == UDP || mType == IPX) {
            const auto value = std::bit_cast<std::array<char, 4>>(TRUE);
            if (const int32_t result = setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, value.data(), value.size());
                result == SOCKET_ERROR) {
                return WSAErrorToEnum(WSAGetLastError());
            }
        }
        u_long mode = 1;
        if (const int32_t result = ioctlsocket(mSocket, FIONBIO, &mode); result == SOCKET_ERROR) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        return ES_NO_ERROR;
    }

    auto EasySocket::bind(const int32_t thePort, const bool allowReuse) -> ES_ErrorType {
        if (isInvalid()) {
            if (const ES_ErrorType result = getNewDescriptor(); result != ES_NO_ERROR) {
                return result;
            }
        }
        if (allowReuse) {
            const auto value = std::bit_cast<std::array<char, 4>>(1);
            if (const int32_t result = setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, value.data(), value.size());
                result == SOCKET_ERROR) {
                return WSAErrorToEnum(WSAGetLastError());
            }
        }
        sockaddr addr{};
        if (mType == TCP || mType == UDP) {
            auto* addr_in = reinterpret_cast<sockaddr_in *>(&addr);
            addr_in->sin_family = AF_INET;
            addr_in->sin_port = htons(thePort);
            addr_in->sin_addr = {{.S_addr = htonl(INADDR_ANY)}};
        }
        else {
            auto* addr_ipx = reinterpret_cast<sockaddr_ipx *>(&addr);
            addr_ipx->sa_family = AF_IPX;
            addr_ipx->sa_socket = htons(thePort);
        }
        if (const int32_t result = ::bind(mSocket, &addr, sizeof(sockaddr)); result == SOCKET_ERROR) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        return ES_NO_ERROR;
    }

    auto EasySocket::connect(const SOCKADDR&theSockAddr, const int32_t theWaitTime) -> ES_ErrorType {
        if (mType == TCP || mType == SPX) {
            disconnect();
        }
        if (isInvalid()) {
            if (const ES_ErrorType result = getNewDescriptor(); result != ES_NO_ERROR) {
                return result;
            }
        }
        if (const int32_t result = ::connect(mSocket, &theSockAddr, sizeof(sockaddr)); result == SOCKET_ERROR) {
            if (const ES_ErrorType error = WSAErrorToEnum(WSAGetLastError()); error != ES_WSAEWOULDBLOCK) {
                return error;
            }
        }
        mConnected = true;
        mDestAddr = theSockAddr;
        if (theWaitTime == 0) {
            return ES_NO_ERROR;
        }
        if (mType == TCP || mType == SPX) {
            return checkAsynchConnect(theWaitTime);
        }
        return ES_NO_ERROR;
    }

    auto EasySocket::connect(const SOCKADDR&theSockAddr, const int32_t theWaitTime,
                             const int32_t theNumTries) -> ES_ErrorType {
        if (theNumTries <= 0) {
            return static_cast<ES_ErrorType>(theNumTries); // ???
        }
        int32_t tries = 0;
        int32_t wait_time = theWaitTime;
        ES_ErrorType result;
        do {
            result = connect(theSockAddr, theWaitTime);
            if (result == ES_NO_ERROR) {
                return ES_NO_ERROR;
            }
            tries++;
            wait_time *= 2;
        }
        while (tries < theNumTries);
        return result;
    }

    auto EasySocket::connect(const int32_t theAddress, const int32_t thePort, const int32_t theWaitTime,
                             const int32_t theNumTries) -> ES_ErrorType {
        if (mType == IPX || mType == SPX) {
            return ES_INVALID_ADDR;
        }
        sockaddr_in sa{AF_INET, htons(thePort), {{.S_addr = static_cast<uint32_t>(theAddress)}}};
        return connect(*reinterpret_cast<sockaddr *>(&sa), theWaitTime, theNumTries);
    }

#ifdef HAVE_IPX
    auto EasySocket::connect(std::array<uint8_t, 6>&theAddress, const int32_t thePort, const int32_t theWaitTime,
                             const int32_t theNumTries) -> ES_ErrorType {
        if (mType == TCP || mType == UDP) {
            return ES_INVALID_ADDR;
        }
        sockaddr sa{};
        auto* sa_ipx = reinterpret_cast<sockaddr_ipx *>(&sa);
        sa_ipx->sa_family = AF_IPX;
        std::ranges::copy(theAddress, sa_ipx->sa_nodenum);
        sa_ipx->sa_socket = htons(thePort);
        return connect(sa, theWaitTime, theNumTries);
    }
#endif // HAVE_IPX

    auto EasySocket::connect(const std::string&theAddress, const int32_t thePort, const int32_t theWaitTime,
                             const int32_t theNumTries) -> ES_ErrorType {
        if (mType == IPX || mType == SPX) {
            return ES_INVALID_ADDR;
        }
        sockaddr_in sa_in{};
        if (const ES_ErrorType result = getSockAddrIn(sa_in, theAddress, thePort); result != ES_NO_ERROR) {
            return result;
        }

        return connect(*reinterpret_cast<sockaddr *>(&sa_in), theWaitTime, theNumTries);
    }

    auto EasySocket::listen(const int32_t theBacklog) const -> ES_ErrorType {
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }
        if (mType == UDP || mType == IPX) {
            return ES_ERROR_DATAGRAM_NOT_ALLOWED;
        }
        if (const int32_t result = ::listen(mSocket, theBacklog); result == SOCKET_ERROR) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        return ES_NO_ERROR;
    }

    auto EasySocket::accept(EasySocket* theEasySocket, const int32_t theWaitTime) const -> ES_ErrorType {
        if (!waitForRead(theWaitTime)) {
            return ES_TIMED_OUT;
        }
        sockaddr sa{};
        int32_t len = sizeof(sockaddr);
        const SOCKET socket = ::accept(mSocket, &sa, &len);
        if (socket == INVALID_SOCKET) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        theEasySocket->close();
        theEasySocket->mSocket = socket;
        theEasySocket->mType = this->mType;
        theEasySocket->mConnected = true;
        theEasySocket->mDestAddr = sa;
        return ES_NO_ERROR;
    }

    auto EasySocket::checkAsynchConnect(const int32_t theWaitTime) const -> ES_ErrorType {
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }
        int32_t wait_time = theWaitTime;
        if (wait_time < 0) {
            wait_time = 0;
        }
        fd_set writefds{1, {mSocket}};
        fd_set exceptfds{1, {mSocket}};
        const timeval timeout{.tv_sec = wait_time / 1000, .tv_usec = wait_time % 1000 * 1000};
        const int32_t result = select(FD_SETSIZE, nullptr, &writefds, &exceptfds, &timeout);
        if (const int32_t is_error = __WSAFDIsSet(mSocket, &exceptfds); is_error != 0) {
            Sleep(1);
            std::array<char, 4> value_bytes{};
            int32_t size = value_bytes.size();
            getsockopt(mSocket, SOL_SOCKET, SO_ERROR, value_bytes.data(), &size);
            const auto value = std::bit_cast<int32_t>(value_bytes);
            return WSAErrorToEnum(value == 0 ? static_cast<int32_t>(ES_UNKNOWN_ERROR) : value);
        }
        return result == 0 ? ES_TIMED_OUT : ES_NO_ERROR;
    }

    auto EasySocket::waitForAccept(const int32_t theWaitTime) const -> bool {
        return waitForRead(theWaitTime);
    }

    auto EasySocket::waitForWrite(const int32_t theWaitTime) const -> bool {
        if (isInvalid()) {
            return false;
        }
        int32_t wait_time = theWaitTime;
        if (wait_time < 0) {
            wait_time = 0;
        }
        const timeval timeout{.tv_sec = wait_time / 1000, .tv_usec = wait_time % 1000 * 1000};
        fd_set writefds{1, {mSocket}};
        const int32_t result = select(FD_SETSIZE, nullptr, &writefds, nullptr, &timeout);
        return result != 0;
    }

    auto EasySocket::waitForRead(const int32_t theWaitTime) const -> bool {
        if (isInvalid()) {
            return false;
        }
        int32_t wait_time = theWaitTime;
        if (wait_time < 0) {
            wait_time = 0;
        }
        const timeval timeout{wait_time / 1000, wait_time % 1000 * 1000};
        fd_set readfds{1, {mSocket}};
        const int32_t result = select(FD_SETSIZE, &readfds, nullptr, nullptr, &timeout);
        return result != 0;
    }

    auto EasySocket::sendBuffer(const void* theBuf, const int32_t theLen, int32_t* theSentLen,
                                const int32_t theTotalTime) const -> ES_ErrorType {
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }
        const uint32_t start = GetTickCount();
        bool timeout = false;
        int32_t i = 0;
        while (i < theLen) {
            const uint32_t elapsed_time = GetTickCount() - start;
            int32_t remaining_time;
            if (elapsed_time < theTotalTime) {
                remaining_time = theTotalTime - static_cast<int32_t>(elapsed_time);
            }
            else {
                remaining_time = 0;
                timeout = true;
            }
            if (waitForWrite(remaining_time)) {
                const int32_t result = send(mSocket, &static_cast<const char *>(theBuf)[i], theLen - i, 0);
                if (result < 0) {
                    if (const int32_t last_error = WSAGetLastError(); last_error != WSAEWOULDBLOCK) {
                        return WSAErrorToEnum(last_error);
                    }
                }
                if (result > 0) {
                    i += result;
                }
            }
            if (timeout) {
                break;
            }
        }
        if (theSentLen != nullptr) {
            *theSentLen = i;
        }
        return i < theLen ? ES_INCOMPLETE_SEND : ES_NO_ERROR;
    }

    auto EasySocket::recvBuffer(void* theBuf, const int32_t theLen, int32_t* theRecvLen,
                                const int32_t theTotalTime) const -> ES_ErrorType {
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }
        const uint32_t start = GetTickCount();
        bool timeout = false;
        bool partialSuccess = false;
        int32_t i = 0;
        while (i < theLen) {
            const uint32_t elapsed_time = GetTickCount() - start;
            int32_t remaining_time;
            if (elapsed_time < theTotalTime) {
                remaining_time = theTotalTime - static_cast<int32_t>(elapsed_time);
            }
            else {
                remaining_time = 0;
                timeout = true;
            }
            if (waitForRead(remaining_time)) {
                const int32_t result = recv(mSocket, &static_cast<char *>(theBuf)[i], theLen - i, 0);
                if (result < 0) {
                    return WSAErrorToEnum(WSAGetLastError());
                }
                if (result == 0) {
                    if (!partialSuccess && (mType == TCP || mType == SPX)) {
                        return ES_SHUTDOWN;
                    }
                    timeout = true;
                }
                i += result;
                partialSuccess = true;
                if (mType == UDP || mType == IPX) {
                    break;
                }
            }
            if (timeout) {
                break;
            }
        }
        if (theRecvLen != nullptr) {
            *theRecvLen = i;
        }
        if (!partialSuccess) {
            return ES_TIMED_OUT;
        }
        if ((mType == TCP || mType == SPX) && i != theLen) {
            return ES_INCOMPLETE_RECV;
        }
        return ES_NO_ERROR;
    }

    auto EasySocket::broadcastBuffer(const void* theBuf, const int32_t theLen, const int32_t thePort,
                                     const int32_t theTotalTime) -> ES_ErrorType {
        if (mType == TCP || mType == SPX) {
            return ES_ERROR_STREAM_NOT_ALLOWED;
        }
        sockaddr addr{};
        if (mType == UDP) {
            getBroadcastSockAddrIn(*reinterpret_cast<sockaddr_in *>(&addr), thePort);
        }
        else {
            getBroadcastSockAddrIpx(*reinterpret_cast<sockaddr_ipx *>(&addr), thePort);
        }
        return sendBufferTo(theBuf, theLen, addr, theTotalTime);
    }

    auto EasySocket::sendBufferTo(const void* theBuf, const int32_t theLen, const SOCKADDR&theSockAddr,
                                  const int32_t theTotalTime) -> ES_ErrorType {
        if (mType == TCP || mType == SPX) {
            return ES_ERROR_STREAM_NOT_ALLOWED;
        }
        if (isInvalid()) {
            if (const ES_ErrorType result = getNewDescriptor(); result != ES_NO_ERROR) {
                return result;
            }
        }
        waitForWrite(theTotalTime);
        const int32_t sent_len = sendto(mSocket, static_cast<const char *>(theBuf), theLen, 0, &theSockAddr,
                                        sizeof(sockaddr));
        if (sent_len == theLen) {
            return ES_NO_ERROR;
        }
        if (sent_len < 0) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        return ES_PARTIAL_SENDTO;
    }

    auto EasySocket::sendBufferTo(const void* theBuf, const int32_t theLen, const int32_t theAddr,
                                  const int32_t thePort, const int32_t theTotalTime) -> ES_ErrorType {
        if (mType == IPX || mType == SPX) {
            return ES_INVALID_ADDR;
        }
        sockaddr_in addr{AF_INET, htons(thePort), {{.S_addr = static_cast<uint32_t>(theAddr)}}};
        return sendBufferTo(theBuf, theLen, *reinterpret_cast<sockaddr *>(&addr), theTotalTime);
    }

#ifdef HAVE_IPX
    auto EasySocket::sendBufferTo(const void* theBuf, const int32_t theLen, std::array<uint8_t, 6>&theAddr,
                                  const int32_t thePort, const int32_t theTotalTime) -> ES_ErrorType {
        if (mType == TCP || mType == UDP) {
            return ES_INVALID_ADDR;
        }
        sockaddr addr{};
        auto* addr_ipx = reinterpret_cast<sockaddr_ipx *>(&addr);
        addr_ipx->sa_family = AF_IPX;
        std::ranges::copy(theAddr, addr_ipx->sa_nodenum);
        addr_ipx->sa_socket = htons(thePort);

        return sendBufferTo(theBuf, theLen, addr, theTotalTime);
    }
#endif // HAVE_IPX

    auto EasySocket::sendBufferTo(const void* theBuf, const int32_t theLen, const std::string&theAddr,
                                  const int32_t thePort,
                                  const int32_t theTotalTime) -> ES_ErrorType {
        if (mType == IPX || mType == SPX) {
            return ES_INVALID_ADDR;
        }
        sockaddr_in sa_in{};
        if (const ES_ErrorType result = getSockAddrIn(sa_in, theAddr, thePort); result != ES_NO_ERROR) {
            return result;
        }
        return sendBufferTo(theBuf, theLen, *reinterpret_cast<sockaddr *>(&sa_in), theTotalTime);
    }

    auto EasySocket::recvBufferFrom(void* theBuf, const int32_t theLen, SOCKADDR* theSockAddr, int32_t* theRecvLen,
                                    const int32_t theTotalTime) const -> ES_ErrorType {
        if (mType == TCP || mType == SPX) {
            return ES_ERROR_STREAM_NOT_ALLOWED;
        }
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }
        if (!waitForRead(theTotalTime)) {
            return ES_TIMED_OUT;
        }

        sockaddr sa{};
        int32_t len = sizeof(sockaddr);

        const int32_t result = recvfrom(mSocket, static_cast<char *>(theBuf), theLen, 0,
                                        theSockAddr == nullptr ? &sa : theSockAddr, &len);
        if (result < 0) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        if (theRecvLen != nullptr) {
            *theRecvLen = result;
        }
        return ES_NO_ERROR;
    }

    auto EasySocket::recvBufferFrom(void* theBuf, const int32_t theLen, int32_t* theAddr, int32_t* thePort,
                                    int32_t* theRecvLen, const int32_t theTotalTime) const -> ES_ErrorType {
        if (mType == IPX || mType == SPX) {
            return ES_INVALID_ADDR;
        }
        sockaddr_in sa_in{};
        if (const ES_ErrorType result = recvBufferFrom(theBuf, theLen, reinterpret_cast<sockaddr *>(&sa_in), theRecvLen,
                                                       theTotalTime); result != ES_NO_ERROR) {
            return result;
        }
        if (theAddr != nullptr) {
            *theAddr = static_cast<int32_t>(sa_in.sin_addr.S_un.S_addr);
        }
        if (thePort != nullptr) {
            *thePort = ntohs(sa_in.sin_port);
        }
        return ES_NO_ERROR;
    }

#ifdef HAVE_IPX
    auto EasySocket::recvBufferFrom(void* theBuf, const int32_t theLen, std::array<uint8_t, 6>&theAddr,
                                    int32_t* thePort, int32_t* theRecvLen,
                                    const int32_t theTotalTime) const -> ES_ErrorType {
        if (mType == TCP || mType == SPX) {
            return ES_INVALID_ADDR;
        }
        sockaddr sa{};
        if (const ES_ErrorType result = recvBufferFrom(theBuf, theLen, &sa, theRecvLen, theTotalTime);
            result != ES_NO_ERROR) {
            return result;
        }
        auto* sa_ipx = reinterpret_cast<sockaddr_ipx *>(&sa);
        std::ranges::copy(sa_ipx->sa_nodenum, theAddr.begin());
        if (thePort != nullptr) {
            *thePort = ntohs(sa_ipx->sa_socket);
        }
        return ES_NO_ERROR;
    }
#endif // HAVE_IPX

    auto EasySocket::recvBufferFrom(void* theBuf, const int32_t theLen, std::string* theAddrString, int32_t* thePort,
                                    int32_t* theRecvLen, const int32_t theTotalTime) const -> ES_ErrorType {
        if (mType == IPX || mType == SPX) {
            return ES_INVALID_ADDR;
        }
        sockaddr_in sa_in{};
        if (const ES_ErrorType result = recvBufferFrom(theBuf, theLen, reinterpret_cast<sockaddr *>(&sa_in), theRecvLen,
                                                       theTotalTime); result != ES_NO_ERROR) {
            return result;
        }

        if (theAddrString != nullptr) {
            *theAddrString = inet_ntoa(sa_in.sin_addr);
        }
        if (thePort != nullptr) {
            *thePort = ntohs(sa_in.sin_port);
        }
        return ES_NO_ERROR;
    }

    auto EasySocket::shutdown(const int32_t theHow) const -> ES_ErrorType {
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }
        if (::shutdown(mSocket, theHow) == SOCKET_ERROR) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        return ES_NO_ERROR;
    }

    void EasySocket::disconnect() {
        mConnected = false;
        if (mType != TCP && mType != SPX) {
            constexpr sockaddr sa{};
            if (::connect(mSocket, &sa, sizeof(sockaddr)) == 0) {
                return;
            }
        }
        close();
    }

    auto EasySocket::gracefulClose(const int theWaitTime) -> ES_ErrorType {
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }

        if (::shutdown(mSocket, 1) != 0) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        bool timeout = false;
        const uint32_t start = GetTickCount();
        while (true) {
            const uint32_t elapsed_time = GetTickCount() - start;
            int32_t remaining_time;
            if (elapsed_time < theWaitTime) {
                remaining_time = theWaitTime - static_cast<int32_t>(elapsed_time);
            }
            else {
                remaining_time = 0;
                timeout = true;
            }
            if (!waitForRead(remaining_time)) {
                return ES_TIMED_OUT;
            }
            std::array<char, 1000> buf{};
            const int32_t recv_len = recv(mSocket, buf.data(), buf.size(), 0);
            if (recv_len == 0) {
                break;
            }
            if (recv_len < 0) {
                return WSAErrorToEnum(WSAGetLastError());
            }
            if (timeout) {
                break;
            }
        }
        close();
        return ES_NO_ERROR;
    }

    void EasySocket::close() {
        if (!isInvalid()) {
            closesocket(mSocket);
        }
        mSocket = INVALID_SOCKET;
        mConnected = false;
    }

    void EasySocket::close(const bool linger, const uint16_t theLingerSeconds) {
        if (!isInvalid()) {
            const auto value = std::bit_cast<std::array<char, 4>>(::linger{
                static_cast<u_short>(linger), theLingerSeconds
            });
            setsockopt(mSocket, SOL_SOCKET, SO_LINGER, value.data(), value.size());
            closesocket(mSocket);
        }
        mSocket = INVALID_SOCKET;
        mConnected = false;
    }

#ifdef WIN32
    auto EasySocket::asyncSelect(HWND__* hWnd, const uint32_t wMsg, const int32_t lEvent) const -> ES_ErrorType {
        if (isInvalid()) {
            return ES_INVALID_SOCKET;
        }
        if (WSAAsyncSelect(mSocket, hWnd, wMsg, lEvent) == SOCKET_ERROR) {
            return WSAErrorToEnum(WSAGetLastError());
        }
        return ES_NO_ERROR;
    }
#endif // WIN32

    auto EasySocket::setOption(const int32_t level, const int32_t optname, const char* optval,
                               const int32_t optlen) const -> int32_t {
        return setsockopt(mSocket, level, optname, optval, optlen);
    }

    auto EasySocket::getOption(const int32_t level, const int32_t optname, char* optval,
                               int32_t* optlen) const -> int32_t {
        return getsockopt(mSocket, level, optname, optval, optlen);
    }
}
