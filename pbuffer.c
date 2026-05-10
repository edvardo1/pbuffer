#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define person_n (((int *)pBuffer)[0])
#define len      (((int *)pBuffer)[1])
#define capacity (((int *)pBuffer)[2])
#define tmp_0    (((int *)pBuffer)[3])
#define tmp_1    (((int *)pBuffer)[4])
#define tmp_2    (((int *)pBuffer)[5])
#define tmp_3    (((int *)pBuffer)[6])
#define OFFSET  (8 * sizeof(int))
#define OFFSETI (8)

void *pBuffer = NULL;

void grow_int( void ) {
	if (len + 4 >= capacity) {
		capacity += 32;
		pBuffer = realloc(pBuffer, capacity);
	}
}

void grow_byte( void ) {
	if (len + 1 >= capacity) {
		capacity += 32;
		pBuffer = realloc(pBuffer, capacity);
	}
}

void addPerson( void ) {
	person_n++;
	tmp_0 = len;

	while (tmp_0 % 4 != 0) {
		grow_byte();
		tmp_0++;
		len++;
	}
	grow_int();
	tmp_1 = tmp_0;

	tmp_0 += 4;
	len += 4;

	printf("digite o nome: ");
	for (;;) {
		grow_byte();
		((char *)pBuffer)[tmp_0] = fgetc(stdin);
		len++;
		tmp_0++;

		if (((char *)pBuffer)[tmp_0 - 1] == '\n') {
			((char *)pBuffer)[tmp_0 - 1] = '\0';
			break;
		}
	}

	printf("digite a idade: ");
	scanf("%d", (int *)&((char *)pBuffer)[tmp_1]);
	assert(fgetc(stdin) == '\n');

	printf("digite o e-mail: ");
	for (;;) {
		grow_byte();
		((char *)pBuffer)[tmp_0] = fgetc(stdin);
		len++;
		tmp_0++;

		if (((char *)pBuffer)[tmp_0 - 1] == '\n') {
			((char *)pBuffer)[tmp_0 - 1] = '\0';
			break;
		}
	}
}

void removePerson( void ) {
	tmp_3 = len; /* base */

	printf("digite um nome para buscar: ");

	for (;;) {
		grow_byte();
		((char *)pBuffer)[len] = fgetc(stdin);
		len++;
		if (((char *)pBuffer)[len - 1] == '\n') {
			((char *)pBuffer)[len - 1] = '\0';
			break;
		}
	}

	printf("procurando...\n");

	tmp_0 = 0;
	tmp_1 = OFFSET;
	tmp_2 = 0;

	for (;;) {
		/* alignment */
		while (tmp_1 % 4 != 0) {
			tmp_1++;
		}

		tmp_1 += 4;

		tmp_2 = 0;
		for (;;) {
			if (((char *)pBuffer)[tmp_3 + tmp_2] != ((char *)pBuffer)[tmp_1 + tmp_2]) {
				break;
			}
			if (((char *)pBuffer)[tmp_3 + tmp_2] == '\0') {
				break;
			}
			tmp_2 += 1;
		}

		if (((char *)pBuffer)[tmp_3 + tmp_2] == ((char *)pBuffer)[tmp_1 + tmp_2]) {
			tmp_0 = 1;
			break;
		}

		tmp_0 += 1;
		if (tmp_0 >= person_n) {
			tmp_0 = 0;
			break;
		}
	}
	len = tmp_3;

	if (tmp_0) { /* found someone */
		tmp_0 = tmp_1 - 4;
		tmp_2 = 0;

		while (((char *)pBuffer)[tmp_1 + tmp_2] != '\0') { /* skip name */
			tmp_2 += 1;
		}
		tmp_2 += 1;
		while (((char *)pBuffer)[tmp_1 + tmp_2] != '\0') { /* skip email */
			tmp_2 += 1;
		}
		tmp_3 = len - tmp_2; /* new len */
		while (tmp_2 != len) {
			((char *)pBuffer)[tmp_1] = ((char *)pBuffer)[tmp_2];
			tmp_1 += 1;
			tmp_2 += 1;
		}
		len = tmp_3;
	}
}

