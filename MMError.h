#ifndef MMERROR_H
#define MMERROR_H

#include "MMTypes.h"

MMError *MMError_init();
MMError *MMError_copy(MMError *recv);

#endif /*MMERROR_H*/