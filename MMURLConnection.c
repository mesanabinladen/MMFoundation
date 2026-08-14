#include "MMURLConnection.h"

#include "MMString.h"

/* ============================================================
   Helper
   ============================================================ */
static void set_error(MMError **error, int code, const char *msg)
{
    if (!error) return;
    *error = calloc(1, sizeof(MMError));
    if (*error) {
        (*error)->code = code;
        (*error)->domain = MMString_initWithCString("MMURLErrorDomain");
        (*error)->localizedDescription = MMString_initWithCString(msg ? msg : "Unknown error");
    }
}

/* ============================================================
   WINDOWS (WinHTTP)
   ============================================================ */
#if defined(_WIN32) || defined(_WIN64)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

static MMData *send_with_winhttp(MMURLRequest *request,
                                 MMHTTPURLResponse **response,
                                 MMError **error)
{
    if (!request || !request->URL) {
        set_error(error, -1, "Invalid request");
        return nil;
    }

    // Converti URL in wide string
    int wlen = MultiByteToWideChar(CP_UTF8, 0, request->URL->url->cString, -1, nil, 0);
    wchar_t *wurl = malloc(wlen * sizeof(wchar_t));
    if (!wurl) {
        set_error(error, -1, "Out of memory");
        return nil;
    }
    MultiByteToWideChar(CP_UTF8, 0, request->URL->url->cString, -1, wurl, wlen);

    URL_COMPONENTS uc = {0};
    uc.dwStructSize = sizeof(uc);
    wchar_t host[256] = {0};
    wchar_t path[2048] = {0};
    uc.lpszHostName = host;
    uc.dwHostNameLength = 256;
    uc.lpszUrlPath = path;
    uc.dwUrlPathLength = 2048;

    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) {
        free(wurl);
        set_error(error, GetLastError(), "Invalid URL");
        return nil;
    }
    free(wurl);

    HINTERNET hSession = WinHttpOpen(L"MMURLConnection/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        set_error(error, GetLastError(), "WinHttpOpen failed");
        return nil;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, host, uc.nPort, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        set_error(error, GetLastError(), "WinHttpConnect failed");
        return nil;
    }

    DWORD flags = (uc.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path,
                                            nil, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            flags);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        set_error(error, GetLastError(), "WinHttpOpenRequest failed");
        return nil;
    }

    // accept non valid certificate, debug only!
    // DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
    //                 SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
    //                 SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
    // WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        set_error(error, GetLastError(), "WinHttpSendRequest failed");
        return nil;
    }

    if (!WinHttpReceiveResponse(hRequest, nil)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        set_error(error, GetLastError(), "WinHttpReceiveResponse failed");
        return nil;
    }

    // Status code
    DWORD statusCode = 0;
    DWORD statusSize = sizeof(statusCode);
    WinHttpQueryHeaders(hRequest,
                        WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                        nil, &statusCode, &statusSize, nil);

    // Leggi il body
    char *data_bytes = calloc(1, sizeof(char));
    int data_length = 1;
    if (!data_bytes) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        set_error(error, -1, "Out of memory");
        return nil;
    }

    DWORD available = 0;
    do {
        if (!WinHttpQueryDataAvailable(hRequest, &available)) break;
        if (available == 0) break;

        char *new_buf = realloc(data_bytes, data_length + available + 1);
        if (!new_buf) {
            free(data_bytes);
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            set_error(error, -1, "Out of memory");
            return nil;
        }
        data_bytes= new_buf;

        DWORD downloaded = 0;
        if (!WinHttpReadData(hRequest, data_bytes + data_length, available, &downloaded))
            break;

        data_length += downloaded;
        data_bytes[data_length] = '\0';
    } while (available > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    if (response) {
        *response = MMHTTPURLResponse_init();
        if (*response) {
            (*response)->statusCode = (long)statusCode;
            (*response)->url = MMURL_copy(request->URL);
        }
    }
    MMData * ret = MMData_initWithBytes(data_bytes, data_length);
    free(data_bytes);
    return ret;
}

/* ============================================================
   macOS / Linux (libcurl)
   ============================================================ */
#else

#include <curl/curl.h>

static size_t write_callback(void *contents, size_t size, size_t nmemb, MMMutableData *mem)
{
    size_t realsize = size * nmemb;

    char *ptr = malloc(realsize + 1);
    memcpy(ptr, contents, realsize);
    if (!ptr) return 0;
    ptr[realsize] = '\0';
    MMMutableData_appendBytes(mem, ptr, realsize +1);
    return realsize;
}

static MMData *send_with_libcurl(MMURLRequest *request, MMHTTPURLResponse **response, MMError **error){
    if (!request || !request->URL) {
        set_error(error, -1, "Invalid request");
        return nil;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        set_error(error, -1, "curl_easy_init failed");
        return nil;
    }

    MMMutableData * data = MMMutableData_initWithCapacity(0);
    if (!data) {
        curl_easy_cleanup(curl);
        set_error(error, -1, "Out of memory");
        return nil;
    }

    curl_easy_setopt(curl, CURLOPT_URL, request->URL->url->cString);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "MMURLConnection/1.0");
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // solo per test

    CURLcode res = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (res != CURLE_OK) {
        MM_release(data);
        set_error(error, (int)res, curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return nil;
    }

    if (response) {
        *response = MMHTTPURLResponse_init();
        if (*response) {
            (*response)->statusCode = http_code;
            (*response)->url = MMURL_copy(request->URL);
        }
    }

    curl_easy_cleanup(curl);
    return (MMData *)data;
}

#endif

/* ============================================================
   Funzione pubblica (stesso prototipo di prima)
   ============================================================ */
MMData *MMURLConnection_sendSynchronousRequest(MMURLRequest *request, MMHTTPURLResponse **response, MMError **error) {
    if (error) *error = nil;
    if (response) *response = nil;
#if defined(_WIN32) || defined(_WIN64)
    return send_with_winhttp(request, response, error);
#else
    return send_with_libcurl(request, response, error);
#endif
}