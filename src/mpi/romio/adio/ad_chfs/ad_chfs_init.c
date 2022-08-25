#include "ad_chfs.h"

/* keyval hack to init/term chfs API*/
static int ADIOI_CHFS_Initialized = MPI_KEYVAL_INVALID;

void ADIOI_CHFS_Term(int* error_code) {
  static char myname[] = "ADIOI_CHFS_Term";

  if (chfs_term()) {
    *error_code =
        MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                             __LINE__, MPI_ERR_FILE, "chfs_term", 0);
    return;
  }
  *error_code = MPI_SUCCESS;
}

void ADIOI_CHFS_Term_call(MPI_Comm comm,
                          int keyval,
                          void* attribute_val,
                          void* extra_state) {
  int error_code;
#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(comm, &nprocs);
  MPI_Comm_rank(comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Term_call called\n", myrank, nprocs);
#endif
  ADIOI_CHFS_Term(&error_code);
  MPI_Comm_free_keyval(&keyval);
  return error_code;
}

void ADIOI_CHFS_Init(int* error_code) {
  static char myname[] = "ADIOI_CHFS_Init";

  if (ADIOI_CHFS_Initialized != MPI_KEYVAL_INVALID) {
    *error_code = MPI_SUCCESS;
    return;
  }

#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  FPRINTF(stdout, "[%d/%d] calling chfs_init in ADIOI_CHFS_Init\n", myrank,
          nprocs);
#endif

  if (chfs_init(NULL)) {
    *error_code =
        MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                             __LINE__, MPI_ERR_OTHER, "chfs_init", 0);
    return;
  }
  *error_code = MPI_SUCCESS;

  MPI_Comm_create_keyval(MPI_COMM_NULL_COPY_FN, ADIOI_CHFS_Term_call,
                         &ADIOI_CHFS_Initialized, (void*)0);
  MPI_Comm_set_attr(MPI_COMM_SELF, ADIOI_CHFS_Initialized, (void*)0);
}
