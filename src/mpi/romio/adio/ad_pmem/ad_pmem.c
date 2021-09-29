/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"

/* adioi.h has the ADIOI_Fns_struct define */
#include "adioi.h"

struct ADIOI_Fns_struct ADIO_PMEM_operations = {
    ADIOI_PMEM_Open,             /* Open */
    ADIOI_GEN_OpenColl,          /* OpenColl */
    ADIOI_PMEM_ReadContig,       /* ReadContig */
    ADIOI_PMEM_WriteContig,      /* WriteContig */
    ADIOI_PMEM_ReadStridedColl,  /* ReadStridedColl */
    ADIOI_PMEM_WriteStridedColl, /* WriteStridedColl */
    ADIOI_PMEM_SeekIndividual,   /* SeekIndividual */
    ADIOI_PMEM_Fcntl,            /* Fcntl */
    ADIOI_PMEM_SetInfo,          /* SetInfo */
    ADIOI_PMEM_ReadStrided,      /* ReadStrided */
    ADIOI_PMEM_WriteStrided,     /* WriteStrided */
    ADIOI_PMEM_Close,            /* Close */
    ADIOI_PMEM_IreadContig,      /* IreadContig */
    ADIOI_PMEM_IwriteContig,     /* IwriteContig */
    ADIOI_PMEM_ReadDone,         /* ReadDone */
    ADIOI_PMEM_WriteDone,        /* WriteDone */
    ADIOI_PMEM_ReadComplete,     /* ReadComplete */
    ADIOI_PMEM_WriteComplete,    /* WriteComplete */
    ADIOI_PMEM_IreadStrided,     /* IreadStrided */
    ADIOI_PMEM_IwriteStrided,    /* IwriteStrided */
    ADIOI_PMEM_Flush,            /* Flush */
    ADIOI_PMEM_Resize,           /* Resize */
    ADIOI_PMEM_Delete,           /* Delete */
    ADIOI_GEN_Feature,           /* Features */
    "PMEM: Utilize node-local persistent memory",
    ADIOI_GEN_IreadStridedColl,  /* IreadStridedColl */
    ADIOI_GEN_IwriteStridedColl, /* IwriteStridedColl */
#if defined(F_SETLKW64)
    ADIOI_GEN_SetLock /* SetLock */
#else
    ADIOI_GEN_SetLock64 /* SetLock */
#endif
};
