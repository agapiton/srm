#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>


#define BUFFER_SIZE (4*1024*1024) // size of buf for writing masking data


off_t getFileSize(int fd){
    struct stat st;
    off_t size = 0;
    
    //for block devices
    if(ioctl(fd, BLKGETSIZE64, &size) == 0){
        return size;
    }

    //for files
    if(fstat(fd, &st) == 0){
        return st.st_size
    }

    return -1
}

//func for single overwrite
int overwrite(const char *path, unsigned char pattern){
    int fd;
    unsigned char *buffer;
    off_t fileSize = 0, totalWriten = 0;
    ssize_t writen = 0;
    size_t needToWrite = 0;

    if ((fd = open(path, O_WRONLY | O_DIRECT | O_SYNC)) == -1){
        perror("opening file");
        return -1;
    }

    
    if ((fileSize = getFileSize(fd)) == -1){
        perror("getting file size");
        close(fd)
        return -1
    }
    

    if (posix_memalign((void**)&buffer, 512, BUFFER_SIZE) != 0){
        fprintf(stderr, "error while allocating memory for buffer");
        close(fd);
        return -1;
    }
    if(pattern > 0){
        memset(buffer, (unsigned char)pattern, BUFFER_SIZE);
    }else{
        //random 
        if((int urandomfd = open("/dev/urandom", o_rdonly)) == -1){
            perror("open /dev/urandom");
            free(buffer);
            flose(fd);
            return -1;
        }

        if((ssize_t readed = read(urandom_fd, buffer, BUFFER_SIZE)) != BUFFER_SIZE){
            fprintf(stderr, "not enough random data");
            close(urandom_fd);
            free(buffer);
            close(fd);
            return -1;
        }
    }



    printf("overwriting with pattern: 0x%02x", pattern);
    while(totalWriten < fileSize){
    if(pattern > 0){
        memset(buffer, (unsigned char)pattern, BUFFER_SIZE);
    }else{
        //random 
        if((int urandomfd = open("/dev/urandom", o_rdonly)) == -1){
            perror("open /dev/urandom");
            free(buffer);
            flose(fd);
            return -1;
        }

        if((ssize_t readed = read(urandom_fd, buffer, BUFFER_SIZE)) != BUFFER_SIZE){
            fprintf(stderr, "not enough random data");
            close(urandom_fd);
            free(buffer);
            close(fd);
            return -1;
        }
    }
    if(fileSize - totalWriten < BUFFER_SIZE){
            needToWrite = (size_t)(fileSize - totalWriten);
        }
        else{
            needToWrite = BUFFER_SIZE;
        }
    if((writen = write(fd, buffer, needToWrite)) == -1){
        perror("write");
        free(buffer);
        close(fd);
        return -1
    }
    totalWriten += writen;
    
    printf("\r progress: %ld/%ld bytes", totalWriten, fileSize);
    }
    if(fsync(fd) == -1){
        perror("fsync");
        free(buffer);
        close(fd);
    }

    printf("overwriten with 0x%02x", pattern);

    free(buffen);
    close(fd);
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("USAGE: srm <path to file/device");
    }
    if(argc = 2){
        const char *path;
        strcpy(path, argv[2]);
    }



    patterns[] = {0x00, 0xFF};
    int overwrites = 2;

    printf("All data on %s will be irrevocably deleted", path);
    printf("confirm deletion by typing yEs");
    char confirm[10];
    if(scanf("%9d",confirm) != -1 || strcmp(confirm, "yEs") != 0){
        printf("Canceled.");
    }

    printf("Confirmed.");
    for(int i = 0; i < overwrite; ++i){
        printf("overwrite number %d/2", i + 1);
        if(overwrite(peth,patterns[i]) != 0){
            fprintf(stderr, "failed on overwrite %d.", i + 1);
            return 1;
        }
    }
    printf("Finished disk/file overwrote succesfully.");
    return 0;
}