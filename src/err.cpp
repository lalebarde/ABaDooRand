/*
    Copyright (c) 2014 Contributors as noted in the AUTHORS file
    
    This file is part of ABaDooRand.
    
    ABaDooRand is available under two different licensing options designed to accommodate the needs of our various users.
    
    ABaDooRand licensed under our commercial license agreement is appropriate for development of proprietary/commercial 
    software where you do not want to share any source code with third parties or otherwise cannot comply with the terms 
    of the [GNU General Public License (GPL) version 3.0](GNU-GPL-v3.0). 
    If you are under this use case, please contact the author.
    
    ABaDooRand licensed under the [GNU GPL version 3.0](GNU-GPL-v3.0) is appropriate for the development of 
    ABaDooRand applications where you wish to use such applications in combination with software subject to the terms 
    of the [GNU GPL version 3.0](GNU-GPL-v3.0) or where you are otherwise willing to comply with the terms of the 
    [GNU GPL version 3.0](GNU-GPL-v3.0).
    
    ABaDooRand documentation is licensed under the terms of the [GNU Free Documentation License (FDL) version 1.3](GNU-FDL-v1.3),
    as published by the Free Software Foundation.
    
    ABaDooRand is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
    
    You should have received a copy of the licences along with this program, LICENCE.txt and the GNU licenses.  If not,
    see <http://www.gnu.org/licenses/> for the GNU ones.
*/

#include "err.hpp"
#include "platform.hpp"

const char *abadoorand::errno_to_string (int errno_)
{
    switch (errno_) {
#if defined ABADOORAND_HAVE_WINDOWS
    case ENOTSUP:
        return "Not supported";
    case EPROTONOSUPPORT:
        return "Protocol not supported";
    case ENOBUFS:
        return "No buffer space available";
    case ENETDOWN:
        return "Network is down";
    case EADDRINUSE:
        return "Address in use";
    case EADDRNOTAVAIL:
        return "Address not available";
    case ECONNREFUSED:
        return "Connection refused";
    case EINPROGRESS:
        return "Operation in progress";
#endif
//    case Exxx:
//        return "xxx";
    default:
#if defined _MSC_VER
#pragma warning (push)
#pragma warning (disable:4996)
#endif
        return strerror (errno_);
#if defined _MSC_VER
#pragma warning (pop)
#endif
    }
}

void abadoorand::abadoorand_abort(const char *errmsg_)
{
#if defined ABADOORAND_HAVE_WINDOWS

    //  Raise STATUS_FATAL_APP_EXIT.
    ULONG_PTR extra_info [1];
    extra_info [0] = (ULONG_PTR) errmsg_;
    RaiseException (0x40000015, EXCEPTION_NONCONTINUABLE, 1, extra_info);
#else
    (void)errmsg_;
    abort ();
#endif
}

#ifdef ABADOORAND_HAVE_WINDOWS

const char *abadoorand::wsa_error()
{
    int no = WSAGetLastError ();
    //  TODO: This is not a generic way to handle this...
    if (no == WSAEWOULDBLOCK)
        return NULL;

    return wsa_error_no (no);
}

