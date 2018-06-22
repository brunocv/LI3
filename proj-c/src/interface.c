#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"
#include "pair.h"

//inicializa a estrutura de dados
TAD_community init() {

    TAD_community tq = malloc(sizeof(struct TCD_community));
    tq -> users = NULL;
    tq -> questoes = NULL;
    tq -> respostas =NULL;
    tq -> tags =NULL;
    return tq;
}

//parse dos ficheiros e consequente preenchimeno das variáveis da estrutura de dados
TAD_community load(TAD_community tq, char* dump_path) {

    char *docname2 = malloc(strlen(dump_path) + strlen("Posts.xml") + 1);
    strcpy(docname2, dump_path);
    strcat(docname2, "Posts.xml");

    char *docname = malloc(strlen(dump_path) + strlen("Users.xml") + 1);
    strcpy(docname, dump_path);
    strcat(docname, "Users.xml");

    char *docname3 = malloc(strlen(dump_path) + strlen("Votes.xml") + 1);
    strcpy(docname3, dump_path);
    strcat(docname3, "Votes.xml");

    char *docname4 = malloc(strlen(dump_path) + strlen("Tags.xml") + 1);
    strcpy(docname4, dump_path);
    strcat(docname4, "Tags.xml");

    xmlDocPtr doc = xmlParseFile(docname); 
    xmlDocPtr doc2 = xmlParseFile(docname2); 
    xmlDocPtr doc3 = xmlParseFile(docname3);
    xmlDocPtr doc4 = xmlParseFile(docname4);
    
    parseUser(doc, tq);
    parseQuestion(doc2, tq);
    parseVotes(doc3, tq);
    parseTags(doc4, tq);
    return tq;
}

//apagar a estrututa de dados
TAD_community clean(TAD_community com) {
	g_hash_table_destroy (com -> users);
	g_hash_table_destroy (com -> questoes);
	g_hash_table_destroy (com -> respostas);
        g_hash_table_destroy (com -> tags);
	return com;
}

// QUERY 1

/*Função que, dado um Id de um post, retorna a informação do título desse post
  (caso seja uma answer retirna o Id da pergunta respetiva) e do user que o publicou
*/
STR_pair info_from_post(TAD_community com, long id){
    char fst[256] = "";
    char snd[256] = "";
    gpointer item_ptr = g_hash_table_lookup (com -> questoes, GINT_TO_POINTER(id));
    
    if (item_ptr != NULL) {
        struct question *q = (struct question *) item_ptr;
        if(q){
            strcpy(fst, q->titulo);

            gpointer user_ptr = g_hash_table_lookup (com -> users, q->id_user);
            struct user *u = (struct user *) user_ptr; 
            if(u) strcpy(snd, u->username);
        }
    }

    else {
        gpointer item_ptr = g_hash_table_lookup (com -> respostas, GINT_TO_POINTER(id));
        struct answer *a = (struct answer *) item_ptr; 
        if (a) {
            gpointer quest_ptr = g_hash_table_lookup (com -> questoes, a->id_question);
            struct question *q = (struct question *) quest_ptr; 
            if(q){
                strcpy(fst, q->titulo);
                gpointer user_ptr = g_hash_table_lookup (com -> users, q->id_user);
                struct user *u = (struct user *) user_ptr; 
                if(u) strcpy(snd, u->username);
            }
        }
    }

    STR_pair r = create_str_pair(fst, snd);
    return r;
}


// QUERY 2

/*Função que compara o n de posts de dois users para posteriormente estes
  serem ordenados pelo n de posts de forma descrescente
*/
int compare_func(gpointer user1_ptr, gpointer user2_ptr) {
    int fst = 0, snd = 0;
    
    struct user *u1 = (struct user *) user1_ptr; 
    if(u1) fst = u1 -> post_count;
    
    struct user *u2 = (struct user *) user2_ptr; 
    if(u2) snd = u2 -> post_count;
	
    return snd - fst;
}

//Função que retorna uma LONG_list com os N users mais ativos
LONG_list top_most_active(TAD_community com, int N) {
    LONG_list l = create_list(N);
    GList* n = g_hash_table_get_values (com -> users);
    gpointer x;
    n = g_list_sort (n, (GCompareFunc) compare_func);
    
    for (int i = 0; i < N; i++) {
        gpointer j = g_list_nth_data (n,i);
        struct user *u = (struct user *) j;
        
	if(u) x = u -> id;
		
        set_list(l, i, (long) GPOINTER_TO_INT(x));
    }
	
    g_list_free (n);
    return l;
}

// QUERY 3

/*Estrutura que contem um intervalo de tempo(inicio <-> fim) e um contador
  de answers e questions que estão nesse intervalo de tempo
*/
typedef struct q3{
    int inicio;
    int fim;
    int resp;
    int quest;
} *Q3;

