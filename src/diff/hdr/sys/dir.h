typedef struct
{
    int     dd_fd;          /* file descriptor */
    int     dd_loc;         /* offset in block */
    int     dd_size;        /* amount of valid data */
    char    *dd_buf;        /* directory block */
} DIR;                      /* stream data from opendir() */

typedef struct direct
{
    int             d_ino;          /* "inode number" of entry */
    int             d_off;          /* offset of disk directory entry */
    unsigned short  d_reclen;       /* length of this record */
    char            d_name[1];      /* name of file */
} dirent_t;

extern DIR              *opendir(const char *);
extern struct direct    *readdir(DIR *);
extern long             telldir(DIR *);
extern void             seekdir(DIR *, long);
extern void             rewinddir(DIR *);
extern int              closedir(DIR *);
