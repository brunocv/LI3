#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include "date.h"
#include <glib.h>

//Estrutura que define uma resposta
typedef struct answer{
	Date postagem; /*Data em que foi postada a resposta*/
	int date;
	gpointer id_resp;
	gpointer id_question; 
	gpointer id_user;
	int comentarios;
	// votos (up e down) NO FICHEIRO VOTES.xml
	int votesUp;
	int votesDown;
	int score; // O que é isto????
	int favoritos;
}*ANSWER;

//Estrutura que define uma pergunta
typedef struct question {
    gpointer id_user;
	Date postagem; 
	int date;
	gpointer id; 
	char titulo[256]; 
	char tags[128]; //TEM QUE SE MUDAR ESTE TIPO PQ FICA TUDO JUNTO
	int votos;
	int respostas_count;
}*QUESTION;

//Estrutura que define um user
typedef struct user {
    gpointer id;
    char username[256]; 
    int post_count; 
    char short_bio[16384]; 
    int reputation;
}*USER;

//Estrutura que define uma tag
typedef struct tags {
    gpointer id;
    char tag[256]; 
}*TAGS;

//Estrutura de dados que engloba toda a informação necessária para a resposta às QUERY's
typedef struct TCD_community{
    GHashTable* respostas;    
    GHashTable* questoes; /*Arvore balanceada com as questoes */
    GHashTable* users; /*Arvore balanceada com os users */
    GHashTable* tags;
}TCD_community;

//Apontador para uma estrutura TCD_community
typedef struct TCD_community * TAD_community;

/*Função que aloca memória para uma question e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
QUESTION newQuest(int id, char* title, int votos, int respostas, Date data, char* tags,int id_user);

/*Função que aloca memória para um user e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
USER newUser(int id, int reputation,char* name,/*GList* questoes,GList* respostas,*/int posts, char* bio);

/*Função que aloca memória para uma answer e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
ANSWER newAnswer(Date data, int id_resp, int id_question, int id_user, int comments, int score);

/*Função que aloca memória para uma tag e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
TAGS newTag (int id, char* name); 

//Função que liberta o espaco ocupado por um dado user
void destroyUser(gpointer x);

//Função que liberta o espaco ocupado por uma dada question
void destroyQuestion(gpointer x);

//Função que liberta o espaco ocupado por uma dada answer
void destroyAnswer(gpointer x);

//Função que liberta o espaco ocupado por uma dada tag
void destroyTags(gpointer x);
