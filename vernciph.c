/*
 * One-time pad (aka Vernam Cipher) implementation for *NIX-like OS
 *
 * Copymeow by xxkfqz <xxkfqz@gmail.com> 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>

#define DATA_BUFFER_SIZE 64

char *pathToKey = NULL;

void printHelp(void);
void processData(char *pathToKey, int blockSize);
int checkKeyFileExists(const char *pathToFile);

int main(int argc, char *argv[])
{
        struct option arguments[] =
        {
                {"block-size", 1, NULL, 'b'},
                {"help", 0, NULL, 'h'},
                {"key-file", 1, NULL, 'k'},
                {NULL, 0, NULL, 0}
        };

        int ch, blockSize = DATA_BUFFER_SIZE;
        char *optString = "b:hk:";
        while((ch = getopt_long(argc, argv, optString, arguments, NULL)) != -1)
        {
                switch(ch)
                {
                        case 'b':
                        {
                                blockSize = atoi(optarg);
                                break;
                        }

                        case 'h':
                        {
                                printHelp();
                                exit(EXIT_SUCCESS);
                                break;
                        }

                        case 'k':
                        {
                                pathToKey = optarg;
                                break;
                        }

                        default:
                        case '?':
                        {
                                exit(EXIT_FAILURE);
                                break;
                        }
                }
        }

        processData(pathToKey, blockSize);

        return EXIT_SUCCESS;
}

void printHelp(void)
{
        fprintf(stderr,
            "Parameters:\n"
            "  -b, --block-size <data-block-size>\n"
            "  -h, --help\n"
            "  -k, --key-file <key>\n\n"
            "Encrypting\n    vernciph -e -k /path/to/input/key/file < /path/to/input/file > /path/to/output/file\n"
                        "    vernciph -e < /path/to/input/file > /path/to/output/file 2> /path/to/key\n"
            "Decrypting\n    vernciph -d -k /path/to/key/file < /path/to/encrypted/file > /path/to/decrypted/file\n"
            "\nIf key file is not exists it will be created at specified path and data will be readed from /dev/random\n"
            "\nxxkfqz <xxkfqz@gmail.com> 2020\n"
        );
}

void processData(char *pathToKey, int blockSize)
{
        char buffer[blockSize], keyBuffer[blockSize];
        char *pathToKeyFile = pathToKey;

        short isKeyExists = checkKeyFileExists(pathToKey);
        if(!isKeyExists)
                pathToKeyFile = "/dev/random";

        FILE *keyFile = fopen(pathToKeyFile, "rb");
        if(keyFile == NULL)
        {
                fprintf(stderr, "Cannot open key file!\n");
                exit(EXIT_FAILURE);
        }

        int bytesReaded = 0, bufPos;
        while((bytesReaded = read(STDIN_FILENO, buffer, blockSize)) > 0)
        {
                fread(keyBuffer, sizeof(keyBuffer), 1, keyFile);
                for(bufPos = 0; bufPos < blockSize; bufPos++)
                        buffer[bufPos] ^= keyBuffer[bufPos];

                if(!isKeyExists)
                        write(STDERR_FILENO, keyBuffer, bytesReaded);

                write(STDOUT_FILENO, buffer, bytesReaded);
        }

        fclose(keyFile);
}

int checkKeyFileExists(const char *pathToFile)
{
        if(pathToFile == NULL)
                return 0;

        struct stat buffer;
        return !stat(pathToFile, &buffer);
}
