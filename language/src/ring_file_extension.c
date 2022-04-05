/* Copyright (c) 2013-2022 Mahmoud Fayed <msfclipper@yahoo.com> */
#include "ring.h"
/* Functions */

void ring_vm_file_loadfunctions ( RingState *pRingState )
{
    RING_API_REGISTER("fopen",ring_vm_file_fopen);
    RING_API_REGISTER("fclose",ring_vm_file_fclose);
    RING_API_REGISTER("fflush",ring_vm_file_fflush);
    RING_API_REGISTER("freopen",ring_vm_file_freopen);
    RING_API_REGISTER("tempfile",ring_vm_file_tempfile);
    RING_API_REGISTER("tempname",ring_vm_file_tempname);
    RING_API_REGISTER("fseek",ring_vm_file_fseek);
    RING_API_REGISTER("ftell",ring_vm_file_ftell);
    RING_API_REGISTER("rewind",ring_vm_file_rewind);
    RING_API_REGISTER("fgetpos",ring_vm_file_fgetpos);
    RING_API_REGISTER("fsetpos",ring_vm_file_fsetpos);
    RING_API_REGISTER("clearerr",ring_vm_file_clearerr);
    RING_API_REGISTER("feof",ring_vm_file_feof);
    RING_API_REGISTER("ferror",ring_vm_file_ferror);
    RING_API_REGISTER("perror",ring_vm_file_perror);
    RING_API_REGISTER("rename",ring_vm_file_rename);
    RING_API_REGISTER("remove",ring_vm_file_remove);
    RING_API_REGISTER("fgetc",ring_vm_file_fgetc);
    RING_API_REGISTER("fgets",ring_vm_file_fgets);
    RING_API_REGISTER("fputc",ring_vm_file_fputc);
    RING_API_REGISTER("fputs",ring_vm_file_fputs);
    RING_API_REGISTER("ungetc",ring_vm_file_ungetc);
    RING_API_REGISTER("fread",ring_vm_file_fread);
    RING_API_REGISTER("fwrite",ring_vm_file_fwrite);
    RING_API_REGISTER("dir",ring_vm_file_dir);
    RING_API_REGISTER("read",ring_vm_file_read);
    RING_API_REGISTER("write",ring_vm_file_write);
    RING_API_REGISTER("fexists",ring_vm_file_fexists);
    RING_API_REGISTER("direxists",ring_vm_file_direxists);
    RING_API_REGISTER("getpathtype",ring_vm_file_getpathtype);
    RING_API_REGISTER("getfilesize",ring_vm_file_getfilesize);
    RING_API_REGISTER("int2bytes",ring_vm_file_int2bytes);
    RING_API_REGISTER("float2bytes",ring_vm_file_float2bytes);
    RING_API_REGISTER("double2bytes",ring_vm_file_double2bytes);
    RING_API_REGISTER("bytes2int",ring_vm_file_bytes2int);
    RING_API_REGISTER("bytes2float",ring_vm_file_bytes2float);
    RING_API_REGISTER("bytes2double",ring_vm_file_bytes2double);
}
/* Check File/Dir/Type */

int ring_fexists_general ( const char *cFileName )
{
    #ifdef _WIN32
        /* Windows Only */
        struct _stat sb  ;
        wchar_t cPath[MAX_PATH]  ;
        int nLen1,nFileNameSize  ;
        nFileNameSize = strlen(cFileName) ;
        nLen1 = MultiByteToWideChar(CP_UTF8, 0, cFileName, nFileNameSize, cPath, nFileNameSize) ;
        if ( nLen1 >= MAX_PATH ) {
            return 0 ;
        }
        cPath[nLen1] = L'\0' ;
        if ( _wstat(cPath, &sb) == 0 ) {
            if ( S_ISREG(sb.st_mode) ) {
                /* Path exists and it is a regular file */
                return 1 ;
            }
        }
    #else
        struct stat sb  ;
        if ( stat(cFileName, &sb) == 0 ) {
            if ( S_ISREG(sb.st_mode) ) {
                /* Path exists and it is a regular file */
                return 1 ;
            }
        }
    #endif
    return 0 ;
}

