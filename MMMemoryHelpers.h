#ifndef MMMEMORYHELPERS_H
#define MMMEMORYHELPERS_H

#include "MMFoundation.h"

//release methods
void MMArray_release(MMArray *recv);
void MMMutableArray_release(MMMutableArray *recv);

void MMDictionary_release(MMDictionary *recv);
void MMMutableDictionary_release(MMMutableDictionary *recv);

void MMData_release(MMData *recv);
void MMMutableData_release(MMMutableData *recv);

void MMDate_release(MMDate * recv);
void MMProcessInfo_release(MMProcessInfo *recv);

void MMError_release(MMError * recv);

void MMFileHandle_release(MMFileHandle *recv);

void MMHTTPURLResponse_release(MMHTTPURLResponse *recv);

void MMLock_release(MMLock * recv);

void MMNumber_release(MMNumber * n);
void MMValue_release(MMValue *recv);

void MMString_release(MMString *recv);
void MMMutableString_release(MMMutableString *recv);

void MMURL_release(MMURL *recv);

void MMURLRequest_release(MMURLRequest *recv);

void MMOperationQueue_release(MMOperationQueue *recv);

void MMInvocationOperation_release(MMInvocationOperation * recv);

//copy methods
MMArray * MMArray_copy(const MMArray * recv);
MMMutableArray * MMMutableArray_copy(const MMMutableArray * recv);

MMData *MMData_copy(MMData * recv);
MMMutableData *MMMutableData_copy(MMMutableData * recv);

MMDictionary * MMDictionary_copy(const MMDictionary * recv);
MMMutableDictionary * MMMutableDictionary_copy(const MMMutableDictionary * recv);

MMValue *MMValue_copy(const MMValue *recv);

MMError *MMError_copy(MMError *recv);

MMHTTPURLResponse *MMTTPURLResponse_copy(MMHTTPURLResponse *recv);

MMOperationQueue *MMOperationQueue_copy(MMOperationQueue * recv);

MMProcessInfo *MMProcessInfo_copy(const MMProcessInfo *recv);

MMString *MMString_copy(MMString *recv);
MMMutableString *MMMutableString_copy(MMMutableString * recv);

MMURL *MMURL_copy(MMURL *recv);

MMURLRequest *MMURLRequest_copy(MMURLRequest *recv);

#endif /*MMMEMORYHELPERS_H*/