/*Função que, dada question e uma estrutura que contem informação sobre um dado
  intervalo de tempo e sobre o n de respostas e de questões, incrementa o contador de
  questions que estão num dado intervalo de tempo
*/
gint Traverse_quest (gpointer key, gpointer value, gpointer data) {
    
    struct q3 *a = (struct q3 *) data;
    if(a){
        struct question *q = (struct question *) value;
        if(q){
            if (q -> date <= a -> fim && q -> date >= a -> inicio)
                a -> quest++;
        }
    }
    
    return FALSE;
}

/*Função que, dada answer e uma estrutura que contem informação sobre um dado
  intervalo de tempo e sobre o n de respostas e de questões, incrementa o contador de
  answers que estão num dado intervalo de tempo
*/
gint Traverse_resp (gpointer key, gpointer value, gpointer data) {
    
    struct q3 *a = (struct q3 *) data;
    if(a){
        struct answer *q = (struct answer *) value;
        if(q){
            if (q -> date <= a -> fim && q -> date >= a -> inicio)
                a -> resp++;
        }
    }
    
    return FALSE;
}

/*Função que, dado um intervalo de tempo arbitrário, retorna um LONG_pair
  com o n total de questões e respostas neste perı́odo;
*/
LONG_pair total_posts (TAD_community com, Date begin, Date end) {
    int in = get_day(begin) + get_month(begin) * 100 + get_year(begin) * 10000;
    int fim = get_day(end) + get_month(end) * 100 + get_year(end) * 10000;
    Q3 aux = malloc (sizeof (struct q3));
    aux -> inicio = in;
    aux -> fim = fim;
    aux -> resp = 0;
    aux -> quest = 0;

    g_hash_table_foreach(com -> questoes, (GHFunc) Traverse_quest,aux);
    g_hash_table_foreach(com -> respostas, (GHFunc) Traverse_resp,aux);
    
    LONG_pair r = create_long_pair((long) aux -> quest, (long) aux -> resp);
    return r;
}

//query 4

//Estrutura usada para ir acumulando questões que tenham uma dada tag
typedef struct q4{
    int inicio;
    int fim;
    GList* questoes;
    char *tag;
    int dim;
}*Q4;

/*Função que compara a data de duas questions para posteriormente estas
  serem ordenadas por data de forma descrescente
*/
int compare_date_4(gpointer ans1_ptr, gpointer ans2_ptr) {
    int fst = 0, snd = 0;
	
    struct question *u1 = (struct question *) ans1_ptr;
    if(u1) fst = u1 -> date;

    struct question *u2 = (struct question *) ans2_ptr;
    if(u2) snd = u2 -> date;
	
    return snd - fst;
}

/*Função que, dada questão e uma estrutura que contem informação sobre uma tag,
  insere ess questão numa glist caso tenha uma dada tag
*/
gint func_q4 (gpointer key, gpointer value, gpointer data) {
    
    struct q4 *a = (struct q4 *) data;
 
    if(a){
        struct question *q = (struct question *) value;
        if(q){
            if (q -> date <= a -> fim && q -> date >= a -> inicio && g_strrstr (q -> tags, a -> tag) != NULL){
                a -> questoes = g_list_insert_sorted(a -> questoes, q, (GCompareFunc) compare_date_4);
                a -> dim++;
            }
        }
    }
   
    return FALSE;
}
 
/*Função que, dado um intervalo de tempo e uma tag, retorna uma LONG_list com os Id's das
  perguntas contendo essa tag(perguntas ordenadas por cronologia inversa)
*/
LONG_list questions_with_tag(TAD_community com, char* tag, Date begin, Date end){
    int in = get_day(begin) + get_month(begin) * 100 + get_year(begin) * 10000;
    int fim = get_day(end) + get_month(end) * 100 + get_year(end) * 10000;
    Q4 aux = malloc(sizeof (struct q4));
    aux -> inicio = in;
    aux -> fim = fim;
    aux -> questoes = g_list_alloc();
    aux -> dim = 0;
    aux -> tag = malloc(strlen(tag) + strlen("<") + strlen(">") + 1);
    strcpy(aux -> tag,"<");
    strcat(aux -> tag, tag);
    strcat(aux -> tag, ">");
       
    g_hash_table_foreach(com -> questoes,(GHFunc) func_q4 ,aux);
    
    if (!aux -> dim) {
        LONG_list l = create_list(1);
        set_list(l, 0, 0);
        return l;
    }
    LONG_list l = create_list(aux -> dim);
 
    for (int i = 0; i < aux -> dim; i++){
        gpointer j = g_list_nth_data (aux -> questoes,i);
        
	struct question *u = (struct question *) j;
        if(u) set_list(l, i, (long) GPOINTER_TO_INT(u -> id));
        else set_list(l, i, 0);    
    }
	
    g_list_free(aux -> questoes);
 
    return l;
}

