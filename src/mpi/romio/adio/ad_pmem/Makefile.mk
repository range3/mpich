##
## Copyright (C) by Argonne National Laboratory
##     See COPYRIGHT in top-level directory
##

if BUILD_AD_PMEM

noinst_HEADERS += adio/ad_pmem/ad_pmem.h

romio_other_sources +=                \
		adio/ad_pmem/ad_pmem_init.c   \
    adio/ad_pmem/ad_pmem_close.c  \
    adio/ad_pmem/ad_pmem_read.c   \
    adio/ad_pmem/ad_pmem_rdcoll.c \
    adio/ad_pmem/ad_pmem_wrcoll.c \
    adio/ad_pmem/ad_pmem_open.c   \
    adio/ad_pmem/ad_pmem_write.c  \
    adio/ad_pmem/ad_pmem_done.c   \
    adio/ad_pmem/ad_pmem_fcntl.c  \
    adio/ad_pmem/ad_pmem_iread.c  \
    adio/ad_pmem/ad_pmem_iwrite.c \
    adio/ad_pmem/ad_pmem_wait.c   \
    adio/ad_pmem/ad_pmem_flush.c  \
    adio/ad_pmem/ad_pmem_seek.c   \
    adio/ad_pmem/ad_pmem_resize.c \
    adio/ad_pmem/ad_pmem_hints.c  \
    adio/ad_pmem/ad_pmem_delete.c \
    adio/ad_pmem/ad_pmem.c

endif BUILD_AD_PMEM
