#ifndef MMURLCONNECTION_H
#define MMURLCONNECTION_H

#include "MMTypes.h"
#include "MMData.h"
#include "MMURLRequest.h"
#include "MMHTTPURLResponse.h"

MMData *MMURLConnection_sendSynchronousRequest(MMURLRequest *request, MMHTTPURLResponse **response, MMError **error);

#endif /*MMURLCONNECTION_H*/