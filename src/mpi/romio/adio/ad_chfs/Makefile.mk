##
## Copyright (C) by Argonne National Laboratory
##     See COPYRIGHT in top-level directory
##

if BUILD_AD_CHFS

noinst_HEADERS += adio/ad_chfs/ad_chfs.h

romio_other_sources +=                \
    adio/ad_chfs/ad_chfs_init.c   \
    adio/ad_chfs/ad_chfs_close.c  \
    adio/ad_chfs/ad_chfs_read.c   \
    adio/ad_chfs/ad_chfs_rdcoll.c \
    adio/ad_chfs/ad_chfs_wrcoll.c \
    adio/ad_chfs/ad_chfs_open.c   \
    adio/ad_chfs/ad_chfs_write.c  \
    adio/ad_chfs/ad_chfs_done.c   \
    adio/ad_chfs/ad_chfs_fcntl.c  \
    adio/ad_chfs/ad_chfs_iread.c  \
    adio/ad_chfs/ad_chfs_iwrite.c \
    adio/ad_chfs/ad_chfs_wait.c   \
    adio/ad_chfs/ad_chfs_flush.c  \
    adio/ad_chfs/ad_chfs_seek.c   \
    adio/ad_chfs/ad_chfs_resize.c \
    adio/ad_chfs/ad_chfs_hints.c  \
    adio/ad_chfs/ad_chfs_delete.c \
    adio/ad_chfs/ad_chfs.c

endif BUILD_AD_CHFS
