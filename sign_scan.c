#include <stdio.h>
#include <stddef.h>

int isExe(FILE* f)
{
    unsigned char format_magic[2];
    int result;

    if (f == NULL)
    {
        printf("Error: NULL file pointer passed to isExe.\n");
        return 1;
    }


    if (fread(format_magic, sizeof(format_magic[0]), sizeof(format_magic)/sizeof(format_magic[0]), f) != sizeof(format_magic)/sizeof(format_magic[0]))
    {
        printf("Error: Failed to read file header.\n");
        return 2;
    }

    if (format_magic[0] != 'M' || format_magic[1] != 'Z')
    {
        if (printf("This file is not an EXE.\n") < 0)
        {
            printf("Output Error.\n");
            return 3;
        }
        return 4;
    }
    return 0;
}

int isSizeEnough(FILE* f, long offset, size_t signature_size)
{
    long size;
    int result;

    if (f == NULL)
    {
        printf("Error: NULL file pointer passed to isSizeEnough.\n");
        return 1;
    }
    if (offset < 0)
    {
        printf("Error: Negative offset passed to isSizeEnough.\n");
        return 2;
    }
    if (signature_size == 0)
    {
        printf("Error: Zero-length signature passed to isSizeEnough.\n");
        return 3;
    }
    if (fseek(f, 0, SEEK_END) != 0)
    {
        printf("Error: Failed to seek to end of file.\n");
        return 4;
    }

    size = ftell(f);
    if (size == -1)
    {
        printf("Error: Failed to get file size.\n");
        return 5;
    }

    if (size < offset + (long)signature_size)
    {
        if (printf("File is too small: signature would go out of bounds.\n") < 0)
        {
            printf("Output Error.\n");
            return 6;
        }
        return 7;
    }
    return 0;
}

int main()
{
    char pathchecked[100];
    char infopath[100];
    FILE* checkedFile;
    FILE* infofile;
    unsigned char sign[8];
    unsigned char buffer[8];
    long int offset;
    int result;
    size_t i;
    char signature_name[100];
    int isExeResult;
    int isSizeEnoughResult;

    if (printf("Enter path to file to check:\n") < 0)
    {
        printf("Output error.\n");
        return 1;
    }
    if (scanf("%s", pathchecked) != 1)
    {
        printf("Error: Failed to read file path.\n");
        return 2;
    }

    if (printf("Enter path to info file:\n") < 0)
    {
        printf("Output error.\n");
        return 1;
    }
    if (scanf("%s", infopath) != 1)
    {
        printf("Error: Failed to read info file path.\n");
        return 2;
    }

    checkedFile = fopen(pathchecked, "rb");
    if (checkedFile == NULL)
    {
        printf("Error opening checked file.\n");
        return 3;
    }

    infofile = fopen(infopath, "r");
    if (infofile == NULL)
    {
        printf("Error opening info file.\n");
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        return 3;
    }

    isExeResult = isExe(checkedFile);
    if (isExeResult != 0)
    {
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 4;
        }
        result = fclose(infofile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        switch (isExeResult)
        {
            case 1:
                return 9;
                break;
            case 2:
                return 7;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 8;
                break;
        }
    }

    result = fscanf(infofile, "%s", signature_name);
    if (result != 1)
    {
        printf("Error: Failed to read signature name from file.\n");
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 4;
        }
        result = fclose(infofile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        return 5;
    }


    result = fscanf(infofile, "%lx", &offset);
    if (result != 1)
    {
        printf("Error: Failed to read offset from file.\n");
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 4;
        }
        result = fclose(infofile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        return 5;
    }
    for(i = 0; i < sizeof(sign); i++)
    {
        result = fscanf(infofile, "%hhx", &sign[i]);
        if (result != 1)
        {
            printf("Error: Failed to read 8-byte signature from file.\n");
            result = fclose(checkedFile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                result = fclose(infofile);
                if (result != 0)
                {
                    printf("Error closing file.\n");
                    return 4;
                }
                return 4;
            }
            result = fclose(infofile);
            if (result != 0)
            {
            printf("Error closing file.\n");
            return 4;
            }
            return 5;
        }
    }


    isSizeEnoughResult = isSizeEnough(checkedFile, offset, sizeof(sign));
    if (isSizeEnoughResult != 0)
    {
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 4;
        }
        result = fclose(infofile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        switch(isSizeEnoughResult)
        {
            case 1:
                return 10;
                break;
            case 2:
                return 11;
                break;
            case 3:
                return 12;
                break;
            case 4:
                return 6;
                break;
            case 5:
                return 13;
                break;
            case 6:
                return 1;
                break;
            case 7:
                return 8;
                break;
        }
    }

    if (fseek(checkedFile, offset, SEEK_SET) != 0)
    {
        printf("Error: Failed to seek to offset.\n");
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 4;
        }
        result = fclose(infofile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        return 6;
    }
    if (fread(buffer, sizeof(buffer[0]), sizeof(buffer)/sizeof(buffer[0]), checkedFile) != sizeof(buffer)/sizeof(buffer[0]))
    {
        printf("Error: Could not read from checked file.\n");
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 4;
        }
        result = fclose(infofile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        return 7;
    }

    for (i = 0; i < sizeof(buffer)/sizeof(buffer[0]); i++)
    {
        if (buffer[i] != sign[i])
        {
            if (printf("No signature match. You can open your file.\n") < 0)
            {
                printf("Output Error.\n");
                result = fclose(checkedFile);
                if (result != 0)
                {
                    printf("Error closing file.\n");
                    result = fclose(infofile);
                    if (result != 0)
                    {
                        printf("Error closing file.\n");
                        return 4;
                    }
                    return 4;
                }
                result = fclose(infofile);
                if (result != 0)
                {
                    printf("Error closing file.\n");
                    return 4;
                }
                return 1;
            }
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                result = fclose(checkedFile);
                if (result != 0)
                {
                    printf("Error closing file.\n");
                    return 4;
                }
                return 4;
            }
            result = fclose(checkedFile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 8;
        }
    }


    if (printf("Don't open your file. Signature matched: %s\n", signature_name) < 0)
    {
        printf("Output Error.\n");
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            result = fclose(infofile);
            if (result != 0)
            {
                printf("Error closing file.\n");
                return 4;
            }
            return 4;
        }
        result = fclose(infofile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        return 1;
    }
    result = fclose(infofile);
    if (result != 0)
    {
        printf("Error closing file.\n");
        result = fclose(checkedFile);
        if (result != 0)
        {
            printf("Error closing file.\n");
            return 4;
        }
        return 4;
    }
    result = fclose(checkedFile);
    if (result != 0)
    {
        printf("Error closing file.\n");
        return 4;
    }
    return 0;
}
