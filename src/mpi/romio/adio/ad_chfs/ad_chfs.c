/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"

/* adioi.h has the ADIOI_Fns_struct define */
#include "adioi.h"

struct ADIOI_Fns_struct ADIO_CHFS_operations = {
    ADIOI_CHFS_Open,  /* Open */
    ADIOI_GEN_OpenColl, /* OpenColl */
    ADIOI_CHFS_ReadContig,    /* ReadContig */
    ADIOI_CHFS_WriteContig,   /* WriteContig */
    ADIOI_CHFS_ReadStridedColl,       /* ReadStridedColl */
    ADIOI_CHFS_WriteStridedColl,      /* WriteStridedColl */
    ADIOI_CHFS_SeekIndividual,        /* SeekIndividual */
    ADIOI_CHFS_Fcntl, /* Fcntl */
    ADIOI_CHFS_SetInfo,       /* SetInfo */
    ADIOI_CHFS_ReadStrided,   /* ReadStrided */
    ADIOI_CHFS_WriteStrided,  /* WriteStrided */
    ADIOI_CHFS_Close, /* Close */
    ADIOI_CHFS_IreadContig,   /* IreadContig */
    ADIOI_CHFS_IwriteContig,  /* IwriteContig */
    ADIOI_CHFS_ReadDone,      /* ReadDone */
    ADIOI_CHFS_WriteDone,     /* WriteDone */
    ADIOI_CHFS_ReadComplete,  /* ReadComplete */
    ADIOI_CHFS_WriteComplete, /* WriteComplete */
    ADIOI_CHFS_IreadStrided,  /* IreadStrided */
    ADIOI_CHFS_IwriteStrided, /* IwriteStrided */
    ADIOI_CHFS_Flush, /* Flush */
    ADIOI_CHFS_Resize,        /* Resize */
    ADIOI_CHFS_Delete,        /* Delete */
    ADIOI_GEN_Feature,  /* Features */
    "CHFS: parallel and distributed file system for node-local persistent memory",
    ADIOI_GEN_IreadStridedColl, /* IreadStridedColl */
    ADIOI_GEN_IwriteStridedColl,        /* IwriteStridedColl */
#if defined(F_SETLKW64)
    ADIOI_GEN_SetLock   /* SetLock */
#else
    ADIOI_GEN_SetLock64 /* SetLock */
#endif
};
