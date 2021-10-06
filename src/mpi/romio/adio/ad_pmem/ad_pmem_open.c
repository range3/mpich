/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adio/ad_ufs/ad_ufs.h"
#include "adioi.h"
#include "libpmemobj/pool_base.h"
#include "mpi.h"
#include <limits.h>
#include <pmembb/pmembb.h>

static int nDigits(int n) {
  if (n < 0)
    return nDigits((n == INT_MIN) ? INT_MAX : -n);
  if (n < 10)
    return 1;
  return 1 + nDigits(n / 10);
}

void ADIOI_PMEM_Open(ADIO_File fd, int *error_code) {
  int myrank, nprocs;
  static char myname[] = "ADIOI_PMEM_OPEN";
  char *pool_path, *buf = NULL;
  int len;
  PMemBBPool pop;

  MPI_Comm_rank(fd->comm, &myrank);

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Open called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  ADIOI_UFS_Open(fd, error_code);
  if (*error_code != MPI_SUCCESS) {
    return;
  }

  if (fd->hints->pmem.pool_per_rank == ADIOI_HINT_DISABLE) {
    pool_path = fd->hints->pmem.pool_list;
  } else {
    // /path/to/pool_name.myrank
    len = (strlen(fd->hints->pmem.pool_list) + 1 + nDigits(myrank) + 1) *
          sizeof(char);
    buf = ADIOI_Malloc(len);
    if (buf == NULL) {
      *error_code =
          MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                               __LINE__, MPI_ERR_OTHER, "**nomem2", 0);
      return;
    }
    pool_path = buf;
    sprintf(pool_path, "%s.%d", fd->hints->pmem.pool_list, myrank);
  }

  pop = pmembb_pool_open(pool_path);
  if (pop == NULL && errno == ENOENT) {
    pop = pmembb_pool_create(pool_path, fd->hints->pmem.pool_size,
                             S_IWUSR | S_IRUSR);
  }

  if (pop == NULL) {
    *error_code = ADIOI_Err_create_code(myname, pool_path, errno);
    goto finalize;
  }

  fd->fs_ptr = pop;
  *error_code = MPI_SUCCESS;

finalize:
  if (buf) {
    ADIOI_Free(buf);
  }
}
