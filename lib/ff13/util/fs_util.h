#pragma once

#include <ff.h>

typedef struct {
    DWORD total;
    DWORD free;
} FS_USAGE;

int fs_info(FATFS *fs);
int fs_usage(FATFS *fs, FS_USAGE *fs_usage);
int fs_read_file(char * filename, void * data, int len);
int fs_read_text_file(char * filename, char * data, int len);
int fs_write_file(char * filename, const void * data, int len);
int fs_get_file_size(char * filename);
