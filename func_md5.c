#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Les constantes :
#define MAXLENGTH 56
#define BLOCKLENGTH 512
#define SIZEBYTE 8
#define BASE2 2
#define NBCHARMAX 56
#define BASE16 16
#define SIZETABLE 64
#define NBBLOCKS 16
#define SIZEBLOCK 32

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Les structures de données utilisées :
struct listBlocks
{
	char **blocks;
	int nb_blocks;
	int size_block;

}listBlocks;

struct tableR
{
	long int *table;
	int nb_elems;
}tableR;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Les fonctions permettant de gérer la structure struct listBlocks :
struct listBlocks *initListBlocks(int nb_blocks, int size_block)
{
	struct listBlocks * lb = (struct listBlocks *) malloc(sizeof(struct listBlocks));
	lb->nb_blocks = nb_blocks;
	lb->size_block = size_block;
	lb->blocks = (char **) malloc(nb_blocks * sizeof(char *));
	for(int i = 0; i < nb_blocks; i++)
	{
		lb->blocks[i] = (char *) malloc(size_block * sizeof(char));
	}

	for(int j = 0; j < nb_blocks; j++)
	{
		for(int i = 0; i < size_block; i++)
		{
			lb->blocks[j][i] = '0';
		}
	}
	return lb;
}

void freeStructListBlock(struct listBlocks *lb)
{
	for(int i = 0; i < (lb->nb_blocks); i++)
	{
		free(lb->blocks[i]);
		lb->blocks[i] = NULL;
	}
	free(lb->blocks);
	lb->blocks = NULL;

	lb->size_block = -1;
	lb->nb_blocks = -1;

	free(lb);
	lb = NULL;
}

