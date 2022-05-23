#include "ad_chfs.h"

static int ADIOI_CHFS_init_cnt = 0;

void ADIOI_CHFS_Init(int* error_code) {
  static char myname[] = "ADIOI_CHFS_Init";

  if (ADIOI_CHFS_init_cnt == 0) {
    if (chfs_init(NULL)) {
      *error_code =
          MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                               __LINE__, MPI_ERR_OTHER, "chfs_init", 0);
      return;
    }
  }
  ADIOI_CHFS_init_cnt += 1;
}

void ADIOI_CHFS_Term(int* error_code) {
  static char myname[] = "ADIOI_CHFS_Term";

  if (ADIOI_CHFS_init_cnt <= 0) {
    return;
  }

  ADIOI_CHFS_init_cnt -= 1;

  if (ADIOI_CHFS_init_cnt == 0) {
    if (chfs_term()) {
      *error_code =
          MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                               __LINE__, MPI_ERR_OTHER, "chfs_term", 0);
      return;
    }
  }
}