// QUERY 6

//Estrutura usada para ir acumulando respostas que estejam num determinado intervalo de tempo
typedef struct q6{
    int inicio;
    int fim;
    GList* respostas;  
    int dim;
}*Q6;

/*Função que compara o n de votos de duas answers para posteriormente estas
  serem ordenadas pelo n de votos de forma crescente
*/
int compare_votos(gpointer ans1_ptr, gpointer ans2_ptr) {
    int fst = 0, snd = 0;
    
    struct answer *u1 = (struct answer *) ans1_ptr; 
    if(u1) fst = u1 -> votesUp - u1 -> votesDown;
	
    struct answer *u2 = (struct answer *) ans2_ptr; 
    if(u2) snd = u2 -> votesUp - u2 -> votesDown;
	
    return snd - fst;
}

/*Função que, dada resposta e uma estrutura de dados, 
  insere essa resposta numa lista ligada caso esteja dentro de um certo intervalo de tempo
*/
gint func (gpointer key, gpointer value, gpointer data) {
    
    struct q6 *a = (struct q6 *) data;
    if(a){
        struct answer *q = (struct answer *) value;
        if(q){
            if (q -> date <= a -> fim && q -> date >= a -> inicio) {
                a -> respostas = g_list_insert_sorted(a -> respostas, q, (GCompareFunc) compare_votos);
                a -> dim++;
            }
        }
    }
    
    return FALSE;
}

/*Função que, dado um intervalo de tempo, retorna uma LONG_list com os Id's das
  N respostas com mais votos(respostas ordenadas por cronologia inversa)
*/
LONG_list most_voted_answers(TAD_community com, int N, Date begin, Date end){
    LONG_list l = create_list(N);

    int in = get_day(begin) + get_month(begin) * 100 + get_year(begin) * 10000;
    int fim = get_day(end) + get_month(end) * 100 + get_year(end) * 10000;
    Q6 aux = malloc(sizeof (struct q6));
    aux -> inicio = in;
    aux -> fim = fim;
    aux -> respostas = g_list_alloc();
    aux -> dim = 0;

    g_hash_table_foreach(com -> respostas,(GHFunc) func ,aux);

    for (int i = 0; i < N; i++) {
        gpointer j = g_list_nth_data (aux -> respostas,i);
        struct answer *u = (struct answer *) j;
        if(u){
          gpointer x = u -> id_resp;
          set_list(l, i, (long) GPOINTER_TO_INT(x));
        }    
    }
    g_list_free(aux -> respostas);

    return l;
}

// QUERY 7

/*Estrutura usada para guardar um dado intervalo de tempo e guardar todas as perguntas e respostas
  para se poder responder ao que é pedido para a QUERY 7
*/
typedef struct q7{
    int inicio;
    int fim;
    GHashTable* results;
    GHashTable* respostas;
    GHashTable* questions; 
    GList* list;
}*Q7;

/*Estrutura usada para ver quantas respostas de uma dada pergunta estão num dado
  intervalo de tempo
*/
typedef struct aux7{
    int inicio;
    int fim;
    int contador;
    gpointer id;
}*Aux7;

/*Função que compara o n de respostas relativos a duas questions para posteriormente estas
  serem ordenadas pelo n de respostas de forma decrescente
*/
int compare_resp_count(gpointer q1_ptr, gpointer q2_ptr) {
    int fst = 0, snd = 0;
    
    struct aux7 *u1 = (struct aux7 *) q1_ptr; 
    if(u1) fst = u1 -> contador;

    struct aux7 *u2 = (struct aux7 *) q2_ptr; 
    if(u2) snd = u2 -> contador;
	
    return snd - fst;
}

//Função que ordena uma lista ordenada pelo contador
gint func_q7 (gpointer key, gpointer value, gpointer data) {
    
    struct q7 *a = (struct q7 *) data;
 
    if(a){
        struct aux7 *q = (struct aux7 *) value;
        if(q){
            a -> list = g_list_insert_sorted(a -> list, q, (GCompareFunc) compare_resp_count);
        }
    }
   
    return FALSE;
}

/*Função que percorre a Hash das questões e cria uma struct auxiliar(aux7), relativa à questão
  em análise, com o contador a 0,caso pertença ao intervalo de tempo
*/
gint transverse_q7_questions (gpointer key, gpointer value, gpointer data) {
    
    struct q7 *a = (struct q7 *) data;
    if(a){
        struct question *q = (struct question *) value;
        if(q && (q -> date <= a -> fim && q -> date >= a -> inicio)){
            gpointer item_ptr = g_hash_table_lookup (a -> questions, q -> id);
            if (item_ptr) {
                Aux7 aux = malloc(sizeof (struct aux7));
                aux -> inicio = a -> inicio;
                aux -> fim = a -> fim;
                aux -> contador = 0;
                aux -> id = q -> id;
                g_hash_table_insert (a -> results, aux -> id,aux);
            }
        }
    }
    return FALSE;
}

