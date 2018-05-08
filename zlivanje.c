#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LENGTH 100

/**
 * Odpre datoteke in vrne tabelo povezav nanje
 */

FILE **open_src_files(int n)
{
    char *fname = (char *) malloc((MAX_FILE_NAME_LENGTH + 1) * sizeof(char));
    FILE **files = (FILE **) malloc(n * sizeof(FILE *));
    for (int i = 0; i < n; i++)
    {
        scanf("%s", fname);
        files[i] = fopen(fname, "r");
        if (files[i] == NULL)
        {
            printf("[Napaka]: could not open file %s; errno = %d.\n", fname, errno);
            exit(1);
        }
    }
    free(fname);
    return files;
}

/**
 * Kreira in vrne tabelo prvih elementov iz vseh datotek.
 * Ce je datoteka <i> prazna, na i-to mesto vpise -1.
 */

int *init(FILE **sources, int n, int *completed)
{
    int *buffer = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        if (fscanf(sources[i], "%d", &buffer[i]) < 1)
        {
            buffer[i] = -1;
            (*completed)++;
        }
    }
    return buffer;
}

/**
 * Vrne indeks minimuma brez upostevanaj elementa -1.
 */
 
int indeks_minimuma(int *t, int n)
{
    int iMin = -1;
    for (int i = 0; i < n; i++)
    {
        if (t[i] >= 0 && (iMin < 0 || t[i] < t[iMin]))
            iMin = i;
    }    
    return iMin;
}

void close_src_files(FILE **files, int n)
{
    for (int i = 0; i < n; i++)
        fclose(files[i]);
}

int main()
{
    int n;
    scanf("%d", &n);
    
    // 1 - open all source files
    FILE **sources = open_src_files(n);
    char *target_fname = (char *) malloc((MAX_FILE_NAME_LENGTH + 1) * sizeof(char));
    // 2 - open target file
    scanf("%s", target_fname);
    FILE *target = fopen(target_fname, "w");
    if (target == NULL)
    {
        printf("[Napaka]: could not open file %s; errno = %d.\n", target_fname, errno);
        return 2;
    }
    
    // 3 - init buffer of current elements
    int completed = 0;
    int *buffer = init(sources, n, &completed);
    
    while (completed < n)
    {
        int iMin = indeks_minimuma(buffer, n);
        fprintf(target, "%d\n", buffer[iMin]);
        
        // 4 - izpisani element nadomestimo z naslednim stevilom iz pripadajoce
        // vhodne datoteke, oziroma z -1, ce smo prispeli do konca datoteke.
        // note: kako bi zlivali, ce bi v datoteki lahko imeli tudi negativna stevila.
    
        // &(*(buffer + iMin))
        if (fscanf(sources[iMin], "%d", &buffer[iMin]) < 1)
        {
            buffer[iMin] = -1;
            completed++;
        }
    }

    // 5 - clean-up
    free(buffer);
    close_src_files(sources, n);
    free(sources);
    free(target_fname);
    fclose(target);

    return 0;
}