const char *abadoorand::wsa_error_no (int no_)
{
    //  TODO:  It seems that list of Windows socket errors is longer than this.
    //         Investigate whether there's a way to convert it into the string
    //         automatically (wsaError->HRESULT->string?).
    return
        (no_ == WSABASEERR) ?
            "No Error" : 
        (no_ == WSAEINTR) ?
            "Interrupted system call" : 
        (no_ == WSAEBADF) ?
            "Bad file number" : 
        (no_ == WSAEACCES) ?
            "Permission denied" : 
        (no_ == WSAEFAULT) ?
            "Bad address" : 
        (no_ == WSAEINVAL) ?
            "Invalid argument" : 
        (no_ == WSAEMFILE) ?
            "Too many open files" : 
        (no_ == WSAEWOULDBLOCK) ?
            "Operation would block" : 
        (no_ == WSAEINPROGRESS) ?
            "Operation now in progress" : 
        (no_ == WSAEALREADY) ?
            "Operation already in progress" : 
        (no_ == WSAENOTSOCK) ?
            "Socket operation on non-socket" : 
        (no_ == WSAEDESTADDRREQ) ?
            "Destination address required" : 
        (no_ == WSAEMSGSIZE) ?
            "Message too long" : 
        (no_ == WSAEPROTOTYPE) ?
            "Protocol wrong type for socket" : 
        (no_ == WSAENOPROTOOPT) ?
            "Bad protocol option" : 
        (no_ == WSAEPROTONOSUPPORT) ?
            "Protocol not supported" : 
        (no_ == WSAESOCKTNOSUPPORT) ?
            "Socket type not supported" : 
        (no_ == WSAEOPNOTSUPP) ?
            "Operation not supported on socket" : 
        (no_ == WSAEPFNOSUPPORT) ?
            "Protocol family not supported" : 
        (no_ == WSAEAFNOSUPPORT) ?
            "Address family not supported by protocol family" : 
        (no_ == WSAEADDRINUSE) ?
            "Address already in use" : 
        (no_ == WSAEADDRNOTAVAIL) ?
            "Can't assign requested address" : 
        (no_ == WSAENETDOWN) ?
            "Network is down" : 
        (no_ == WSAENETUNREACH) ?
            "Network is unreachable" : 
        (no_ == WSAENETRESET) ?
            "Net dropped connection or reset" : 
        (no_ == WSAECONNABORTED) ?
            "Software caused connection abort" : 
        (no_ == WSAECONNRESET) ?
            "Connection reset by peer" : 
        (no_ == WSAENOBUFS) ?
            "No buffer space available" : 
        (no_ == WSAEISCONN) ?
            "Socket is already connected" : 
        (no_ == WSAENOTCONN) ?
            "Socket is not connected" : 
        (no_ == WSAESHUTDOWN) ?
            "Can't send after socket shutdown" : 
        (no_ == WSAETOOMANYREFS) ?
            "Too many references can't splice" : 
        (no_ == WSAETIMEDOUT) ?
            "Connection timed out" : 
        (no_ == WSAECONNREFUSED) ?
            "Connection refused" : 
        (no_ == WSAELOOP) ?
            "Too many levels of symbolic links" : 
        (no_ == WSAENAMETOOLONG) ?
            "File name too long" : 
        (no_ == WSAEHOSTDOWN) ?
            "Host is down" : 
        (no_ == WSAEHOSTUNREACH) ?
            "No Route to Host" : 
        (no_ == WSAENOTEMPTY) ?
            "Directory not empty" : 
        (no_ == WSAEPROCLIM) ?
            "Too many processes" : 
        (no_ == WSAEUSERS) ?
            "Too many users" : 
        (no_ == WSAEDQUOT) ?
            "Disc Quota Exceeded" : 
        (no_ == WSAESTALE) ?
            "Stale NFS file handle" : 
        (no_ == WSAEREMOTE) ?
            "Too many levels of remote in path" : 
        (no_ == WSASYSNOTREADY) ?
            "Network SubSystem is unavailable" : 
        (no_ == WSAVERNOTSUPPORTED) ?
            "WINSOCK DLL Version out of range" : 
        (no_ == WSANOTINITIALISED) ?
            "Successful WSASTARTUP not yet performed" : 
        (no_ == WSAHOST_NOT_FOUND) ?
            "Host not found" : 
        (no_ == WSATRY_AGAIN) ?
            "Non-Authoritative Host not found" : 
        (no_ == WSANO_RECOVERY) ?
            "Non-Recoverable errors: FORMERR REFUSED NOTIMP" : 
        (no_ == WSANO_DATA) ?
            "Valid name no data record of requested" :
        "error not defined"; 
}

void abadoorand::win_error (char *buffer_, size_t buffer_size_)
{
    DWORD errcode = GetLastError ();
#if defined _WIN32_WCE
    DWORD rc = FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errcode, MAKELANGID(LANG_NEUTRAL,
        SUBLANG_DEFAULT), (LPWSTR)buffer_, buffer_size_ / sizeof(wchar_t), NULL );
#else
    DWORD rc = FormatMessageA (FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errcode, MAKELANGID(LANG_NEUTRAL,
        SUBLANG_DEFAULT), buffer_, (DWORD) buffer_size_, NULL );
#endif
    abadoorand_assert (rc);
}

int abadoorand::wsa_error_to_errno (int errcode)
{
    switch (errcode) {
//  10009 - File handle is not valid.
    case WSAEBADF:
        return EBADF;
//  10013 - Permission denied.
    case WSAEACCES:
        return EACCES;
//  10014 - Bad address.
    case WSAEFAULT:
        return EFAULT;
//  10022 - Invalid argument.
    case WSAEINVAL:
        return EINVAL;
//  10024 - Too many open files.
    case WSAEMFILE:
        return EMFILE;
//  10036 - Operation now in progress.
    case WSAEINPROGRESS:
        return EAGAIN;
//  10040 - Message too long.
    case WSAEMSGSIZE:
        return EMSGSIZE;
//  10043 - Protocol not supported.
    case WSAEPROTONOSUPPORT:
        return EPROTONOSUPPORT;
//  10047 - Address family not supported by protocol family.
    case WSAEAFNOSUPPORT:
        return EAFNOSUPPORT;
//  10048 - Address already in use.
    case WSAEADDRINUSE:
        return EADDRINUSE;
//  10049 - Cannot assign requested address.
    case WSAEADDRNOTAVAIL:
        return EADDRNOTAVAIL;
//  10050 - Network is down.
    case WSAENETDOWN:
        return ENETDOWN;
//  10051 - Network is unreachable.
    case WSAENETUNREACH:
        return ENETUNREACH;
//  10052 - Network dropped connection on reset.
    case WSAENETRESET:
        return ENETRESET;
//  10053 - Software caused connection abort.
    case WSAECONNABORTED:
        return ECONNABORTED;
//  10054 - Connection reset by peer.
    case WSAECONNRESET:
        return ECONNRESET;
//  10055 - No buffer space available.
    case WSAENOBUFS:
        return ENOBUFS;
//  10057 - Socket is not connected.
    case WSAENOTCONN:
        return ENOTCONN;
//  10060 - Connection timed out.
    case WSAETIMEDOUT:
        return ETIMEDOUT;
//  10061 - Connection refused.
    case WSAECONNREFUSED:
        return ECONNREFUSED;
//  10065 - No route to host.
    case WSAEHOSTUNREACH:
        return EHOSTUNREACH;
    default:
        wsa_assert (false);
    }
    //  Not reachable
    return 0;
}

#endif