/*Função que percorre a Hash das respostas e aumenta o contador na estrutura
  presente da Hash dos resultados, da respetiva questao a que responde caso 
  pertença ao intervalo de tempo
*/
gint transverse_q7_answers (gpointer key, gpointer value, gpointer data) {
    
    struct q7 *a = (struct q7 *) data;
    if(a){
        struct answer *q = (struct answer *) value;
        if(q && (q -> date <= a -> fim && q -> date >= a -> inicio)){
            gpointer item_ptr = g_hash_table_lookup (a -> results, q -> id_question);
            if (item_ptr) {
                struct aux7 *z = (struct aux7 *) item_ptr;
                if(z) {
                    z -> contador++;
                }
            }
        }
    }
    return FALSE;
}

/*Função que, dado um intervalo de tempo, devolve uma LONG_list com os Id's das N perguntas 
  com mais respostas, em ordem decrescente do número de respostas
*/
LONG_list most_answered_questions(TAD_community com, int N, Date begin, Date end){
    LONG_list l = create_list(N);

    int in = get_day(begin) + get_month(begin) * 100 + get_year(begin) * 10000;
    int fim = get_day(end) + get_month(end) * 100 + get_year(end) * 10000;
    Q7 aux = malloc(sizeof (struct q7));
    aux -> inicio = in;
    aux -> fim = fim;
    aux -> results = g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,NULL);;
    aux -> respostas = com -> respostas;
    aux -> questions = com -> questoes;
    aux -> list = g_list_alloc();

    g_hash_table_foreach(aux -> questions,(GHFunc) transverse_q7_questions ,aux);
    g_hash_table_foreach(aux -> respostas,(GHFunc) transverse_q7_answers ,aux);
    g_hash_table_foreach(aux -> results,(GHFunc) func_q7 ,aux);

    for (int i = 0; i < N; i++) {
        gpointer j = g_list_nth_data (aux -> list,i);
        struct aux7 *u = (struct aux7 *) j;
        if(u){
          set_list(l, i, (long) GPOINTER_TO_INT(u -> id));
        } 
        else set_list(l, i, 0);   
    }

    g_list_free(aux -> list);
    g_hash_table_destroy(aux -> results);

    return l;
}


// QUERY 5

/*Estrutura de dados que guarda um Id de um dado user e que contém duas listas ligadas
  que servirão para guardar as questões e respostas relativas a esse user
*/
typedef struct q5{
    long id;
    GList *questoes;
    int dim1;
    GList *respostas;
    int dim2;
}*Q5;

/*Função que compara a data de duas questions para posteriormente estas
  serem ordenadas pelo data de forma decrescente
*/
int compare_quest_date(gpointer q1_ptr, gpointer q2_ptr) {
    int fst = 0, snd = 0;
    
    struct question *u1 = (struct question *) q1_ptr; 
    if(u1) fst = u1 -> date;
	    
    struct question *u2 = (struct question *) q2_ptr; 
    if(u2) snd = u2 -> date;
	
    return snd - fst;
}

/*Função que compara a data de duas answers para posteriormente estas
  serem ordenadas pelo data de forma decrescente
*/
int compare_resp_date(gpointer q1_ptr, gpointer q2_ptr) {
    int fst = 0, snd = 0;
    
    struct answer *u1 = (struct answer *) q1_ptr; 
    if(u1) fst = u1 -> date;
	
    struct answer *u2 = (struct answer *) q2_ptr; 
    if(u2) snd = u2 -> date;
	
    return snd - fst;
}

/*Função que, dada question e uma estrutura de dados, 
  insere essa question numa lista ligada caso tenha sido postada por um 
  user com um dado Id
*/
gint func_q5_question (gpointer key, gpointer value, gpointer data) {
    
    struct q5 *a = (struct q5 *) data;
    if(a){
        struct question *q = (struct question *) value;
        if(q && (GPOINTER_TO_INT(q -> id_user) == a -> id)){
            a -> questoes = g_list_insert_sorted(a -> questoes, q, (GCompareFunc) compare_quest_date);
            a -> dim1++;
            }
        }
    
    return FALSE;
}

/*Função que, dada answer e uma estrutura de dados, 
  insere essa answer numa lista ligada caso tenha sido postada por um 
  user com um dado Id
*/
gint func_q5_answer (gpointer key, gpointer value, gpointer data) {
    
    struct q5 *a = (struct q5 *) data;
    if(a){
        struct answer *q = (struct answer *) value;
        if(q && (GPOINTER_TO_INT(q -> id_user) == a -> id)){
            a -> respostas = g_list_insert_sorted(a -> respostas, q, (GCompareFunc) compare_resp_date);
            a -> dim2++;
            }
        }
    
    return FALSE;
}

