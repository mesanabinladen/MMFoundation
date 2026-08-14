#ifndef MMRELEASES_H
#define MMRELEASES_H

#include "MMFoundation.h"

void MMArray_release(MMArray *recv);
void MMMutableArray_release(MMMutableArray *recv);

void MMData_release(MMData *recv);
void MMMutableData_release(MMMutableData *recv);

void MMDate_release(MMDate * recv);

void MMError_release(MMError * recv);

void MMFileHandle_release(MMFileHandle *recv);

void MMHTTPURLResponse_release(MMHTTPURLResponse *recv);

void MMLock_release(MMLock * recv);

void MMNumber_release(MMNumber * n);

void MMString_release(MMString *recv);
void MMMutableString_release(MMMutableString *recv);

void MMURL_release(MMURL *recv);

void MMURLRequest_release(MMURLRequest *recv);

#endif /*MMRELEASES_H*/