void showStructListBlocks(struct listBlocks *lb)
{
	printf("\n\nLa liste est composée de %d blocs de %d bits\n", lb->nb_blocks, lb->size_block);
	for(int i = 0; i < (lb->nb_blocks); i++)
	{
		printf("Liste n°%d : ", i + 1);
		for(int j = 0; j < (lb->size_block); j++)
		{
			printf("%c ", lb->blocks[i][j]);
		}
		printf("\n");
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Les fonctions permettant de gérer la structure struct tableR :

//fonction qui génére une structure TableR vide :
struct tableR *initStructTableR()
{
	struct tableR *t = (struct tableR *) malloc(sizeof(struct tableR));
	t->nb_elems = SIZETABLE;
	t->table = (long int *) malloc(SIZETABLE * sizeof(long int));
	return t;
}

//fonction qui génére la variable r contenant les blocs message qui est un tableau de taille 64
struct tableR *initStructTableBlocMessage()
{
	struct tableR *t = initStructTableR();
	
	for(int i = 0; i < 16; i= i + 4)
	{	
		t->table[i] = 7;
		t->table[i + 1] = 12;
		t->table[i + 2] = 17;
		t->table[i + 3] = 22;
		
	}

	for(int i = 16; i < 32; i= i + 4)
	{	
		t->table[i] = 5;
		t->table[i + 1] = 9;
		t->table[i + 2] = 14;
		t->table[i + 3] = 20;
		
	}

	for(int i = 32; i < 48; i= i + 4)
	{	
		t->table[i] = 4;
		t->table[i + 1] = 11;
		t->table[i + 2] = 16;
		t->table[i + 3] = 23;
		
	}

	for(int i = 48; i < SIZETABLE; i= i + 4)
	{	
		t->table[i] = 6;
		t->table[i + 1] = 10;
		t->table[i + 2] = 15;
		t->table[i + 3] = 21;
		
	}
	return t;
}


//fonction qui génére le tableau k :
struct tableR *generateSinusEntiers()
{
	struct tableR *k = initStructTableR();
	double temp;
	double res;
	double power = pow(2, 32);


	for(int i = 0; i < (k->nb_elems); i++)
	{
		temp = sin(i + 1);
		if(temp < 0)
		{
			temp = (-1) * temp;
		}
		res = temp * power;
		k->table[i] = (long int) res;
	}

	return k;
}

void freeStructTableR(struct tableR *t)
{
	t->nb_elems = -1;
	free(t->table);
	t->table = NULL;
	free(t);
	t = NULL;
}

void showStructTableR(struct tableR *t)
{
	printf("\n\n Le tableau est composé de %d éléments\n", t->nb_elems);
	for(int i = 0; i < (t->nb_elems); i++)
	{
		printf("%ld ", t->table[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fonction qui convertit un entier en binaire sur 8 bits (1 octet)
char *convertIntToBin(int ascii)
{
	if(ascii < 256)
	{
		char *block = (char *) malloc((SIZEBYTE + 1) * sizeof(char));
		block[SIZEBYTE] = '\0';
		for(int i = 0; i < SIZEBYTE; i++)
		{
			block[i] = '0';
		}

		int q, r;
		q = ascii;
		int i = SIZEBYTE - 1;
		while(q != 0)
		{
			r = q % BASE2;
			block[i] = '0' + r;
			q = q / 2;
			i = i - 1;

		}
		return block;
		
	}
	else if(ascii <= 448)
	{
		char *block = (char *) malloc((SIZEBYTE + 2) * sizeof(char));
		block[SIZEBYTE + 1] = '\0';
		for(int i = 0; i < SIZEBYTE + 1; i++)
		{
			block[i] = '0';
		}

		int q, r;
		q = ascii;
		int i = SIZEBYTE;
		while(q != 0)
		{
			r = q % BASE2;
			block[i] = '0' + r;
			q = q / 2;
			i = i - 1;

		}
		return block;
	}
	
		
}

//fonction qui convertit un mot de passe en binaire sur 512 bits : message || padding || (64 bits pour la taille du message)
char *convertStringToMD5Block(char *str)
{
	char *md5_block = (char *) malloc((BLOCKLENGTH + 1) * sizeof(char));
	md5_block[BLOCKLENGTH] = '\0';

	int length_str = strlen(str);
	int ascii_of_char;
	int j, k;
	k = 0;

	for(int i = 0; i < BLOCKLENGTH; i++)
	{
		md5_block[i] = '0';
	}


	for(int i = 0; i < length_str; i++)
	{	
		ascii_of_char = (int) (str[i]);
		char *char_temp = convertIntToBin(ascii_of_char);
		j = 0;

		while((j < SIZEBYTE) && (k < NBCHARMAX * SIZEBYTE))
		{
			md5_block[k] = char_temp[j];
			k = k + 1;
			j = j + 1;
		}
		free(char_temp);
		char_temp = NULL;
	}

	//Ajout du padding '1' + '00000.....0000' dans le cas ou le message ne remplit pas les 448 bits (512-64 = 448)
	if((length_str * SIZEBYTE) < (NBCHARMAX * SIZEBYTE))
	{
		md5_block[k] = '1';
		for(int i = k + 1; i < (NBCHARMAX *SIZEBYTE); i++)
		{
			md5_block[i] = '0';
		}

	}

	//Traitement de la dernière partie relative à la taille du message
	char *char_to_bin = (char *) malloc(SIZEBYTE * sizeof(char));
	sprintf(char_to_bin, "%X", length_str * SIZEBYTE);
	int hexa_size = strlen(char_to_bin);
	int str_char = strlen(char_to_bin);

/*
	//(length_str * SIZEBYTE) <=  256 
	if(hexa_size % 2 == 0)
	{
		char *char_temp = convertIntToBin(length_str * SIZEBYTE);
		j = SIZEBYTE - 1;
		for(int i = BLOCKLENGTH - 1; i >= (BLOCKLENGTH - SIZEBYTE); i--)
		{
			md5_block[i] = char_temp[j];
			j = j - 1;
		}
		
		free(char_temp);
		char_temp = NULL;
	}
	else //(length_str * SIZEBYTE) > 256
	{
		char *char_temp = convertIntToBin(length_str * SIZEBYTE);
		j = SIZEBYTE;
		for(int i = BLOCKLENGTH - 1; i >= (BLOCKLENGTH - SIZEBYTE -  1); i--)
		{
			md5_block[i] = char_temp[j];
			j = j - 1;
		}
		free(char_temp);
		char_temp = NULL;
	}
*/

	if(hexa_size % 2 == 0)
	{
		char *char_temp = convertIntToBin(length_str * SIZEBYTE);
		j = 0;
		for(int i = NBCHARMAX * SIZEBYTE; i < NBCHARMAX * SIZEBYTE + SIZEBYTE; i++)
		{
			md5_block[i] = char_temp[j];
			j = j + 1;
		}
		
		for(int i = NBCHARMAX * SIZEBYTE + SIZEBYTE; i < BLOCKLENGTH; i++)
		{
			md5_block[i] = '0';
		}
		free(char_temp);
		char_temp = NULL;
	}
	else
	{
		char *char_temp = convertIntToBin(length_str * SIZEBYTE);
		j = 0;
		for(int i = NBCHARMAX * SIZEBYTE + 7; i < NBCHARMAX * SIZEBYTE + 16; i++)
		{
			md5_block[i] = char_temp[j];
			j = j + 1;
		}
		
		for(int i = NBCHARMAX * SIZEBYTE + 16; i < BLOCKLENGTH; i++)
		{
			md5_block[i] = '0';
		}
		free(char_temp);
		char_temp = NULL;
	}	


	free(char_to_bin);
	char_to_bin = NULL;
	return md5_block;

}


void blockToLittleEndian(char *ch)
{
	char *char_temp = (char *) malloc((SIZEBLOCK + 1) * sizeof(char));
	char_temp[SIZEBLOCK] = '\0';
	int k = 0, p = 0;

	for(int i = 3; i >= 0; i--)
	{
		for(int j = 0 + k * SIZEBYTE; j < SIZEBYTE + k * SIZEBYTE; j++)
		{
			char_temp[j] = ch[i * SIZEBYTE + p];
			p = p + 1;
		}
		k = k + 1;
		p = 0;
	}

	for(int i = 0; i < SIZEBLOCK; i++)
	{
		ch[i] = char_temp[i];
	}
	free(char_temp);
	char_temp = NULL;

}

//fonction qui découpe le message de 512 bits en 16 blocks de 32 bits 
struct listBlocks *coupageBlockMD5(char *md5_block)
{
	struct listBlocks *lb = initListBlocks(NBBLOCKS, SIZEBLOCK);

	int i = 0, k = 0, p = 0;
	for(int i = 0; i < NBBLOCKS; i++)
	{	
		for(int j = k; j < k + SIZEBLOCK; j++)
		{
			lb->blocks[i][p] = md5_block[j];
			p = p + 1;
		}
		k = k + SIZEBLOCK;
		p = 0;
	}

	for(int i = 0; i < (lb->nb_blocks); i++)
	{
		blockToLittleEndian(lb->blocks[i]);
	}

	return lb;
}

struct listBlocks *initializeABCD()
{
	struct listBlocks *lb = initListBlocks(4, SIZEBLOCK);
	
	//h0 := 0x67 45 23 01
	lb->blocks[0][1] = '1';
	lb->blocks[0][2] = '1';
	lb->blocks[0][5] = '1';
	lb->blocks[0][6] = '1';
	lb->blocks[0][7] = '1';
	lb->blocks[0][9] = '1';
	lb->blocks[0][13] = '1';
	lb->blocks[0][15] = '1';
	lb->blocks[0][18] = '1';
	lb->blocks[0][22] = '1';
	lb->blocks[0][23] = '1';
	lb->blocks[0][31] = '1';

	//h1 := 0xEF CD AB 89
	lb->blocks[1][0] = '1';
	lb->blocks[1][1] = '1';
	lb->blocks[1][2] = '1';
	lb->blocks[1][4] = '1';
	lb->blocks[1][5] = '1';
	lb->blocks[1][6] = '1';
	lb->blocks[1][7] = '1';
	lb->blocks[1][8] = '1';
	lb->blocks[1][9] = '1';
	lb->blocks[1][12] = '1';
	lb->blocks[1][13] = '1';
	lb->blocks[1][15] = '1';
	lb->blocks[1][16] = '1';
	lb->blocks[1][18] = '1';
	lb->blocks[1][20] = '1';
	lb->blocks[1][22] = '1';
	lb->blocks[1][23] = '1';
	lb->blocks[1][24] = '1';
	lb->blocks[1][28] = '1';
	lb->blocks[1][31] = '1';

	//h2 := 0x98 BA DC FE
	lb->blocks[2][0] = '1';	
	lb->blocks[2][3] = '1';
	lb->blocks[2][4] = '1';
	lb->blocks[2][8] = '1';
	lb->blocks[2][10] = '1';
	lb->blocks[2][11] = '1';
	lb->blocks[2][12] = '1';
	lb->blocks[2][14] = '1';
	lb->blocks[2][16] = '1';
	lb->blocks[2][17] = '1';
	lb->blocks[2][19] = '1';
	lb->blocks[2][20] = '1';
	lb->blocks[2][21] = '1';
	lb->blocks[2][24] = '1';
	lb->blocks[2][25] = '1';
	lb->blocks[2][26] = '1';
	lb->blocks[2][27] = '1';
	lb->blocks[2][28] = '1';
	lb->blocks[2][29] = '1';
	lb->blocks[2][30] = '1';

	//h3 := 0x10 32 54 76
	lb->blocks[3][3] = '1';
	lb->blocks[3][10] = '1';
	lb->blocks[3][11] = '1';
	lb->blocks[3][14] = '1';
	lb->blocks[3][17] = '1';
	lb->blocks[3][19] = '1';
	lb->blocks[3][21] = '1';
	lb->blocks[3][25] = '1';
	lb->blocks[3][26] = '1';
	lb->blocks[3][27] = '1';
	lb->blocks[3][29] = '1';
	lb->blocks[3][30] = '1';
	return lb;
}


char ANDoperator(char a, char b)
{
	if((a == '0') || (b == '0'))
	{
		return '0';
	}
	return '1';
	

}

char NOToperator(char a)
{
	if(a == '0')
	{
		return '1';
	}
	return '0';
}

char XOROperator(char a, char b)
{
	if(a == b)
	{
		return '0';
	}
	return '1';
}

char OROperator(char a, char b)
{
	if((a == '1') || (b == '1'))
	{
		return '1';
	}
	return '0';
}

void leftRotate(char *ch, long int dec)
{
	char *char_temp = (char *) malloc((SIZEBLOCK + 1) * sizeof(char));
	char_temp[SIZEBLOCK] = '\0';

	int j = 0;
	for(int i = dec; i < SIZEBLOCK; i++)
	{
		char_temp[j] = ch[i];
		j = j + 1;
	}

	for(int i = 0; i < dec; i++)
	{
		char_temp[j] = ch[i];
		j = j + 1;
	}

	for(int i = 0; i < SIZEBLOCK; i++)
	{
		ch[i] = char_temp[i];
	}

	
	free(char_temp);
	char_temp = NULL;
}

long int getDecimalValueOfSIZEBLOCK(char *ch)
{
	long int res = 0, power = 0;
	int j = 0;

	for(int i = SIZEBLOCK - 1; i >= 0; i--)
	{
		power = pow(2, j);
		if(ch[i] == '1')
		{
			res = res + 1 * power;
		}
		else
		{
			res = res + 0 * power;
		}
		j = j + 1;
	}
	return res;
}


char * convertDecimalToSizeblock(long int decimal)
{

	char *block = (char *) malloc((SIZEBLOCK + 1) * sizeof(char));
	block[SIZEBLOCK] = '\0';
	for(int i = 0; i < SIZEBLOCK; i++)
	{
		block[i] = '0';
	}

	long int q = decimal;
	int r;
	int i = SIZEBLOCK - 1;
	while((q != 0) && (i >= 0))
	{
		r = q % 2;
		block[i] = '0' + r;
		i = i - 1;
		q = q / 2;
	}
	
	
	return block;
}


struct listBlocks *md5Function(char *str)
{
	//Définir r comme suit 
	struct tableR *r = initStructTableBlocMessage(); //{7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22, ....., 21}  tableau de 64 éléments utilisé pour les leftRotate
	//showStructTableR(r);

	//MD5 utilise des sinus d'entiers pour ses constantes :
	struct tableR *k = generateSinusEntiers();
	//showStructTableR(k);
		
	//Préparation des variables :
	struct listBlocks *var = initializeABCD();  // var->table[0] = 0x0x67452301   var->table[1] = 0xEFCDAB89  var->table[2] = 0x98BADCFE  var->table[3] = 0x10325476

	//Préparation du message (padding) :
	char *md5_block = convertStringToMD5Block(str);

	//Découpage en 16 blocs de 32 bits
	struct listBlocks *lb = coupageBlockMD5(md5_block);


	//initialiser les valeurs de hachage :
	struct listBlocks *val_hachage = initializeABCD();
	char *funcF = (char *) malloc(SIZEBLOCK * sizeof(char));
	int g = 0;;

	//boucle principale 
	for(int i = 0; i < SIZETABLE; i++)
	{
		if((i >= 0) && (i <= 15))
		{
			for(int i = 0; i < SIZEBLOCK; i++)
			{
				//f := (b et c) ou ((non b) et d)

				funcF[i] = OROperator(ANDoperator(var->blocks[1][i], var->blocks[2][i]), ANDoperator(NOToperator(var->blocks[1][i]), var->blocks[3][i]));
			}
		}
		else if((i >= 16) && (i <= 31))
		{
			for(int i = 0; i < SIZEBLOCK; i++)
			{
				// f := (d et b) ou ((non d) et c)

				funcF[i] = OROperator(ANDoperator(var->blocks[3][i], var->blocks[1][i]), ANDoperator(NOToperator(var->blocks[3][i]), var->blocks[2][i]));
				g = (5 * i + 1) % 16;
			}
		}

		else if((i >= 32) && (i <= 47))
		{
			for(int i = 0; i < SIZEBLOCK; i++)
			{
				// f := b xor c xor d

				funcF[i] =  XOROperator(XOROperator(var->blocks[1][i], var->blocks[2][i]), var->blocks[3][i]);
				g = (3 * i + 5) % 16;
			}
		}


		else if((i >= 48) && (i <= 63))
		{
			for(int i = 0; i < SIZEBLOCK; i++)
			{
			//            f := c xor (b ou (non d))

				funcF[i] =  XOROperator(var->blocks[2][i], OROperator(var->blocks[1][i], NOToperator(var->blocks[3][i])));
				g = (7 * i) % 16;
			}
		}

		if((i >= 0) && (i <= 15))
		{
			g = i;
		}
		else if((i >= 16) && (i <= 31))
		{
			g = (5 * i + 1) % 16;

		}
		else if ((i >= 32) && (i <= 47))
		{
			g = (3 * i + 5) % 16;
		}
		else if((i >= 48) && (i <= 63))
		{
			g = (7 * i) % 16;

		}
		
		char *char_temp = (char *) malloc((SIZEBLOCK + 1) * sizeof(char));
		char_temp[SIZEBLOCK] = '\0';
		for(int i = 0; i < SIZEBLOCK; i++)
		{
			char_temp[i] = var->blocks[3][i];
		}
		for(int i = 0; i < SIZEBLOCK; i++)
		{
			var->blocks[3][i] = var->blocks[2][i];
		}
		for(int i = 0; i < SIZEBLOCK; i++)
		{
			var->blocks[2][i] = var->blocks[1][i];
		}

		long int val_b = getDecimalValueOfSIZEBLOCK(var->blocks[1]);
		int ri = r->table[i];
		long int wg = getDecimalValueOfSIZEBLOCK(lb->blocks[g]);
		long int ki = k->table[i];
		long int val_f = getDecimalValueOfSIZEBLOCK(funcF);
		long int val_a = getDecimalValueOfSIZEBLOCK(var->blocks[0]);
		long int val_temp = val_a + val_f + ki + wg;
		char *lf_b = convertDecimalToSizeblock(val_temp);
		leftRotate(lf_b, ri);
		long int new_val_b = getDecimalValueOfSIZEBLOCK(lf_b) + val_b;
		char *char_b = convertDecimalToSizeblock(new_val_b);

		for(int i = 0; i < SIZEBLOCK; i++)
		{
			var->blocks[1][i] = char_b[i];
		}

		for(int i = 0; i < SIZEBLOCK; i++)
		{
			var->blocks[0][i] = char_temp[i];
		}

		free(char_temp);
		char_temp = NULL;
		free(lf_b);
		lf_b = NULL;
		free(char_b);
		char_b = NULL;

	}

	long int h0 = getDecimalValueOfSIZEBLOCK(val_hachage->blocks[0]);
	long int h1 = getDecimalValueOfSIZEBLOCK(val_hachage->blocks[1]);
	long int h2 = getDecimalValueOfSIZEBLOCK(val_hachage->blocks[2]);
	long int h3 = getDecimalValueOfSIZEBLOCK(val_hachage->blocks[3]);

	long int val_a = getDecimalValueOfSIZEBLOCK(var->blocks[0]);
	long int val_b = getDecimalValueOfSIZEBLOCK(var->blocks[1]);
	long int val_c = getDecimalValueOfSIZEBLOCK(var->blocks[2]);
	long int val_d = getDecimalValueOfSIZEBLOCK(var->blocks[3]);

	h0 = h0 + val_a;
	h1 = h1 + val_b;
	h2 = h2 + val_c;
	h3 = h3 + val_d;

	val_hachage->blocks[0] = convertDecimalToSizeblock(h0);
	val_hachage->blocks[1] = convertDecimalToSizeblock(h1);
	val_hachage->blocks[2] = convertDecimalToSizeblock(h2);
	val_hachage->blocks[3] = convertDecimalToSizeblock(h3);

	freeStructTableR(r);
	freeStructTableR(k);
	freeStructListBlock(var);
	free(md5_block);
	md5_block = NULL;
	freeStructListBlock(lb);
	free(funcF);
	funcF = NULL;
	return val_hachage;

}

void hexaToLittleEndian(char *ch)
{
	char *temp = (char *) malloc((SIZEBYTE + 1) * sizeof(char));
	temp[SIZEBYTE] = '\0';
	int j = 0;
	for(int k = 3; k >= 0; k--)
	{
		temp[j] = ch[2 * k];
		temp[j + 1] = ch[2 * k + 1];
		j = j + 2;
	}

	for(int i = 0; i < SIZEBYTE; i++)
	{
		ch[i] = temp[i];
	}
	free(temp);
	temp = NULL;
}

char *getTheHash(struct listBlocks *val_hachage)
{
	char *res = (char *) malloc((SIZEBLOCK * 4 + 1) * sizeof(char));
	res[SIZEBLOCK * 4] = '\0';

	char *ch1 = malloc((SIZEBLOCK + 1) * sizeof(char));
	ch1[SIZEBLOCK] = '\0';

	sprintf(ch1, "%X", (unsigned int) getDecimalValueOfSIZEBLOCK(val_hachage->blocks[0]));
	hexaToLittleEndian(ch1);
	int j = 0;
	for(int i = 0; i < SIZEBYTE; i++)
	{
		res[j] = ch1[i];
		j = j + 1;
	}


	sprintf(ch1, "%X", (unsigned int) getDecimalValueOfSIZEBLOCK(val_hachage->blocks[1]));
	hexaToLittleEndian(ch1);
	j = SIZEBYTE;
	for(int i = 0; i < SIZEBYTE; i++)
	{
		res[j] = ch1[i];
		j = j + 1;
	}

	sprintf(ch1, "%X", (unsigned int) getDecimalValueOfSIZEBLOCK(val_hachage->blocks[2]));
	hexaToLittleEndian(ch1);
	j = SIZEBYTE * 2;
	for(int i = 0; i < SIZEBYTE; i++)
	{
		res[j] = ch1[i];
		j = j + 1;
	}

	sprintf(ch1, "%X", (unsigned int) getDecimalValueOfSIZEBLOCK(val_hachage->blocks[3]));
	hexaToLittleEndian(ch1);
	j = SIZEBYTE * 3;
	for(int i = 0; i < SIZEBYTE; i++)
	{
		res[j] = ch1[i];
		j = j + 1;
	}

	return res;
}

int main()
{
char* str = "cryptis";
struct listBlocks *val_hachage = md5Function(str);
char *hash = getTheHash(val_hachage);
printf("Input : %s\n", str);
printf("MD5 Hash : %s\n", getTheHash(val_hachage));


//Libération de la mémoire :
freeStructListBlock(val_hachage);


return EXIT_SUCCESS;

}