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
#define OFFSET  (8 * sizeof( int ))

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
	TMP_1 = (LEN + GROWTH_ADDITIVE_FACTOR - 1);
	TMP_1 /= GROWTH_ADDITIVE_FACTOR;
	TMP_1 *= GROWTH_ADDITIVE_FACTOR;
	if ( TMP_1 < CAPACITY && LEN <= TMP_1 ) {
		CAPACITY = TMP_1;
		pBuffer = realloc( pBuffer, TMP_1 );
	}
}

void AddPerson( void ) {
	PERSON_N++;
	TMP_0 = LEN;

	while ( TMP_0 % sizeof( int ) != 0 ) {
		GrowByte();
		TMP_0++;
		LEN++;
	}
	GrowInt();
	TMP_1 = TMP_0;

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
	TMP_3 = LEN; /* base */

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

	TMP_0 = 0;
	TMP_1 = OFFSET;
	TMP_2 = 0;

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
			TMP_0 = 1;
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
		}

		TMP_0 += 1;
		if ( TMP_0 >= PERSON_N ) {
			TMP_0 = 0;
			break;
		}
	}
	LEN = TMP_3;

	if ( TMP_0 ) { /* found someone */
		TMP_0 = TMP_1 - sizeof( int );
		TMP_2 = 0;

		while ( ( (char *) pBuffer )[TMP_1 + TMP_2] != '\0' ) { /* skip name */
			TMP_2 += 1;
		}
		TMP_2 += 1;
		while ( ( (char *) pBuffer )[TMP_1 + TMP_2] != '\0' ) { /* skip e-mail */
			TMP_2 += 1;
		}

		/* alignment */
		while ( (TMP_1 + TMP_2) % sizeof( int ) != 0 ) {
			TMP_2 += 1;
		}

		TMP_3 = LEN - TMP_2; /* new LEN */
		TMP_1 += TMP_2;

		while ( TMP_1 != LEN ) {
			( (char *) pBuffer )[TMP_0] = ( (char *) pBuffer )[TMP_1];
			TMP_0 += 1;
			TMP_1 += 1;
		}
		LEN = TMP_3;

		Ungrow();
		PERSON_N--;

		printf( "Uma pessoa foi achada e removida da agenda.\n" );
	} else {
		printf( "Nenhuma pessoa foi encontrada\n" );
	}
}

void SearchPerson( void ) {
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

	printf( "Buscando...\n" );

	TMP_0 = 0;
	TMP_1 = OFFSET;
	TMP_2 = 0;

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
		}

		TMP_0 += 1;
	}

	printf( "Nenhuma pessoa foi encontrada.\n" );
}

void ListPeople( void ) {
	TMP_0 = 0;
	TMP_1 = OFFSET;
	TMP_2 = 0;

	while ( TMP_0 < PERSON_N ) {
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
		TMP_0 += 1;
	}
}

int main( void ) {
	pBuffer = malloc( 32 );

	PERSON_N = 0;
	CAPACITY = 32;
	LEN = OFFSET;
	TMP_0 = 0;
	TMP_1 = 0;
	TMP_2 = 0;

	for (;;) {
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
				printf("opcao invalida\n");
				break;
		}
	}
	return 0;
}
