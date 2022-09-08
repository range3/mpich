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

char* ADIOI_CHFS_get_absolute_path(const char* relative_path, char* buf, size_t size) {
  size_t rel_path_len, cwd_len;
  rel_path_len = strlen(relative_path);
  if (relative_path[0] != '/') {
    if (getcwd(buf, size) == NULL) {
      return NULL;
    }
    cwd_len = strlen(buf);
    if (cwd_len + 1 + rel_path_len + 1 > size) {
      errno = ENAMETOOLONG;
      return NULL;
    }
    buf[cwd_len] = '/';
    memcpy(buf + cwd_len + 1, relative_path, rel_path_len + 1);
  } else {
    memcpy(buf, relative_path, rel_path_len + 1);
  }
  return buf;
}