/*Função que, dado um ID de um user, devolver a informação do
  seu perfil (short bio) e os Id's dos seus 10 últimos posts (perguntas ou res-
  postas), ordenados por cronologia inversa;
*/
USER get_user_info(TAD_community com, long id){

    long posts[10] = {0};
    
    Q5 aux = malloc(sizeof (struct q5));
    aux -> id = id;
    aux -> questoes = g_list_alloc();
    aux -> dim1 = 0;
    aux -> respostas = g_list_alloc();
    aux -> dim2 = 0;

    char bio[16384] = "";
    gpointer item_ptr = g_hash_table_lookup (com -> users, GINT_TO_POINTER(id));
    
    if (item_ptr != NULL) {
        struct user *u = (struct user *) item_ptr;
        
        if(u) strcpy(bio, u -> short_bio);
    }

    g_hash_table_foreach(com -> questoes,(GHFunc) func_q5_question ,aux);
    g_hash_table_foreach(com -> respostas,(GHFunc) func_q5_answer ,aux);

    for (int i = 0, j = 0, z = 0; i < 10; i++) {
        gpointer t = g_list_nth_data (aux -> questoes,j);
        struct question *u = (struct question *) t;
            
        gpointer t2 = g_list_nth_data (aux -> respostas,z);
        struct answer *a = (struct answer *) t2;
            
        
        if (a == NULL && u) {
                    posts[i] = (long) GPOINTER_TO_INT(u -> id);
                    j++;
                    continue;
            } 
        else if (u == NULL && a) {
                posts[i] = (long) GPOINTER_TO_INT(a -> id_resp);
                z++;
                continue;
            }    

        if(u && a){
            
            if (a -> date >= u -> date) {
                posts[i] = (long) GPOINTER_TO_INT(a -> id_resp);
                z++;
                continue;
                }
            else {
                posts[i] = (long) GPOINTER_TO_INT(u -> id);
                j++;
                }
            } 
        } 
     
    
    g_list_free(aux -> questoes);
    g_list_free(aux -> respostas);

    USER new = create_user(bio, posts);

    return new; 
}

// QUERY 8

/*Estrutura usada para ir acumulando questões que estejam num dado 
  intervalo de tempo e tenham uma dada palavra no seu título
*/
typedef struct q8{
    int inicio;
    int fim;
    GList* questoes;
    char *word;
    int dim;
}*Q8;

/*Função que, dada uma pergunta e uma estrutura de dados, 
  insere essa pergunta numa lista ligada caso o seu título contenha uma determinada palavra
*/
gint func_q8 (gpointer key, gpointer value, gpointer data) {
    
    struct q8 *a = (struct q8 *) data;
 
    if(a){
        struct question *q = (struct question *) value;
        if(q){
            char aux[259];
            strcpy(aux," ");
            int i; 
            for (i = 0; i < 256; i++) {
                if ((q -> titulo[i] > 64 && q -> titulo[i] < 91) || (q -> titulo[i] > 96 && q -> titulo[i] < 123))
                    aux[i + 1] = q -> titulo[i];
                else aux[i + 1] = ' '; 
            }
            aux[i] = '\0';
            strcat(aux, " ");
            if (g_strrstr (aux, a -> word) != NULL) {
                a -> questoes = g_list_insert_sorted(a -> questoes, q, (GCompareFunc) compare_date_4);
                a -> dim++;
            }
        }
    }
   
    return FALSE;
}

/*Função que, ado uma palavra, retorna uma LONG_list com os ID's das
  N respostas cujos tı́tulos a contenham(respostas ordenados por cronologia inversa)
*/
LONG_list contains_word(TAD_community com, char* word, int N){
    Q8 aux = malloc(sizeof (struct q8));
    aux -> questoes = g_list_alloc();
    aux -> dim = 0;
    aux -> word = malloc(strlen(word) + strlen(" ") + strlen(" ") + 1);
    strcpy(aux -> word," ");
    strcat(aux -> word, word);
    strcat(aux -> word, " ");
       
    g_hash_table_foreach(com -> questoes,(GHFunc) func_q8 ,aux);
    
    if (!aux -> dim) {
        LONG_list l = create_list(1);
        set_list(l, 0, 0);
        return l;
    }

    LONG_list l = create_list(N);
 
    for (int i = 0; i < N; i++){
        gpointer j = g_list_nth_data (aux -> questoes,i);
        
	struct question *u = (struct question *) j;
        if(u) set_list(l, i, (long) GPOINTER_TO_INT(u -> id));
        else set_list(l, i, 0);    
    }
	
    g_list_free(aux -> questoes);
 
    return l;
}

//QUERY 10

//Estrutura usada oara guardar valores para o calculo da média de uma resposta
typedef struct q10{
    long id;
    long id_maiorResp;
    float score;
    float reputation;
    float comentarios;
    float votesUp;
    float votesDown;
    float guarda;
    float maior;
    GList* utiliz;
}*Q10;

