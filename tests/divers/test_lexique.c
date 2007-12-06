#include "../../lexique.h"
#include <string.h>
#include <stdio.h>


void test_lexique()
{
    lexique_t * l = create_lexique();

    int i;

    char buff[50];

    for(i = 0; i < 100; ++i)
    {
	sprintf(buff, "chaine n°%d", i);
	lexique_add(l, strdup(buff));
    }

    lexique_add(l, strdup("ceci est un test !"));
    lexique_add(l, strdup("bonjour"));

    for(i = 0; i < 50; ++i)
    {
	sprintf(buff, "blabla n°%d", i);
	lexique_add(l, strdup(buff));
    }

    printf("search(bonjour) == %d\n", lexique_search(l, "bonjour"));
    printf("search(salut) == %d\n", lexique_search(l, "salut"));
    printf("search(bonjour vous) == %d\n", lexique_search(l, "bonjour vous"));
    printf("search(chaine n°50) == %d\n", lexique_search(l, "chaine n°50"));
    printf("search(chaine n°150) == %d\n", lexique_search(l, "chaine n°150"));
    printf("search(blabla) == %d\n", lexique_search(l, "blala"));


    int s = lexique_count(l);
    printf("CONTENU -- %d elements\n", s);

    for(i = 0; i < s; ++i)
    {
	printf("lexique[%d] = %s\n", i, lexique_get(l, i));
    }

    free_lexique(l);
}


int main()
{
    test_lexique();
    return 0;
}
