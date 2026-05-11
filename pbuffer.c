#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define GROWTH_ADDITIVE_FACTOR 32
#define PERSON_N (((int *)pBuffer)[0])
#define LEN      (((int *)pBuffer)[1])
#define CAPACITY (((int *)pBuffer)[2])
#define TMP_0    (((int *)pBuffer)[3])
#define TMP_1    (((int *)pBuffer)[4])
#define TMP_2    (((int *)pBuffer)[5])
#define TMP_3    (((int *)pBuffer)[6])
#define OFFSET  (7 * sizeof( int ))

void *pBuffer = NULL;

void GrowInt( void ) {
	if ( LEN + sizeof( int ) >= CAPACITY ) {
		CAPACITY += GROWTH_ADDITIVE_FACTOR;
		pBuffer = realloc( pBuffer, CAPACITY );
	}
}

void GrowByte( void ) {
	if ( LEN + 1 >= CAPACITY ) {
		CAPACITY += GROWTH_ADDITIVE_FACTOR;
		pBuffer = realloc( pBuffer, CAPACITY );
	}
}

void Ungrow( void ) {
	TMP_1 = (LEN + GROWTH_ADDITIVE_FACTOR - 1); /* new capacity */
	TMP_1 /= GROWTH_ADDITIVE_FACTOR;
	TMP_1 *= GROWTH_ADDITIVE_FACTOR;
	if ( TMP_1 < CAPACITY && LEN <= TMP_1 ) {
		CAPACITY = TMP_1;
		pBuffer = realloc( pBuffer, TMP_1 );
	}
}

void AddPerson( void ) {
	PERSON_N++;
	TMP_0 = LEN; /* index to the value we are currently modifying */

	while ( TMP_0 % sizeof( int ) != 0 ) {
		GrowByte();
		TMP_0++;
		LEN++;
	}
	GrowInt();
	TMP_1 = TMP_0; /* base index, used for getting the person's age */

	TMP_0 += sizeof( int );
	LEN += sizeof( int );

	printf( "Digite o nome: " );
	for (;;) {
		GrowByte();
		( (char *) pBuffer )[TMP_0] = fgetc(stdin);
		LEN++;
		TMP_0++;

		if ( ( (char *) pBuffer )[TMP_0 - 1] == '\n' ) {
			( (char *) pBuffer )[TMP_0 - 1] = '\0';
			break;
		}
	}

	printf( "Digite a idade: " );
	scanf( "%d", (int *)&( (char *) pBuffer )[TMP_1] );
	assert( fgetc( stdin ) == '\n' );

	printf( "Digite o e-mail: " );
	for (;;) {
		GrowByte();
		( (char *) pBuffer )[TMP_0] = fgetc( stdin );
		LEN++;
		TMP_0++;

		if ( ( (char *) pBuffer )[TMP_0 - 1] == '\n' ) {
			( (char *) pBuffer )[TMP_0 - 1] = '\0';
			break;
		}
	}
}

void RemovePerson( void ) {
	if ( PERSON_N <= 0 ) {
		printf( "Nao e possivel remover uma pessoa de uma agenda vazia." );
		return;
	}

	TMP_3 = LEN; /* offset of the name being searched */

	printf( "Digite um nome para remover: " );

	for (;;) {
		GrowByte();
		( (char *) pBuffer )[LEN] = fgetc( stdin );
		LEN++;
		if ( ( (char *) pBuffer )[LEN - 1] == '\n' ) {
			( (char *) pBuffer )[LEN - 1] = '\0';
			break;
		}
	}

	printf( "Buscando...\n" );

	TMP_0 = 0;      /* person count */
	TMP_1 = OFFSET; /* index of the value we are comparing the name against */
	TMP_2 = 0;      /* offset, used for indexing */

	while ( TMP_0 < PERSON_N ) {
		/* alignment */
		while ( TMP_1 % sizeof( int ) != 0 ) {
			TMP_1++;
		}

		TMP_1 += sizeof( int );

		TMP_2 = 0;
		for (;;) {
			if ( ( (char *) pBuffer )[TMP_3 + TMP_2] != ( (char *) pBuffer )[TMP_1 + TMP_2] ) {
				break;
			}
			if ( ( (char *) pBuffer )[TMP_3 + TMP_2] == '\0' ) {
				break;
			}
			TMP_2 += 1;
		}

		if ( ( (char *) pBuffer )[TMP_3 + TMP_2] == ( (char *) pBuffer )[TMP_1 + TMP_2] ) {
			TMP_0 = 1; /* found someone */
			break;
		} else {
			/* not this one, skip it */
			while ( ( (char *) pBuffer )[TMP_1] != '\0' ) {
				TMP_1 += 1;
			}
			TMP_1 += 1;
			while ( ( (char *) pBuffer )[TMP_1] != '\0' ) {
				TMP_1 += 1;
			}
			TMP_1 += 1;
		}

		TMP_0 += 1;
		if ( TMP_0 >= PERSON_N ) {
			TMP_0 = 0; /* didn't find anyone */
			break;
		}
	}
	LEN = TMP_3; /* reset the length */

	if ( TMP_0 ) {
		/* remove the person */
		TMP_0 = TMP_1 - sizeof( int );
		TMP_2 = 0;

		while ( ( (char *) pBuffer )[TMP_1 + TMP_2] != '\0' ) { /* skip name */
			TMP_2 += 1;
		}
		TMP_2 += 1;
		while ( ( (char *) pBuffer )[TMP_1 + TMP_2] != '\0' ) { /* skip e-mail */
			TMP_2 += 1;
		}
		TMP_2 += 1;

		/* alignment */
		while ( (TMP_1 + TMP_2) % sizeof( int ) != 0 ) {
			TMP_2 += 1;
		}

		TMP_3 = LEN - TMP_2 - sizeof(int); /* new LEN */
		TMP_1 += TMP_2;

		/* 
		 * TMP_0 index value being replaced
		 * TMP_1 index of value replacing the value at TMP_0
		 */
		while ( TMP_1 != LEN ) {
			( (char *) pBuffer )[TMP_0] = ( (char *) pBuffer )[TMP_1];
			TMP_0 += 1;
			TMP_1 += 1;
		}
		LEN = TMP_3; /* reset length */
		PERSON_N--;

		printf( "Uma pessoa foi achada e removida da agenda.\n" );
	} else {
		printf( "Nenhuma pessoa foi encontrada\n" );
	}
	Ungrow();
}