//Estrutura usada para guardar a reputação de um user com um dado Id
typedef struct aux10{
    long id_user;
    float reputation;
}*AUX10;

//Função que usada para guardar a media de uma dada answer
void guardRep(gpointer val0, gpointer val1){
    struct user *a0 = (struct user *) val0;
    struct aux10 *a1 = (struct aux10 *) val1;

    if(GPOINTER_TO_INT(a0 -> id) == (int)a1->id_user) a1->reputation = a0 -> reputation;
}

/*Função que, a uma dada resposta da estrutura de dados, acumula os valores (de avaliacao) 
  dessa dada resposta que vao ser usados no calculo da sua media
*/
gint fun (gpointer key, gpointer value, gpointer data) {
    struct q10 *a = (struct q10 *) data;    

    if(a){
        struct answer *q = (struct answer *) value;

        if(q){
            if ((long)q -> id_question == a -> id){
                AUX10 aux = malloc(sizeof (struct aux10));
                aux -> id_user = GPOINTER_TO_INT(q->id_user);
                aux -> reputation = 0;
                
                a -> score = (float)q -> score;
                a -> comentarios = (float)q -> comentarios;
                a -> votesUp = (float)q -> votesUp;
                a -> votesDown = (float)q -> votesDown;

                g_list_foreach(a->utiliz,(GFunc) guardRep,aux);
                
                a -> reputation = aux -> reputation;

                free(aux);
                
                a -> guarda = ((a->score) * 0.45) + ((a->reputation) * 0.25) + ((a->votesUp - a->votesDown) * 0.2) + ((a->comentarios) * 0.1);
                if((a->guarda) >= (a->maior)){
                    a->maior = a->guarda;
                    a -> id_maiorResp = (long)q -> id_resp;
                }

            }
        }
    }
    
    return FALSE;
}

//Função que, dado o Id de uma pergunta, retorna a média ponderada da melhor resposta
long better_answer(TAD_community com, long id){

    Q10 a = malloc(sizeof (struct q10));
    a -> id = id;
    a -> id_maiorResp = -1;
    a -> score = 0;
    a -> reputation = 0;
    a -> comentarios = 0;
    a -> votesUp = 0;
    a -> votesDown = 0;
    a -> guarda = -99;
    a -> maior = -99;
    a -> utiliz = g_hash_table_get_values (com -> users);    

    g_hash_table_foreach(com -> respostas,(GHFunc) fun, a);

    g_list_free(a->utiliz);

    return (long)(a->id_maiorResp);
}

//QUERY 9

/*Estrutura que armazena Id's de dois users, uma Hash com todas as perguntas existentes,
  duas Hashs com perguntas em que dois users(separadamente) participaram e uma glist que 
  contém perguntas em que ambos os users participaram
*/
typedef struct q9{
    int id1;
    int id2;
    GHashTable* questoes;
    GHashTable* questoes1;
    int used1;
    GHashTable* questoes2;
    int used2;
    GList* comuns;
}*Q9;

/*Função que acrescenta informação a uma dada Hash de questions quando encontra 
  uma dada question feita por um user
*/
gint func_q9_question (gpointer key, gpointer value, gpointer data) {
    
    struct q9 *a = (struct q9 *) data;
    if(a){
        struct question *q = (struct question *) value;
        if(q && (GPOINTER_TO_INT(q -> id_user) == a -> id1)){
            g_hash_table_insert (a -> questoes1,q -> id,q);
            a -> used1++;
            }
        }
    
    return FALSE;
}

/*Função que acrescenta informação a uma dada Hash de questions quando encontra 
  uma dada answer feita por um user(é feita a inserção da question relativa a essa
  answer)
*/
gint func_q9_answer (gpointer key, gpointer value, gpointer data) {
    
    struct q9 *a = (struct q9 *) data;
    if(a){
        struct answer *q = (struct answer *) value;
        if(q && (GPOINTER_TO_INT(q -> id_user) == a -> id1)){
            
			gpointer quest_ptr = g_hash_table_lookup (a -> questoes, q -> id_question);
            struct question *u = (struct question *) quest_ptr; 
            if(u) { 
            	g_hash_table_insert (a -> questoes1,u -> id,u);
            	a -> used1++;
            }
        }
    }
    
    return FALSE;
}

/*Função que acrescenta informação a uma dada Hash de questions quando encontra 
  uma dada question feita por um user
*/
gint func_q9_question2 (gpointer key, gpointer value, gpointer data) {
    
    struct q9 *a = (struct q9 *) data;
    if(a){
        struct question *q = (struct question *) value;
        if(q && (GPOINTER_TO_INT(q -> id_user) == a -> id2)){
            g_hash_table_insert (a -> questoes2,q -> id,q);
            a -> used2++;
            }
        }
    
    return FALSE;
}

