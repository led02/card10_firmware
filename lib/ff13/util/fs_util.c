#include "fs_util.h"
#include "ff.h"
#include <stdint.h>
#include <string.h>

FATFS FatFs;          /* File system object for logical drive */
FS_USAGE FsUsage;


int fs_info(FATFS *fs)
{
    memcpy(fs, &FatFs, sizeof(FATFS));
    return 0;
}
int fs_usage(FATFS *fs, FS_USAGE *fs_usage)
{
    FRESULT res;
    DWORD tot_clust, fre_clust, sec_size;

    res = f_getfree("/", &fre_clust, &fs);
    if(res != FR_OK)
        return -res;

    // sectore size = sectors per cluster *  sector size
#if FF_MAX_SS == FF_MIN_SS
    sec_size = fs->csize * FF_MAX_SS;
#else
    sec_size = fs->csize * fs.ssize;
#endif

    // total/free sectors * sectore size
    tot_clust = fs->n_fatent - 2;
    fs_usage->total = tot_clust * sec_size; //FatFs.ssize;
    fs_usage->free = fre_clust * sec_size; //FatFs.ssize;

    return 0;
}

int fs_read_file(char * filename, void * data, int len){
    FIL file;
    UINT readbytes;
    int res;

    res=f_open(&file, filename, FA_OPEN_EXISTING|FA_READ);
    if(res){
        return -1;
    };

    res = f_read(&file, data, len, &readbytes);
    if(res){
        return -1;
    };

    f_close(&file);

	return readbytes;
}

int fs_read_text_file(char * filename, char * data, int len){
    int readbytes;

    if(len<1) return -1;
    readbytes=fs_read_file(filename,data,len-1);
    if(readbytes<0){
        data[0]=0;
        return readbytes;
    };
    data[readbytes]=0;
    while(readbytes>0 && data[readbytes-1]<0x20){
        data[--readbytes]=0;
    };
    return readbytes;
}


int fs_write_file(char * filename, const void * data, int len){
    FIL file;
    UINT writebytes;
    int res;

	res=f_open(&file, filename, FA_CREATE_ALWAYS|FA_WRITE);
    if(res){
        return -res;
    };

    res = f_write(&file, data, len, &writebytes);
    if(res){
        return -res;
    };
    f_close(&file);

	return writebytes;
}

int fs_get_file_size(char * filename){
    FILINFO finfo;
    int res;

    /// XXX: Untested
    res=f_stat(filename, &finfo);
    if(res){
        return -1;
    }

    return finfo.fsize;
}
