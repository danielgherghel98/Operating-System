#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define pipe_resp "RESP_PIPE_57861" 
#define pipe_req  "REQ_PIPE_57861"
#define SHM_KEY 12702

int main(int argc, char **argv)
{
    char s[]={0x07,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54};
    //char control[]={0x04,0x45,0x58,0x49,0x54};
    char ping[]={0x04,0x50,0x49,0x4e,0x47};
    char pong[]={0x04,0x50,0x4f,0x4e,0x47};
    char shm[]={0x0a,0x43,0x52,0x45,0x41,0x54,0x45,0x5f,0x53,0x48,0x4d};
    char succ[]={0x07,0x53,0x55,0x43,0x43,0x45,0x53,0x53};
    char wrshm[]={0x0c,0x57,0x52,0x49,0x54,0x45,0x5f,0x54,0x4f,0x5f,0x53,0x48,0x4d};
    char error[]={0x05,0x45,0x52,0x52,0x4f,0x52};
    char map[]={0x08,0x4d,0x41,0x50,0x5f,0x46,0x49,0x4c,0x45};
    char ofset[]={0x15,0x52,0x45,0x41,0x44,0x5f,0x46,0x52,0x4f,0x4d,0x5f,0x46,0x49,0x4c,0x45,0x5f,0x4f,0x46,0x46,0x53,0x45,0x54};
    char section[]={0x16,0x52 ,0x45 ,0x41 ,0x44 ,0x5f ,0x46 ,0x52 ,0x4f ,0x4d ,0x5f ,0x46 ,0x49 ,0x4c ,0x45 ,0x5f ,0x53 ,0x45 ,0x43 ,0x54 ,0x49 ,0x4f ,0x4e};
    char logical[]={0x1e,'R','E','A','D','_','F','R','O','M','_','L','O','G','I','C','A','L','_','S','P','A','C','E','_','O','F','F','S','E','T'};
    int fd1=-1;
    int fd2=-1;
    //int fd3;
    int shmId;
    int fd;
    unsigned int start=0;
    unsigned int end=-1;
    off_t size;
    char* sharedMem=NULL;
    char* data=NULL;
    unlink(pipe_resp);
    if(mkfifo(pipe_resp, 0600) != 0) {
        perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
        return 1;
    }
    fd2 = open(pipe_req,O_RDONLY);
    if(fd2 == -1) {
        perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
        return 1;
    }
    fd1 = open(pipe_resp,O_RDWR);
    if(fd1 == -1) {
        perror("Could not open input file");
        return 1;
    }
    write(fd1,s,sizeof(s));
    printf("SUCCESS\n");
    int index=6;
    while(1)
    {
        //printf("while\n");
        char dimensiune;
        unsigned int nr=57861;
        read(fd2,&dimensiune,sizeof(char));
        unsigned int dim=(int)dimensiune;
        char z[dim+1];
        read(fd2,&z,dim);
        z[dim]='\0';
        if(strcmp(z,"PING")==0)
        {
            //printf("if\n");
            write(fd1,ping,sizeof(ping));
            write(fd1,pong,sizeof(pong));
            write(fd1,&nr,sizeof(nr));
        }
        if(strcmp(z,"CREATE_SHM")==0)
        {
           
            unsigned int octeti;
            read(fd2,&octeti,sizeof(unsigned int));
            //printf("Nr oct:  %d\n",octeti);
            shmId = shmget(SHM_KEY, octeti, IPC_CREAT | 0664);
            //printf("ShareId:  %d\n",shmId);
            if(shmId < 0) {
                write(fd1,shm,sizeof(shm));
                write(fd1,error,sizeof(error));
                
            }
            else
            {
                sharedMem = (char*)shmat(shmId, NULL, 0);
                if(sharedMem == (void*)-1)
                {
                    write(fd1,shm,sizeof(shm));
                    write(fd1,error,sizeof(error));
                    
                }
                else
                {
                    write(fd1,shm,sizeof(shm));
                    write(fd1,succ,sizeof(succ));
                }
           
            }
        }
        if(strcmp(z,"WRITE_TO_SHM")==0)
        {
            unsigned int offset;
            unsigned int value;
            read(fd2,&offset,sizeof(unsigned int));
            read(fd2,&value,sizeof(unsigned int));
            //printf("off:  %d  val:  %d\n",offset,value);
            if((offset>=0)&&(offset<=5017193))
            {
                if(offset<5017193-sizeof(unsigned int))
                {
                    *(unsigned int*)(sharedMem+offset)=value;
                    write(fd1,wrshm,sizeof(wrshm));
                    write(fd1,succ,sizeof(succ));
                }
                else{
                    write(fd1,wrshm,sizeof(wrshm));
                    write(fd1,error,sizeof(error));
                }
            }
            else
            {
                write(fd1,wrshm,sizeof(wrshm));
                write(fd1,error,sizeof(error));
            }
            
        }
        if(strcmp(z,"MAP_FILE")==0)
        {
            
            char dimen;
            read(fd2,&dimen,sizeof(char));
            unsigned int sizeF=(int)dimen;
            char file[sizeF+1];
            read(fd2,&file,sizeF);
            file[sizeF]='\0';
            fd=open(file,O_RDONLY);
            // printf("Fd:  %d\n",fd);
            // printf("File name:  %s\n",file);
            if(fd == -1) {
                write(fd1,map,sizeof(map));
                write(fd1,error,sizeof(error));
            }
            else{
                size = lseek(fd, 0, SEEK_END);
                lseek(fd, 0, SEEK_SET);
                data= (char*)mmap(NULL, size, PROT_READ,MAP_SHARED, fd, 0);
                if(data == (void*)-1) {
                    write(fd1,map,sizeof(map));
                    write(fd1,error,sizeof(error));
                    close(fd);
                   
                }
                else
                {
                    write(fd1,map,sizeof(map));
                    write(fd1,succ,sizeof(succ));
                    // printf("Fd:  %d\n",fd);
                    // printf("File name:  %s\n",file);
                }
               // munmap(data, size);
                //close(fd);
            }
            
        }
        if(strcmp(z,"READ_FROM_FILE_OFFSET")==0)
        {
            unsigned int adroffset=0;
            unsigned int nrbytes=0;
            read(fd2,&adroffset,sizeof(unsigned int));
            read(fd2,&nrbytes,sizeof(unsigned int));
            // printf("Offset:  %d\n",adroffset);
            // printf("bytes:  %d\n",nrbytes);
            if((sharedMem == (void*)-1)||(data == (void*)-1)||(adroffset+nrbytes>=size))
            {
                //printf("if 1\n");
                write(fd1,ofset,sizeof(ofset));
                write(fd1,error,sizeof(error));
                    
            }
            else
            {
                //printf("else\n");
                for (int i=0; i<nrbytes; i++)
                {
                    sharedMem[i]=data[i+adroffset];
                }
                //printf("dupa for\n");
                write(fd1,ofset,sizeof(ofset));
                write(fd1,succ,sizeof(succ));
            }
            
        }
        if(strcmp(z,"READ_FROM_FILE_SECTION")==0)
        {
            unsigned int nrsections=0;
            unsigned int adroffset=0;
            unsigned int nrbytes=0;
            unsigned int offsetSec=0;
            read(fd2,&nrsections,sizeof(unsigned int));
            read(fd2,&adroffset,sizeof(unsigned int));
            read(fd2,&nrbytes,sizeof(unsigned int));
            // printf("Section:  %d\n",nrsections);
            //  printf("Offset:  %d\n",adroffset);
            // printf("bytes:  %d\n",nrbytes);
            unsigned int nrofsections=data[5];
            if(nrsections>nrofsections)
            {
                write(fd1,section,sizeof(section));
                write(fd1,error,sizeof(error));
            }
            else
            { 
                memcpy(&offsetSec,(data+((nrsections-1)*19+6+11)),4);
            
                for (int i=0; i<nrbytes; i++)
                {
                    sharedMem[i]=data[i+offsetSec+adroffset];
                }
                write(fd1,section,sizeof(section));
                write(fd1,succ,sizeof(succ));
            }
            
        }
        if(strcmp(z,"READ_FROM_LOGICAL_SPACE_OFFSET")==0)
        {
            unsigned int logicaloffset=0;
            unsigned int nrbytes=0;
            read(fd2,&logicaloffset,sizeof(unsigned int));
            read(fd2,&nrbytes,sizeof(unsigned int));
            // printf("Offset:  %d\n",logicaloffset);
            // printf("bytes:  %d\n",nrbytes);
            unsigned int check=0;
            unsigned int secNo=logicaloffset/3072;
            unsigned int nrofsections=data[5];
            
            for (int i=1; i<=nrofsections; i++)
            {
                index+=11;
                unsigned int secoffset=0;
                unsigned int size=0;
                memcpy(&secoffset,(data+(index)),4);
                index+=4;
                memcpy(&size,(data+(index)),4);
                index+=4;
                start=end+1;
                end=start+size/3072;
                if((start<=secNo)&&(secNo<=end))
                {
                    check=1;
                    for (int j=0; j<nrbytes; j++)
                    {
                        sharedMem[j]=data[secoffset + (logicaloffset - (3072 * start))+j];
                    }
                    write(fd1,logical,sizeof(logical));
                    write(fd1,succ,sizeof(succ));
                    break;
                }
            }
            if(check==0)
            {
                write(fd1,logical,sizeof(logical));
                write(fd1,error,sizeof(error));
            }
            
        }
        if(strcmp(z,"EXIT")==0)
        {
            close(fd1);
            close(fd2);
            munmap(data, size);
            close(fd);
            break;
            return 0;
        }

    }
    close(fd1);
    close(fd2);
    return 0;
}