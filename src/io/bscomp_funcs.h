/**
 * @file bscomp_funcs.h
 * @brief Function macros for compressing and decompressing arrays
 * @author Dominique LaSalle <lasalle@cs.umn.edu>
 * Copyright 2013
 * @version 1
 * @date 2014-03-18
 */




/* prefixing ugliness */
#define BSCOMP_PRE2(prefix,suffix) __ ## prefix ## _ ## suffix
#define BSCOMP_PRE1(prefix,suffix) BSCOMP_PRE2(prefix,suffix)
#define BSCOMP_PRI(name) BSCOMP_PRE1(BSCOMP_PREFIX,name)



/******************************************************************************
* CONSTANTS *******************************************************************
******************************************************************************/

#ifndef BSCOMP_CONSTANTS
#define BSCOMP_CONSTANTS
static const size_t __CHUNK_SIZE = 0x1000000ULL;
static const size_t __BUFFERSIZE = 0x4000ULL;
#endif





/******************************************************************************
* FUNCTIONS *******************************************************************
******************************************************************************/



#ifdef BSCOMP_TYPE_FLOAT
#define BSCOMP_SMALL_T float
#define BSCOMP_BIG_T double
#else
#define BSCOMP_SMALL_T uint32_t
#define BSCOMP_BIG_T uint64_t
#endif


static int BSCOMP_PRI(read)(
    BSCOMP_TYPE_T * const a, 
    const size_t len,
    file_t * const file,
    const int big,
    const int compression) 
{
  int rv;
  uint64_t b64,i;
  BSCOMP_BIG_T * big_buffer;
  BSCOMP_SMALL_T * small_buffer;

  rv = BOWSTRING_SUCCESS;

  if (big) {
    if (sizeof(BSCOMP_BIG_T) == sizeof(BSCOMP_TYPE_T)) {
      big_buffer = (BSCOMP_BIG_T*)a;
    } else {
      big_buffer = (BSCOMP_BIG_T*)malloc(sizeof(BSCOMP_BIG_T)*len);
    }
    switch (compression) {
      case BOWSTRING_COMPRESSION_NONE:
        if (fread(&b64,sizeof(uint64_t),1,file->fd) != 1) {
          rv = BOWSTRING_ERROR_FILEREAD;;
          goto END;
        }
        if (fread(big_buffer,sizeof(BSCOMP_BIG_T),b64/sizeof(BSCOMP_BIG_T), \
            file->fd) != b64/sizeof(BSCOMP_BIG_T)) {
          rv = BOWSTRING_ERROR_FILEREAD;;
          goto END;
        }
        break;
      default:
        dl_error("Unsupported compression type: '%d'\n",compression);
    }
    if ((void*)big_buffer != (void*)a) {
      for (i=0;i<len;++i) {
        a[i] = (BSCOMP_BIG_T)big_buffer[i];
      }
      dl_free(big_buffer);
    }
  } else {
    if (sizeof(BSCOMP_SMALL_T) == sizeof(BSCOMP_TYPE_T)) {
      small_buffer = (BSCOMP_SMALL_T*)a;
    } else {
      small_buffer = (BSCOMP_SMALL_T*)malloc(sizeof(BSCOMP_SMALL_T)*len);
    }
    switch (compression) {
      case BOWSTRING_COMPRESSION_NONE:
        if (fread(&b64,sizeof(uint64_t),1,file->fd) != 1) {
          rv = BOWSTRING_ERROR_FILEREAD;;
          goto END;
        }
        if (fread(small_buffer,sizeof(BSCOMP_SMALL_T),
            b64/sizeof(BSCOMP_SMALL_T),file->fd) != 
            b64/sizeof(BSCOMP_SMALL_T)) {
          rv = BOWSTRING_ERROR_FILEREAD;;
          goto END;
        }
        break;
      default:
        dl_error("Unsupported compression type: '%d'\n",compression);
    }
    if ((void*)small_buffer != (void*)a) {
      for (i=0;i<len;++i) {
        a[i] = (BSCOMP_SMALL_T)small_buffer[i];
      }
      dl_free(small_buffer);
    }
  }

  END:

  return rv;
}


static int BSCOMP_PRI(write)(
    const BSCOMP_TYPE_T * const a, 
    const size_t len,
    file_t * const file,
    const int big,
    const int compression) 
{
  int rv;
  uint64_t b64,i;
  BSCOMP_BIG_T * x = NULL;
  BSCOMP_SMALL_T * y = NULL; 
  const BSCOMP_BIG_T * big_buffer;
  const BSCOMP_SMALL_T * small_buffer;

  rv = BOWSTRING_SUCCESS;

  if (a == NULL) {
    b64 = 0;
    fwrite(&b64,sizeof(uint64_t),1,file->fd);
    rv = BOWSTRING_SUCCESS;
    goto END;
  }

  if (big) {
    if (sizeof(BSCOMP_BIG_T) == sizeof(BSCOMP_TYPE_T)) {
      big_buffer = (const BSCOMP_BIG_T*)a;
    } else {
      x = (BSCOMP_BIG_T*)malloc(sizeof(BSCOMP_BIG_T)*len);
      for (i=0;i<len;++i) {
        x[i] = (BSCOMP_BIG_T)a[i];
      }
      big_buffer = x;
    }
    switch (compression) {
      case BOWSTRING_COMPRESSION_NONE:
        b64 = len*sizeof(BSCOMP_BIG_T);
        fwrite(&b64,sizeof(uint64_t),1,file->fd);
        fwrite(big_buffer,sizeof(BSCOMP_BIG_T),len,file->fd);
        break;
      default:
        dl_error("Unsupported compression type: '%d'\n",compression);
    }
    if (big_buffer == x) {
      dl_free(x);
    }
  } else {
    if (sizeof(BSCOMP_SMALL_T) == sizeof(BSCOMP_TYPE_T)) {
      small_buffer = (const BSCOMP_SMALL_T*)a;
    } else {
      y = (BSCOMP_SMALL_T*)malloc(sizeof(BSCOMP_SMALL_T)*len);
      for (i=0;i<len;++i) {
        y[i] = (BSCOMP_SMALL_T)a[i];
      }
      small_buffer = y;
    }
    switch (compression) {
      case BOWSTRING_COMPRESSION_NONE:
        b64 = len*sizeof(BSCOMP_SMALL_T);
        fwrite(&b64,sizeof(uint64_t),1,file->fd);
        fwrite(small_buffer,sizeof(BSCOMP_SMALL_T),len,file->fd);
        break;
      default:
        dl_error("Unsupported compression type: '%d'\n",compression);
    }
    if (small_buffer == y) {
      dl_free(y);
    }
  }

  END:

  return rv;
}


#undef BSCOMP_BIG_T
#undef BSCOMP_SMALL_T