void searchPerson( void ) {
	tmp_3 = len; /* base */

	printf("digite um nome para buscar: ");

	for (;;) {
		grow_byte();
		((char *)pBuffer)[len] = fgetc(stdin);
		len++;
		if (((char *)pBuffer)[len - 1] == '\n') {
			((char *)pBuffer)[len - 1] = '\0';
			break;
		}
	}

	printf("procurando...\n");

	tmp_0 = 0;
	tmp_1 = OFFSET;
	tmp_2 = 0;

	while (tmp_0 < person_n) {
		/* alignment */
		while (tmp_1 % 4 != 0) {
			tmp_1++;
		}

		tmp_1 += 4;

		tmp_2 = 0;
		for (;;) {
			if (((char *)pBuffer)[tmp_3 + tmp_2] != ((char *)pBuffer)[tmp_1 + tmp_2]) {
				break;
			}
			if (((char *)pBuffer)[tmp_3 + tmp_2] == '\0') {
				break;
			}
			tmp_2 += 1;
		}

		if (((char *)pBuffer)[tmp_3 + tmp_2] == ((char *)pBuffer)[tmp_1 + tmp_2]) {
			printf("uma pessoa foi achada:\n");
			tmp_2 = tmp_1 - 4; /* age index */
			tmp_1 += -6 + printf(
				"nome: %s\n",
				&((char *)pBuffer)[tmp_1]
			);
			printf("idade: %d\n", *(int *)&((char *)pBuffer)[tmp_2]);
			tmp_1 += -7 + printf(
				"email: %s\n",
				&((char *)pBuffer)[tmp_1]
			);
			break;
		}

		tmp_0 += 1;
	}
	len = tmp_3;
}

void listPeople( void ) {
	tmp_0 = 0;
	tmp_1 = OFFSET;
	tmp_2 = 0;

	while (tmp_0 < person_n) {
		/* alignment */
		while (tmp_1 % 4 != 0) {
			tmp_1++;
		}

		tmp_2 = tmp_1;
		tmp_1 += 4;

		tmp_1 += -6 + printf(
			"nome: %s\n",
			&((char *)pBuffer)[tmp_1]
		);

		printf(
			"idade: %d\n",
			*(int *)&((char *)pBuffer)[tmp_2]
		);

		tmp_1 += -7 + printf(
			"email: %s\n",
			&((char *)pBuffer)[tmp_1]
		);
		tmp_0 += 1;
	}
}

int main() {
	pBuffer = malloc(8);

	person_n = 0;
	capacity = 8;
	len = OFFSET;
	tmp_0 = 0;
	tmp_1 = 0;
	tmp_2 = 0;

	for (;;) {
		printf("1. Adicionar Pessoa\n");
		printf("2. Remover Pessoa\n");
		printf("3. Buscar Pessoa\n");
		printf("4. Listar Todos\n");
		printf("5. Sair\n");

		scanf("%d", &tmp_0);
		assert(fgetc(stdin) == '\n');
		switch (tmp_0) {
			case 1:
				addPerson();
				break;
			case 2:
				//removePerson();
				break;
			case 3:
				searchPerson();
				break;
			case 4:
				listPeople();
				break;
			case 5:
				return 0;
			default:
				printf("opcao invalida\n");
				break;
		}

		printf("\n");
		printf("OFFSET: %ld\n", OFFSET);
		printf("len: %d\n", len);

		printf("..  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
		int j = 1;
		printf("00 ");
		for (int i = OFFSET; i < capacity; i++) {
			if (i >= len) {
				printf("\x1b[37m");
				printf("\x1b[41m");
			} else if (((i / 4) % 2 == 0) ^ (i / 16 % 2 == 0)) {
				printf("\x1b[30m");
				printf("\x1b[47m");
			} else {
				printf("\x1b[37m");
				printf("\x1b[40m");
			}

			if (((char *)pBuffer)[i] < '!') {
				printf("%02x", ((char *)pBuffer)[i]);
				printf("\x1b[0m ");
			} else {
				printf("_%c", ((char *)pBuffer)[i]);
				printf("\x1b[0m ");
			}

			if ((i - OFFSET) % 16 == 15) {
				printf("\x1b[0m\n%02x ", j++);
			}
		}
		printf("\x1b[0m");
		printf("\n\n");
	}
	return 0;
}
