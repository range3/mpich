/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adio.h"
#include "adio/ad_pmem/ad_pmem_common.h"
#include "adio/ad_ufs/ad_ufs.h"
#include "adioi.h"
#include "libpmemobj/pool_base.h"
#include "mpi.h"
#include <limits.h>
#include <pmembb/pmembb.h>
#include <string.h>

static int nDigits(int n) {
  if (n < 0)
    return nDigits((n == INT_MIN) ? INT_MAX : -n);
  if (n < 10)
    return 1;
  return 1 + nDigits(n / 10);
}

static int get_local_rank() {
  char *str;
  int local_rank = -1;

  str = getenv("MPI_LOCALRANKID");
  if (!str) {
    str = getenv("OMPI_COMM_WORLD_LOCAL_RANK");
  }
  if (str) {
    local_rank = atoi(str);
  }
  return local_rank;
}

static int get_pool_name_at(const char *pool_list, int n, const char **begin) {
  int slen, skip;
  const char *p = pool_list;

  for (skip = n; skip > 0; --skip) {
    slen = (int)strcspn(p, ",");
    p += slen;

    if (*p == '\0') {
      return -1;
    }

    p += 1;
  }

  *begin = p;
  return (int)strcspn(p, ",");
}

static int get_pool_list_size(const char *pool_list) {
  int i, c;
  for (i = 0, c = 1; pool_list[i]; ++i) {
    if (pool_list[i] == ',') {
      ++c;
    }
  }
  return c;
}

void ADIOI_PMEM_Open(ADIO_File fd, int *error_code) {
  int grank, myrank, nprocs, lrank;
  static char myname[] = "ADIOI_PMEM_OPEN";
  char *pool_path, *buf = NULL;
  const char *begin;
  int len;
  struct ADIOI_PMEM_fs_s *pmem_fs = NULL;

  MPI_Comm_rank(MPI_COMM_WORLD, &grank);
  MPI_Comm_rank(fd->comm, &myrank);

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Open called on %s\n", myrank, nprocs,
          fd->filename);
  FPRINTF(stdout, "[%d/%d] local_rank = %d\n", myrank, nprocs,
          get_local_rank());
#endif

  ADIOI_PMEM_Init(fd, error_code);

  ADIOI_UFS_Open(fd, error_code);
  if (*error_code != MPI_SUCCESS) {
    return;
  }

  pmem_fs = (ADIOI_PMEM_fs *)ADIOI_Malloc(sizeof(ADIOI_PMEM_fs));
  if (pmem_fs == NULL) {
    *error_code =
        MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                             __LINE__, MPI_ERR_OTHER, "**nomem2", 0);
    goto on_abort;
  }
  memset(pmem_fs, 0, sizeof(ADIOI_PMEM_fs));

  if (ADIOI_PMEM_pool_ref_cnt == 0) {
    // get pool_path
    if (fd->hints->pmem.pool_per_rank == ADIOI_HINT_DISABLE) {
      lrank = get_local_rank();
      if (lrank < 0) {
        *error_code = MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE,
                                           myname, __LINE__, MPI_ERR_OTHER,
                                           "Local rank is not set in env", 0);
        goto on_abort;
      }
      len = get_pool_name_at(
          fd->hints->pmem.pool_list,
          lrank % get_pool_list_size(fd->hints->pmem.pool_list), &begin);
      if (len < 0) {
        *error_code = MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE,
                                           myname, __LINE__, MPI_ERR_OTHER,
                                           "Error get pool name", 0);
        goto on_abort;
      }
      buf = ADIOI_Malloc(len + 1);
      if (buf == NULL) {
        *error_code =
            MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                                 __LINE__, MPI_ERR_OTHER, "**nomem2", 0);
        goto on_abort;
      }
      pool_path = buf;
      ADIOI_Strncpy(pool_path, begin, len);
      pool_path[len] = '\0';
    } else {
      // /path/to/pool_name.grank
      len = (strlen(fd->hints->pmem.pool_list) + 1 + nDigits(grank) + 1) *
            sizeof(char);
      buf = ADIOI_Malloc(len);
      if (buf == NULL) {
        *error_code =
            MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
                                 __LINE__, MPI_ERR_OTHER, "**nomem2", 0);
        goto on_abort;
      }
      pool_path = buf;
      sprintf(pool_path, "%s.%d", fd->hints->pmem.pool_list, grank);
    }
#ifdef DEBUG
    FPRINTF(stdout, "[%d/%d] open pool_path = %s\n", myrank, nprocs, pool_path);
#endif

    pmem_fs->pool = pmembb_pool_open(pool_path);
    if (pmem_fs->pool == NULL && errno == ENOENT) {
      pmem_fs->pool = pmembb_pool_create(pool_path, fd->hints->pmem.pool_size,
                                         S_IWUSR | S_IRUSR);
    }
    ADIOI_PMEM_pool = pmem_fs->pool;
    ADIOI_PMEM_pool_ref_cnt += 1;
  } else {
    // pool is already opend in this process.
    pmem_fs->pool = ADIOI_PMEM_pool;
    ADIOI_PMEM_pool_ref_cnt += 1;
  }

  if (pmem_fs->pool == NULL) {
    *error_code = ADIOI_Err_create_code(myname, pool_path, errno);
    goto on_abort;
  }

  pmem_fs->file = pmembb_file_open(pmem_fs->pool, fd->filename);
  if (pmem_fs->file == NULL && errno == ENOENT) {
    pmem_fs->file = pmembb_file_create(pmem_fs->pool, fd->filename);
  }

  if (pmem_fs->file == NULL) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
    goto on_abort;
  }

  pmem_fs->bb = pmembb_bb_create(pmem_fs->file);
  if (pmem_fs->bb == NULL) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
    goto on_abort;
  }

  fd->fs_ptr = pmem_fs;
  *error_code = MPI_SUCCESS;

  goto finalize;
on_abort:
  if (pmem_fs) {
    if (pmem_fs->bb) {
      pmembb_bb_close(pmem_fs->bb);
    }
    if (pmem_fs->file) {
      pmembb_file_close(pmem_fs->file);
    }
    if (pmem_fs->pool) {
      if (--ADIOI_PMEM_pool_ref_cnt <= 0) {
#ifdef DEBUG
        FPRINTF(stdout, "[%d/%d] close pool\n", myrank, nprocs);
#endif
        ADIOI_PMEM_pool = NULL;
        pmembb_pool_close(pmem_fs->pool);
      }
    }
    ADIOI_Free(pmem_fs);
  }
finalize:
  if (buf) {
    ADIOI_Free(buf);
  }
}
