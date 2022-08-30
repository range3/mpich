#include "ad_chfs_common.h"
#include "ad_chfs.h"
#include "adioi.h"

MPI_Offset ADIOI_CHFS_get_size_coll(ADIO_File fd, int* error_code) {
  static char myname[] = "ADIOI_CHFS_get_size_coll";
  int nprocs, myrank;
  struct stat st;
  MPI_Offset res;

  MPI_Comm_rank(fd->comm, &myrank);
#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  FPRINTF(stdout, "[%d/%d] %s called on %s\n", myrank, nprocs, myname,
          fd->filename);
#endif

  if (myrank == fd->hints->ranklist[0]) {
    if (chfs_stat(fd->filename, &st)) {
      res = -errno;
    } else {
      res = st.st_size;
    }
  }

  MPI_Bcast(&res, 1, MPI_LONG, fd->hints->ranklist[0], fd->comm);

  if (res < 0) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, -res);
    return -1;
  } else {
    *error_code = MPI_SUCCESS;
    return res;
  }
}
