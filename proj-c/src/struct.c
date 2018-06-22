#include "struct.h"

/*Função que aloca memória para uma question e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
QUESTION newQuest(int id, char* title, int votos, int respostas, Date data, char* tags,int id_user){
	QUESTION n = (QUESTION) malloc (sizeof (struct question));

	if (n != NULL) {
		n -> postagem = data;
		n -> date = get_day(data) + get_month(data) * 100 + get_year(data) * 10000;
		n -> id = GINT_TO_POINTER(id);
		strcpy(n -> titulo,title);
		n -> votos = votos;
        n -> id_user = GINT_TO_POINTER(id_user);
		n -> respostas_count = respostas;
		strcpy(n -> tags, tags);
	}
	return n;
}

/*Função que aloca memória para um user e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
USER newUser(int id, int reputation,char* name,int posts, char* bio){
	USER n = (USER) malloc (sizeof (struct user));
	
	if (n != NULL) {
		n -> id = GINT_TO_POINTER(id);
		strcpy(n -> username, name);
		n -> post_count = posts;
		strcpy(n -> short_bio, bio);
		n -> reputation = reputation;
	}
	return n;
}

/*Função que aloca memória para uma answer e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
ANSWER newAnswer(Date data,int id_resp, int id_question, int id_user, int comments, int score){
	ANSWER n = (ANSWER) malloc (sizeof (struct answer));
	
	if (n != NULL) {
		n -> postagem = data;
		n -> date = get_day(data) + get_month(data) * 100 + get_year(data) * 10000;
		n -> id_resp = GINT_TO_POINTER(id_resp);
		n -> id_question = GINT_TO_POINTER(id_question);
		n -> id_user = GINT_TO_POINTER(id_user);
		n -> comentarios = comments;
		n -> score = score;	
		n -> votesUp = 0;
		n -> votesDown = 0;	
		n -> favoritos = 0;
	}
	return n;
}

/*Função que aloca memória para uma tag e preenche os campos da respetiva struct com os valores passados 
  como argumentos
*/
TAGS newTag (int id, char* name){
	TAGS n = (TAGS) malloc (sizeof (struct tags));
	
	if (n != NULL) {
		n -> id = GINT_TO_POINTER(id);
		strcpy(n -> tag, name);
	}

	return n;
}

//Função que liberta o espaco ocupado por um dado user
void destroyUser(gpointer x){
	struct user *n = (struct user *) x;	
	
	if (n) free(n);
}

//Função que liberta o espaco ocupado por uma dada question
void destroyQuestion(gpointer x){
	struct question *n = (struct question *) x;	
	
	if (n) free(n);
}

//Função que liberta o espaco ocupado por uma dada answer
void destroyAnswer(gpointer x){
	struct answer *n = (struct answer *) x;	
	
	if (n) free(n);
}

//Função que liberta o espaco ocupado por uma dada tag
void destroyTags(gpointer x){
	struct tags *n = (struct tags *) x;	
	
	if (n) free(n);
}