/*Função que acrescenta informação a uma dada Hash de questions quando encontra 
  uma dada answer feita por um user(é feita a inserção da question relativa a essa
  answer)
*/
gint func_q9_answer2 (gpointer key, gpointer value, gpointer data) {
    
    struct q9 *a = (struct q9 *) data;
    if(a){
        struct answer *q = (struct answer *) value;
        if(q && (GPOINTER_TO_INT(q -> id_user) == a -> id2)){
            
			gpointer quest_ptr = g_hash_table_lookup (a -> questoes, q -> id_question);
            struct question *u = (struct question *) quest_ptr; 
            if(u) { 
            	g_hash_table_insert (a -> questoes2,u -> id,u);
            	a -> used2++;
            }
        }
    }
    
    return FALSE;
}

//Função que filtra os posts em que ambos os users participam
void filtro(GHashTable* h1, GHashTable* h2,int dim, Q9 aux){
    
	GList* l = g_hash_table_get_values(h1);
	for (int i = 0; i < dim; i++) {
		gpointer quest = g_list_nth_data (l, i);
        struct question *a = (struct question *) quest;
        if (a) {		
            if (g_hash_table_contains(h2, a -> id) == TRUE) {
            	aux -> comuns = g_list_insert_sorted(aux -> comuns, a, (GCompareFunc) compare_quest_date);       
            }
        }
	}
	g_list_free(l);
}

/*Função que, dados os Id's de dois utilizadores, devolve uma LONG_list com as últimas
  N perguntas (cronologia inversa) em que participaram estes dois utilizadores
*/
LONG_list both_participated(TAD_community com, long id1, long id2, int N){

    Q9 aux = malloc(sizeof (struct q9));
    aux -> id1 = (int) id1;
    aux -> id2 = (int) id2;
    aux -> questoes = com -> questoes;
    aux -> questoes1 = g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,NULL);
    aux -> questoes2 = g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,NULL);
    aux -> used1 = 0;
    aux -> used2 = 0;
    aux -> comuns = g_list_alloc();
    
    g_hash_table_foreach(com -> questoes,(GHFunc) func_q9_question ,aux);
    g_hash_table_foreach(com -> respostas,(GHFunc) func_q9_answer ,aux);
    g_hash_table_foreach(com -> questoes,(GHFunc) func_q9_question2 ,aux);
    g_hash_table_foreach(com -> respostas,(GHFunc) func_q9_answer2 ,aux);
    
    if (aux -> used1 >= aux -> used2) {
    	filtro(aux -> questoes1, aux -> questoes2,aux -> used1, aux);
    }
    else {
    	filtro(aux -> questoes2, aux -> questoes1,aux -> used2, aux);
    }
    
    LONG_list l = create_list(N);

    for (int i=0; i < N; i++){
        
        gpointer t = g_list_nth_data (aux -> comuns, i);
        struct question *q = (struct question *) t;
        
        if (q) {
        	set_list(l,i,GPOINTER_TO_INT (q -> id)); 
    	}
    	else set_list(l,i,0);
    }
    g_hash_table_destroy(aux -> questoes1);
    g_hash_table_destroy(aux -> questoes2);
    g_list_free(aux -> comuns);
   
    return l;
}

// QUERY 11

/*Estrutura que guarda um dado intervalo de tempo, as tag numa glist, os users numa glist
  e que tem mais duas glists(uma para guardar informações relativas às tags e outra para
  guardar questions que foram feitas pelos N users com melhor reputação e que estajam 
  num dado intervalo de tempo)
*/
typedef struct q11{
    GList* reputacao;
    GList* questoes;
    GList* tags;
    GList* contadores;
    int inicio;
    int fim;
    int id;
}*Q11;

//Estrutura que guarda informção sobre uma dada tag
typedef struct contador{
    char *tag;
    gpointer id;
    int conta;
}*CONTADOR;

/*Função que compara o n de ocorrências de duas tags(tags essas que estão separadamente numa 
  estrutura que contém toda a informação necessária sobre estas);
  posteriormente, estas estruturas vão ser ordenadas(de forma decrescente) pelo n de ocorrências 
  da tag que guardam
*/
int compare_ntag(gpointer q1_ptr, gpointer q2_ptr){
    int fst = 0, snd = 0;
    
    struct contador *u1 = (struct contador *) q1_ptr; 
    if(u1) fst = u1 -> conta;
        
    struct contador *u2 = (struct contador *) q2_ptr; 
    if(u2) snd = u2 -> conta;
    
    return snd - fst;
}

/*Função que compara a reputaçao de dois users para posteriormente estes serem ordenados 
  pela reputação de forma decrescente
*/
int compare_user_rep(gpointer q1_ptr, gpointer q2_ptr){
    long fst = 0, snd = 0;
    
    struct user *u1 = (struct user *) q1_ptr; 
    if(u1) fst =(long) u1 -> reputation;
	    
    struct user *u2 = (struct user *) q2_ptr; 
    if(u2) snd =(long) u2 -> reputation;

    return snd - fst;
}

