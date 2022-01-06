/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adioi.h"
#include "hint_fns.h"
#include "mpi.h"
#include <libpmemobj.h>
#include <stdlib.h>

#ifdef ROMIO_BGL
#include "../ad_bgl/ad_bgl.h"
#endif

#define MPIO_PMEM_IO_NTHREADS_DFLT "0"
#define MPIO_PMEM_POOL_PER_RANK_DFLT "enable"
#define MPIO_PMEM_POOL_LIST_DFLT "mpio_pmem_pool"
#define MPIO_PMEM_POOL_SIZE "8388608"

void ADIOI_PMEM_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code) {
  int myrank, nprocs, len;
  char *value, *env, *pool_size = NULL, *pool_size_buf = NULL, *endptr;
  int initialized = 0;
  static char myname[] = "ADIOI_PMEM_SETINFO";

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_SetInfo called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  initialized = fd->hints->initialized;

  /* set the values for collective I/O and data sieving parameters */
  ADIOI_GEN_SetInfo(fd, users_info, error_code);

  /* initialize hints to default values if they haven't been
   * previously initialized
   */
  if (!initialized) {
    env = getenv("MPIO_PMEM_IO_NTHREADS");
    value = env ? env : MPIO_PMEM_IO_NTHREADS_DFLT;
    ADIOI_Info_set(fd->info, "mpio_pmem_io_nthreads", value);
    fd->hints->pmem.io_nthreads = atoi(value);

    env = getenv("MPIO_PMEM_POOL_PER_RANK");
    value = env ? env : MPIO_PMEM_POOL_PER_RANK_DFLT;
    ADIOI_Info_set(fd->info, "mpio_pmem_pool_per_rank", value);
    fd->hints->pmem.pool_per_rank =
        !strcmp(value, "enable") ? ADIOI_HINT_ENABLE : ADIOI_HINT_DISABLE;

    env = getenv("MPIO_PMEM_POOL_LIST");
    value = env ? env : MPIO_PMEM_POOL_LIST_DFLT;
    ADIOI_Info_set(fd->info, "mpio_pmem_pool_list", value);
    len = (strlen(value) + 1) * sizeof(char);
    fd->hints->pmem.pool_list = ADIOI_Malloc(len);
    if (fd->hints->pmem.pool_list == NULL) {
      *error_code =
          MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                               __LINE__, MPI_ERR_OTHER, "**nomem2", 0);
      goto finalize;
    }
    ADIOI_Strncpy(fd->hints->pmem.pool_list, value, len);

    env = getenv("MPIO_PMEM_POOL_SIZE");
    pool_size = env ? env : MPIO_PMEM_POOL_SIZE;
    ADIOI_Info_set(fd->info, "mpio_pmem_pool_size", pool_size);
  }

  /* set user specified values if exist */
  if (users_info != MPI_INFO_NULL) {
    ADIOI_Info_check_and_install_int(fd, users_info, "mpio_pmem_io_nthreads",
                                     &(fd->hints->pmem.io_nthreads), myname,
                                     error_code);

    ADIOI_Info_check_and_install_enabled(
        fd, users_info, "mpio_pmem_pool_per_rank",
        &(fd->hints->pmem.pool_per_rank), myname, error_code);

    ADIOI_Info_check_and_install_str(fd, users_info, "mpio_pmem_pool_list",
                                     &(fd->hints->pmem.pool_list), myname,
                                     error_code);


    if (ADIOI_Info_check_and_install_str(fd, users_info, "mpio_pmem_pool_size",
                                         &pool_size_buf, myname,
                                         error_code) == 0) {
      if(pool_size_buf) {
        pool_size = pool_size_buf;
      }
    }
  }

  if (pool_size) {
    // set pool_size converting str to size_t
    fd->hints->pmem.pool_size = (size_t)strtoumax(pool_size, &endptr, 10);
    if (errno == ERANGE) {
      *error_code =
          MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                               __LINE__, MPI_ERR_OTHER, "**infoval", 0);
      goto finalize;
    }
  }

  *error_code = MPI_SUCCESS;

finalize:
  if (pool_size_buf) {
    ADIOI_Free(pool_size_buf);
  }
}
