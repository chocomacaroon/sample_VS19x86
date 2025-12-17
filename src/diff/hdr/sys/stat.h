#define S_IFDIR (1)
#define S_IFMT  (2)
#define S_IFREG (3)

struct stat
{
    int  st_ino;
    int  st_dev;
    int  st_mtime;
    int  st_mode;
    long st_size;
};