/*Função que, dada uma question e uma estrutura de dados, guarda uma question numa lista ligada
  caso esta tenha sido postada por um user com um dado Id e caso pertença a um dado intervalo
  de tempo
*/
gint func_quest (gpointer key, gpointer value, gpointer data){
    
    struct q11 *a = (struct q11 *) data;
    if(a){
        struct question *q = (struct question *) value;
        if(q && (GPOINTER_TO_INT(q -> id_user) == a -> id) && a -> inicio <= q -> date && a -> fim >= q -> date){
            a -> questoes = g_list_append (a -> questoes, q);
            }
        }
    
    return FALSE;
}

/*Função que, dada uma question e uma estrutura que contém informação
  sobre uma tag, verifica se a tag aparece ou não nessa question;se aparecer,
  incrementa o n de ocorrências dessa tag numa dada lista ligada de questoes(questoes
  essas feitas pelos N users com melhor reputação)
*/
void function1 (gpointer data, gpointer user_data){
    struct question *a = (struct question *) data;
    struct contador *b = (struct contador *) user_data;

    if(b){
        if(a){
            if(g_strrstr (a -> tags, b -> tag) != NULL) b -> conta++;
        }
    }
}

/*Função que, dada uma lista ligada e uma estrutura de dados, 
  percorre a lista ligada(de questions) com o objetivo de contar quantas vezes
  certa tag aparece  
*/
void function0 (gpointer data, gpointer user_data){
    struct contador *a = (struct contador *) data;
    GList *guarda = (GList *) user_data;

    g_list_foreach(guarda, (GFunc) function1, a);
}

/*Função que, dado um intervalo de tempo, devolve uma LONG_list com os identificadores 
  das N tags mais usadas pelos N utilizadores com melhor reputação, em ordem decrescente 
  do número de vezes em que a tag foi usada.
*/
LONG_list most_used_best_rep (TAD_community com, int N, Date begin, Date end){
    int i;
    Q11 aux = malloc(sizeof (struct q11));
    aux -> reputacao = g_list_alloc();
    aux -> questoes = g_list_alloc();
    aux -> tags = g_list_alloc();
    aux -> contadores = g_list_alloc();
    aux -> inicio = get_day(begin) + get_month(begin) * 100 + get_year(begin) * 10000;
    aux -> fim = get_day(end) + get_month(end) * 100 + get_year(end) * 10000;

    
    aux -> reputacao = g_hash_table_get_values (com -> users);
    aux -> tags = g_hash_table_get_values (com -> tags);
    aux -> reputacao = g_list_sort (aux -> reputacao , (GCompareFunc) compare_user_rep);
    
    LONG_list l = create_list(N);
 
    for (i=0; i<N; i++){
        
        gpointer t = g_list_nth_data (aux -> reputacao, i);
        struct user *u = (struct user *) t;
        
        if (u) {
            set_list(l,i, GPOINTER_TO_INT ( u -> id));
            aux -> id = GPOINTER_TO_INT (u -> id); 
            g_hash_table_foreach(com -> questoes,(GHFunc) func_quest ,aux);
        } 
    }

    gpointer t1 = g_list_nth_data (aux -> tags, 0);
    
    for (i=0; t1; i++){
        
        gpointer t1 = g_list_nth_data (aux -> tags, i);
        struct tags *tag = (struct tags *) t1;
        
        if (tag){
            CONTADOR conta = malloc(sizeof (struct contador));
            conta -> id = tag -> id;
            conta -> conta = 0;
            
            conta -> tag = malloc(strlen(tag -> tag) + strlen("<") + strlen(">") + 1);
            strcpy(conta -> tag,"<");
            strcat(conta -> tag, tag -> tag);
            strcat(conta -> tag, ">");
            
            aux -> contadores = g_list_append (aux -> contadores, conta);

        }
        else break;
    }

    g_list_foreach (aux -> contadores,(GFunc) function0,aux -> questoes);

    aux -> contadores = g_list_sort (aux -> contadores, (GCompareFunc) compare_ntag);

    LONG_list n = create_list(N);
 
    for (int i = 0; i < N; i++){
        gpointer j = g_list_nth_data (aux -> contadores,i);
        
        struct contador *u = (struct contador *) j;
        
        if(u){
            if(u -> conta == 0) break;
            else set_list(n, i, (long) GPOINTER_TO_INT(u -> id));
        }
            
        else set_list(n, i, 0);    
    }

    g_list_free(aux -> reputacao);
    g_list_free(aux -> questoes);
    g_list_free(aux -> tags);
    g_list_free(aux -> contadores);
    
    return n;

}
