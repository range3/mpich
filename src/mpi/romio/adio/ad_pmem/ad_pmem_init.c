#include "ad_pmem.h"

static bool ADIOI_PMEM_Initialized = false;

void ADIOI_PMEM_Init(ADIO_File fd, int *error_code) {
  if (ADIOI_PMEM_Initialized == false){
    pmembb_init();
  }
}