void SearchPerson( void ) {
	if ( PERSON_N <= 0 ) {
		printf( "Nao e possivel buscar uma pessoa em uma agenda vazia." );
		return;
	}

	TMP_3 = LEN; /* base */

	printf( "Digite um nome para buscar: " );

	for (;;) {
		GrowByte();
		( (char *) pBuffer )[LEN] = fgetc( stdin );
		LEN++;
		if ( ( (char *) pBuffer )[LEN - 1] == '\n' ) {
			( (char *) pBuffer )[LEN - 1] = '\0';
			break;
		}
	}

	LEN = TMP_3;
	Ungrow();

	printf( "Buscando...\n" );

	TMP_1 = OFFSET; /* index of the value we are comparing the name against */
	TMP_2 = 0;      /* offset, used for indexing */

	for ( TMP_0 = 0; TMP_0 < PERSON_N; TMP_0++ ) { /* TMP_0: person count */
		/* alignment */
		while ( TMP_1 % sizeof( int ) != 0 ) {
			TMP_1++;
		}

		TMP_1 += sizeof( int );

		TMP_2 = 0;
		for (;;) {
			if ( ( (char *) pBuffer )[TMP_3 + TMP_2] != ( (char *) pBuffer )[TMP_1 + TMP_2] ) {
				break;
			}
			if ( ( (char *) pBuffer )[TMP_3 + TMP_2] == '\0' ) {
				break;
			}
			TMP_2 += 1;
		}

		if ( ( (char *) pBuffer )[TMP_3 + TMP_2] == ( (char *) pBuffer )[TMP_1 + TMP_2] ) {
			TMP_2 = TMP_1 - sizeof( int ); /* age index */
			TMP_1 += -6 + printf(
				"Nome: %s\n",
				&( (char *) pBuffer )[TMP_1]
			);
			printf( "Idade: %d\n", *(int *)&( (char *) pBuffer )[TMP_2] );
			TMP_1 += -8 + printf(
				"e-mail: %s\n\n",
				&( (char *) pBuffer )[TMP_1]
			);
			return;
		} else {
			/* not this one, skip it */
			while ( ( (char *) pBuffer )[TMP_1] != '\0' ) {
				TMP_1 += 1;
			}
			TMP_1 += 1;
			while ( ( (char *) pBuffer )[TMP_1] != '\0' ) {
				TMP_1 += 1;
			}
			TMP_1 += 1;
			printf("skipped: %lx\n", TMP_1 - OFFSET);
		}
	}

	printf( "Nenhuma pessoa foi encontrada.\n" );
}

void ListPeople( void ) {
	if ( PERSON_N <= 0 ) {
		printf( "A agenda esta vazia." );
		return;
	}

	TMP_1 = OFFSET; /* index of the value we are printing*/
	TMP_2 = 0;      /* base index of the entry, used for printing the age. */

	for ( TMP_0 = 0; TMP_0 < PERSON_N; TMP_0++ ) { /* TMP_0: person count */
		/* alignment */
		while ( TMP_1 % sizeof( int ) != 0 ) {
			TMP_1++;
		}

		TMP_2 = TMP_1;
		TMP_1 += sizeof( int );

		TMP_1 += -6 + printf(
			"Nome: %s\n",
			&( (char *) pBuffer )[TMP_1]
		);

		printf(
			"Idade: %d\n",
			*(int *)&( (char *) pBuffer )[TMP_2]
		);

		TMP_1 += -8 + printf(
			"e-mail: %s\n",
			&( (char *) pBuffer )[TMP_1]
		);
		printf( "\n" );
	}
}

int main( void ) {
	pBuffer = malloc( 32 );

	PERSON_N = 0;
	CAPACITY = 32;
	LEN = OFFSET;

	for (;;) {
		TMP_0 = 0;
		TMP_1 = 0;
		TMP_2 = 0;
		TMP_3 = 0;

		printf( "1. Adicionar Pessoa\n" );
		printf( "2. Remover Pessoa\n" );
		printf( "3. Buscar Pessoa\n" );
		printf( "4. Listar Todos\n" );
		printf( "5. Sair\n" );

		scanf( "%d", &TMP_0 );
		assert( fgetc( stdin ) == '\n' );
		switch ( TMP_0 ) {
			case 1:
				AddPerson();
				break;
			case 2:
				RemovePerson();
				break;
			case 3:
				SearchPerson();
				break;
			case 4:
				ListPeople();
				break;
			case 5:
				return 0;
			default:
				printf( "Opcao invalida\n" );
				break;
		}
	}

	free( pBuffer );

	return 0;
}
