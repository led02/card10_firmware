#ifndef EPICARCIUM_FS_INTERNAL_H_INCLUDED
#define EPICARCIUM_FS_INTERNAL_H_INCLUDED

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "epicardium.h"

/* Number of bits to use for indexing into our internal pool of files/directories
 * This indirectly specifies the size of the pool as 1^EPIC_FAT_FD_INDEX_BITS
 * Increase if number of open file descriptors is not enough, but be aware of
 * memory usage of the pool!
 */
#define EPIC_FAT_FD_INDEX_BITS 4
#define EPIC_FAT_STATIC_SEMAPHORE 1

// forward declaration, actual definition is in filesystem_fat.c
typedef struct EpicFileSystem EpicFileSystem;

int efs_open(EpicFileSystem *fs, const char *filename, const char *modeString);
int efs_close(EpicFileSystem *fs, int fd);
int efs_read(EpicFileSystem *fs, int fd, void *buf, size_t nbytes);
int efs_write(EpicFileSystem *fs, int fd, const void *buf, size_t nbytes);
int efs_flush(EpicFileSystem *fs, int fd);
int efs_seek(EpicFileSystem *fs, int fd, long offset, int whence);
int efs_tell(EpicFileSystem *fs, int fd);
int efs_stat(EpicFileSystem *fs, const char *filename, struct epic_stat *stat);
/**
 * lock global filesystem
 * 
 * locks the global mutex and, if the global EpicFileSystem has been initialized correctly
 * passes it to *fs
 *
 * Upon successful return, the filesystem has to be re-locked with epic_fs_unlock_global
 * In case of error, the filesystem will be left in a locked state.
 */
int efs_lock_global(EpicFileSystem** fs);
void efs_unlock_global(EpicFileSystem* fs);

#endif// EPICARCIUM_FS_INTERNAL_H_INCLUDED