int ring_direxists ( const char *cDirPath )
{
    struct stat sb  ;
    if ( stat(cDirPath, &sb) == 0 ) {
        if ( S_ISDIR(sb.st_mode) ) {
            /* Path exists and it is a directory */
            return 1 ;
        }
    }
    return 0 ;
}

int ring_getpathtype ( const char *cDirPath )
{
    struct stat sb  ;
    if ( stat(cDirPath, &sb) == 0 ) {
        if ( S_ISREG(sb.st_mode) ) {
            /* Path exists and it is a regular file */
            return 1 ;
        }
        if ( S_ISDIR(sb.st_mode) ) {
            /* Path exists and it is a directory */
            return 2 ;
        }
        /* Unknown Type */
        return -1 ;
    }
    return 0 ;
}

RING_LONGLONG ring_getfilesize ( const char *cFilePath )
{
    struct stat sb  ;
    if ( stat(cFilePath, &sb) == 0 ) {
        if ( S_ISREG(sb.st_mode) ) {
            /* Path exists and it is a regular file */
            return (RING_LONGLONG) sb.st_size ;
        }
    }
    /* Doesn't exist or not a file */
    return (RING_LONGLONG) -1 ;
}

void ring_vm_file_fopen ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) && RING_API_ISSTRING(2) ) {
        fp = (FILE *) ring_custom_fopen(RING_API_GETSTRING(1),RING_API_GETSTRING(2));
        RING_API_RETMANAGEDCPOINTER(fp,RING_VM_POINTER_FILE,ring_vm_file_freefunc);
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fclose ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            RING_API_RETNUMBER(fclose(fp));
            RING_API_SETNULLPOINTER(1);
        }
        else {
            RING_API_ERROR(RING_API_NULLPOINTER);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fflush ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            RING_API_RETNUMBER(fflush(fp));
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_freopen ( void *pPointer )
{
    FILE *fp  ;
    FILE *fp2  ;
    if ( RING_API_PARACOUNT != 3 ) {
        RING_API_ERROR(RING_API_MISS3PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) && RING_API_ISSTRING(2) && RING_API_ISPOINTER(3) ) {
        fp = (FILE *) RING_API_GETCPOINTER(3,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            fp2 = freopen(RING_API_GETSTRING(1),RING_API_GETSTRING(2),fp);
            RING_API_RETCPOINTER(fp2,RING_VM_POINTER_FILE);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_tempfile ( void *pPointer )
{
    FILE *fp  ;
    fp = tmpfile();
    RING_API_RETMANAGEDCPOINTER(fp,RING_VM_POINTER_FILE,ring_vm_file_freefunc);
}

void ring_vm_file_tempname ( void *pPointer )
{
    #ifdef _WIN32
        /* Windows */
        #ifdef _MSC_VER
            /* Visual C/C++ */
            char _tmpfile[L_tmpnam_s]  ;
            errno_t error  ;
            error = tmpnam_s(_tmpfile,L_tmpnam_s);
            if ( error ) {
                RING_API_ERROR(RING_VM_ERROR_TEMPFILENAME);
            }
            else {
                RING_API_RETSTRING(_tmpfile);
            }
        #else
            RING_API_RETSTRING(tmpnam(NULL));
        #endif
        /* Mac OS X */
    #elif __MACH__
        RING_API_RETSTRING(tmpnam(NULL));
        /* Linux */
    #else
        char _tmpfile[20] = "/tmp/ringtempXXXXXX" ;
        RING_API_RETSTRING(mkdtemp(_tmpfile));
    #endif
}

void ring_vm_file_fseek ( void *pPointer )
{
    int nResult  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 3 ) {
        RING_API_ERROR(RING_API_MISS3PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISNUMBER(2) && RING_API_ISNUMBER(3) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            nResult = fseek(fp,RING_API_GETNUMBER(2),RING_API_GETNUMBER(3));
            RING_API_RETNUMBER(nResult);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_ftell ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            RING_API_RETNUMBER(ftell(fp));
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_rewind ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            rewind(fp);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fgetpos ( void *pPointer )
{
    fpos_t *pos  ;
    int nResult  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            pos = (fpos_t *) ring_state_malloc(((VM *) pPointer)->pRingState,sizeof(fpos_t));
            nResult = fgetpos(fp,pos);
            if ( nResult == 0 ) {
                RING_API_RETMANAGEDCPOINTER(pos,RING_VM_POINTER_FILEPOS,ring_state_free);
            }
            else {
                RING_API_RETNUMBER(nResult);
            }
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fsetpos ( void *pPointer )
{
    int nResult  ;
    FILE *fp  ;
    fpos_t *pos  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISPOINTER(2) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        pos = (fpos_t *) RING_API_GETCPOINTER(2,RING_VM_POINTER_FILEPOS) ;
        if ( (fp != NULL)  && (pos != NULL) ) {
            nResult = fsetpos(fp,pos);
            RING_API_RETNUMBER(nResult);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_clearerr ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            clearerr(fp);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_feof ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            RING_API_RETNUMBER(feof(fp));
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_ferror ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            RING_API_RETNUMBER(ferror(fp));
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_perror ( void *pPointer )
{
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        perror(RING_API_GETSTRING(1));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_rename ( void *pPointer )
{
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) && RING_API_ISSTRING(2) ) {
        RING_API_RETNUMBER(rename(RING_API_GETSTRING(1),RING_API_GETSTRING(2)));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_remove ( void *pPointer )
{
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        remove(RING_API_GETSTRING(1));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fgetc ( void *pPointer )
{
    int c  ;
    char cStr[2]  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            c = fgetc(fp);
            if ( c != EOF ) {
                cStr[0] = c ;
                cStr[1] = '\0' ;
                RING_API_RETSTRING(cStr);
            }
            else {
                RING_API_RETNUMBER(c);
            }
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fgets ( void *pPointer )
{
    char *cStr  ;
    int nSize  ;
    FILE *fp  ;
    char *cResult  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISNUMBER(2) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            nSize = RING_API_GETNUMBER(2) ;
            if ( nSize < 1 ) {
                RING_API_ERROR(RING_VM_FILE_BUFFERSIZE);
                return ;
            }
            nSize++ ;
            cStr = (char *) ring_state_malloc(((VM *) pPointer)->pRingState,nSize);
            cResult = fgets(cStr,nSize,fp);
            if ( cResult != NULL ) {
                RING_API_RETSTRING(cStr);
            }
            else {
                RING_API_RETNUMBER(0);
            }
            ring_state_free(((VM *) pPointer)->pRingState,cStr);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fputc ( void *pPointer )
{
    int c  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISSTRING(2) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            c = fputc(RING_API_GETSTRING(2)[0],fp);
            if ( c != EOF ) {
                RING_API_RETNUMBER(1);
            }
            else {
                RING_API_RETNUMBER(0);
            }
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fputs ( void *pPointer )
{
    int c  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISSTRING(2) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            c = fputs(RING_API_GETSTRING(2),fp);
            if ( c != EOF ) {
                RING_API_RETNUMBER(1);
            }
            else {
                RING_API_RETNUMBER(0);
            }
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_ungetc ( void *pPointer )
{
    int c  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISSTRING(2) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            c = ungetc(RING_API_GETSTRING(2)[0],fp);
            if ( c != EOF ) {
                RING_API_RETNUMBER(1);
            }
            else {
                RING_API_RETNUMBER(0);
            }
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fread ( void *pPointer )
{
    char *cStr  ;
    int nSize  ;
    int nResult  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISNUMBER(2) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            nSize = RING_API_GETNUMBER(2) ;
            if ( nSize < 1 ) {
                RING_API_ERROR(RING_VM_FILE_BUFFERSIZE);
                return ;
            }
            cStr = (char *) ring_state_malloc(((VM *) pPointer)->pRingState,nSize);
            nResult = fread(cStr,1,nSize,fp);
            if ( nResult == 0 ) {
                RING_API_RETNUMBER(nResult);
            }
            else {
                RING_API_RETSTRING2(cStr,nResult);
            }
            ring_state_free(((VM *) pPointer)->pRingState,cStr);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fwrite ( void *pPointer )
{
    int nResult  ;
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISPOINTER(1) && RING_API_ISSTRING(2) ) {
        fp = (FILE *) RING_API_GETCPOINTER(1,RING_VM_POINTER_FILE) ;
        if ( fp != NULL ) {
            nResult = fwrite(RING_API_GETSTRING(2),1,RING_API_GETSTRINGSIZE(2) ,fp);
            RING_API_RETNUMBER(nResult);
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_dir ( void *pPointer )
{
    const char *cStr  ;
    List *pList, *pList2  ;
    #ifdef _WIN32
        /* Windows Only */
        WIN32_FIND_DATA fdFile  ;
        HANDLE hFind  ;
        String *pString  ;
    #else
        DIR *pDir  ;
        struct dirent *pDirent  ;
        struct stat st  ;
        char cPath[FILENAME_MAX]  ;
        char cCurrentDir[FILENAME_MAX]  ;
    #endif
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        cStr = RING_API_GETSTRING(1);
        pList = RING_API_NEWLIST ;
        #ifdef _WIN32
            /* Windows Only */
            pString = ring_string_new_gc(((VM *) pPointer)->pRingState,cStr);
            ring_string_add_gc(((VM *) pPointer)->pRingState,pString,"\\*.*");
            cStr = ring_string_get(pString);
            if ( ! ((hFind = FindFirstFile(cStr, &fdFile)) == INVALID_HANDLE_VALUE) ) {
                do {
                    if ( strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0 ) {
                        pList2 = ring_list_newlist_gc(((VM *) pPointer)->pRingState,pList);
                        ring_list_addstring_gc(((VM *) pPointer)->pRingState,pList2,fdFile.cFileName);
                        if ( fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                            ring_list_adddouble_gc(((VM *) pPointer)->pRingState,pList2,1.0);
                        }
                        else {
                            ring_list_adddouble_gc(((VM *) pPointer)->pRingState,pList2,0.0);
                        }
                    }
                } while (FindNextFile(hFind, &fdFile))  ;
                FindClose(hFind);
                RING_API_RETLIST(pList);
            }
            else {
                RING_API_ERROR(RING_API_BADDIRECTORY);
            }
            ring_string_delete_gc(((VM *) pPointer)->pRingState,pString);
        #else
            pDir = opendir(cStr);
            if ( pDir != NULL ) {
                getcwd(cCurrentDir, FILENAME_MAX);
                chdir(cStr);
                while ( (pDirent = readdir(pDir)) ) {
                    if ( strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0 ) {
                        pList2 = ring_list_newlist_gc(((VM *) pPointer)->pRingState,pList);
                        ring_list_addstring_gc(((VM *) pPointer)->pRingState,pList2,pDirent->d_name);
                        /* Prepare Path */
                        getcwd(cPath, FILENAME_MAX);
                        strcat(cPath,"/");
                        strcat(cPath,pDirent->d_name);
                        stat(cPath,&st);
                        if ( S_ISDIR(st.st_mode) ) {
                            ring_list_adddouble_gc(((VM *) pPointer)->pRingState,pList2,1.0);
                        }
                        else {
                            ring_list_adddouble_gc(((VM *) pPointer)->pRingState,pList2,0.0);
                        }
                    }
                }
                closedir(pDir);
                RING_API_RETLIST(pList);
                chdir(cCurrentDir);
            }
            else {
                RING_API_ERROR(RING_API_BADDIRECTORY);
            }
        #endif
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_read ( void *pPointer )
{
    FILE *fp  ;
    long int nSize  ;
    char *cBuffer  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        fp = (FILE *) ring_custom_fopen(RING_API_GETSTRING(1) , "rb");
        if ( fp == NULL ) {
            RING_API_ERROR(RING_VM_ERROR_CANTOPENFILE);
            return ;
        }
        fseek( fp , 0 , SEEK_END );
        nSize = ftell(fp);
        fseek( fp , 0 , SEEK_SET );
        cBuffer = (char *) ring_state_malloc(((VM *) pPointer)->pRingState,nSize);
        fread( cBuffer , 1 , nSize , fp );
        fclose( fp ) ;
        RING_API_RETSTRING2(cBuffer,nSize);
        ring_state_free(((VM *) pPointer)->pRingState,cBuffer);
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_write ( void *pPointer )
{
    FILE *fp  ;
    if ( RING_API_PARACOUNT != 2 ) {
        RING_API_ERROR(RING_API_MISS2PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        if ( RING_API_ISSTRING(2) ) {
            fp = (FILE *) ring_custom_fopen(RING_API_GETSTRING(1) , "w+b");
            if ( fp == NULL ) {
                RING_API_ERROR(RING_VM_ERROR_CANTOPENFILE);
                return ;
            }
            fwrite( RING_API_GETSTRING(2) , RING_API_GETSTRINGSIZE(2) , 1 , fp );
            fflush(fp);
            fclose( fp ) ;
        }
        else {
            RING_API_ERROR("Error in second parameter, Function requires string !");
            return ;
        }
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_fexists ( void *pPointer )
{
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        RING_API_RETNUMBER(ring_fexists_general(RING_API_GETSTRING(1)));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_direxists ( void *pPointer )
{
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        RING_API_RETNUMBER(ring_direxists(RING_API_GETSTRING(1)));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_getpathtype ( void *pPointer )
{
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        RING_API_RETNUMBER(ring_getpathtype(RING_API_GETSTRING(1)));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_getfilesize ( void *pPointer )
{
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        RING_API_RETNUMBER(ring_getfilesize(RING_API_GETSTRING(1)));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}
/* Number & Bytes */

void ring_vm_file_int2bytes ( void *pPointer )
{
    Ring_uData uData  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISNUMBER(1) ) {
        uData.iNumber = (int) RING_API_GETNUMBER(1) ;
        RING_API_RETSTRING2(uData.cBytes,sizeof(int));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_float2bytes ( void *pPointer )
{
    Ring_uData uData  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISNUMBER(1) ) {
        uData.fNumber = (float) RING_API_GETNUMBER(1) ;
        RING_API_RETSTRING2(uData.cBytes,sizeof(float));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_double2bytes ( void *pPointer )
{
    Ring_uData uData  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISNUMBER(1) ) {
        uData.dNumber = RING_API_GETNUMBER(1) ;
        RING_API_RETSTRING2(uData.cBytes,sizeof(double));
    }
    else {
        RING_API_ERROR(RING_API_BADPARATYPE);
    }
}

void ring_vm_file_bytes2int ( void *pPointer )
{
    Ring_uData uData  ;
    int x  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        if ( RING_API_GETSTRINGSIZE(1) == sizeof(int) ) {
            for ( x = 0 ; x < sizeof(int) ; x++ ) {
                uData.cBytes[x] = RING_API_GETSTRING(1)[x] ;
            }
            RING_API_RETNUMBER((double) uData.iNumber);
            return ;
        }
    }
    RING_API_ERROR(RING_API_BADPARATYPE);
}

void ring_vm_file_bytes2float ( void *pPointer )
{
    Ring_uData uData  ;
    int x  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        if ( RING_API_GETSTRINGSIZE(1) == sizeof(float) ) {
            for ( x = 0 ; x < sizeof(float) ; x++ ) {
                uData.cBytes[x] = RING_API_GETSTRING(1)[x] ;
            }
            RING_API_RETNUMBER((double) uData.fNumber);
            return ;
        }
    }
    RING_API_ERROR(RING_API_BADPARATYPE);
}

void ring_vm_file_bytes2double ( void *pPointer )
{
    Ring_uData uData  ;
    int x  ;
    if ( RING_API_PARACOUNT != 1 ) {
        RING_API_ERROR(RING_API_MISS1PARA);
        return ;
    }
    if ( RING_API_ISSTRING(1) ) {
        if ( RING_API_GETSTRINGSIZE(1) == sizeof(double) ) {
            for ( x = 0 ; x < sizeof(double) ; x++ ) {
                uData.cBytes[x] = RING_API_GETSTRING(1)[x] ;
            }
            RING_API_RETNUMBER(uData.dNumber);
            return ;
        }
    }
    RING_API_ERROR(RING_API_BADPARATYPE);
}

void ring_vm_file_freefunc ( void *pRingState,void *pPointer )
{
    FILE *fp  ;
    fp = (FILE *) pPointer ;
    fclose( fp ) ;
}
