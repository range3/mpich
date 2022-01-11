#ifndef AD_PMEM_COMMON_H_INCLUDED
#define AD_PMEM_COMMON_H_INCLUDED
#include "ad_pmem.h"
#include "pmembb/pmembb.h"

struct ADIOI_PMEM_fs_s {
  PMemBBPool* pool;
  PMemBBFile* file;
  PMemBBBurstBuffer* bb; 
};
typedef struct ADIOI_PMEM_fs_s ADIOI_PMEM_fs;

extern int ADIOI_PMEM_pool_ref_cnt;
extern PMemBBPool* ADIOI_PMEM_pool;


#endif /* AD_PMEM_COMMON_H_INCLUDED */
