#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/*
    Marc Marti
    CIS 3360
    Hill Cipher Project
    Date - 02/28/2017
*/


int main(int argc, char** argv)
{
    printf("%s %s %s\n\n\nKey Matrix:\n\n", argv[0], argv[1], argv[2]);

    char *msgFile = argv[1];
    char *keyFile = argv[2];

    FILE *mp;
    FILE *kp;

    // message = original text. letters = letters being multiplied to the key each iteration
    // product = product of key and letter matrices. cText = cypher text
    char *message, *letters, *product, *cText;
    char c;
    int matrixSize, pad, i, j, k, sum=0, n=0;

    //2d key matrix
    int **matrix = NULL;

    // attempt to open the key file
    kp = fopen(keyFile, "r");
    if (kp == NULL) printf("\ncould not open the key file\n\n"), exit(1);

    // store the matrix size
    fscanf(kp, "%d", &matrixSize);

    // create the 2d key matrix
    matrix = (int **)malloc(matrixSize * sizeof(int *));
    for (i=0; i<matrixSize; i++)
         matrix[i] = (int *)malloc(matrixSize * sizeof(int));

    // create the letter matrix the key will be multiplied to
    letters = malloc(sizeof(char) * matrixSize);

    // create the product matrix where each cipher product is stored
    product = malloc(sizeof(char) * matrixSize);

    // store the key into the matrix
    for (i = 0; i <  matrixSize; i++)
      for (j = 0; j < matrixSize; j++)
         fscanf(kp, "%d", &matrix[i][j]);

    // closing the key file
    fclose(kp); kp = NULL;

    // print the key matrix
    for (i = 0; i <  matrixSize; i++)
    {
      for (j = 0; j < matrixSize; j++)
         printf("%d ", matrix[i][j]);
      printf("\n");
    }

    printf("\n\nPlaintext:\n\n");

    /////////////////////////////////////////////

    // attempt to open the message file safely
    mp = fopen(msgFile, "r");
    if (mp == NULL)
    {
        printf("\ncould not open the message file\n\n");
        free(letters); free(product);

        for (i=0; i<matrixSize; i++)
        free(matrix[i]);

        free(matrix);
    }

    // allocate memory to store the original message
    message = calloc(10000, sizeof(char));

    // allocate memory to store the cypher text
    cText = calloc(10000, sizeof(char));

    // store only the characters into the message array while converting
    // uppercase letters to lowercase
    while ((c = fgetc(mp)) != EOF)
    {
        if(!isalpha(c)) continue;

        else if(isupper(c))
            c = c + 32;

        message[n++] = c;
    }

    fclose(mp); mp = NULL;

    // calculate how much 'X' padding the message requires
    pad = matrixSize - (n % matrixSize);

    // pad the end of the message with the required X's
    if(pad == matrixSize){;}
    else
    {
        for(i=0; i<pad; i++)
        message[n++] = 'x';
    }

    // terminate the message
    //message[n+1] = '\0';

    // print plain text
    for(i=0; message[i] != 0; i++)
    {
        printf("%c", message[i]);

        if((i+1)%80 == 0)
            printf("\n");
    }

    printf("\n\n\nCiphertext:\n\n");

    // convert original text into cipher text
    // piece by piece
    for(n=0; message[n] != '\0'; n+=matrixSize)
    {
        // store the letters to be converted sequentially
        for(i=0; i < matrixSize; i++)
        {
            letters[i] = message[n+i];
        }

        // calculate hill cipher
        for (i = 0; i < matrixSize; i++)
        {
            for (k = 0; k < matrixSize; k++)
            {
                sum = sum + matrix[i][k]*(letters[k] - 97);
            }

            product[i] = sum%26;
            sum = 0;
        }

        // store the calculated cipher chunk into cText
        for(i=0; i<matrixSize; i++)
        {
            cText[n+i] = 97 + product[i];
        }
    }

    // terminate the cipher text
    //cText[n] = '\0';

    for(i=0; cText[i] != 0; i++)
    {
        printf("%c", cText[i]);

        if((i+1)%80 == 0)
            printf("\n");
    }

    free(message);
    free(letters);
    free(cText);
    free(product);

    for (i=0; i<matrixSize; i++)
        free(matrix[i]);

    free(matrix);

    return 0;
}
