#include "load.h"
#include "date.h"
#include <gmodule.h>

//Função que incrementa o numero de posts de um dado user, caso este exista
void incrementa_posts (GHashTable* a,gpointer id){

    gpointer item_ptr = g_hash_table_lookup (a, id);

    struct user *u = (struct user *) item_ptr;
    if(u) u -> post_count++;
}

//Função que incrementa o numero de votesUp de uma dada resposta, caso esta exista
void incrementa_votesUp (gpointer item_ptr){

    struct answer *u = (struct answer *) item_ptr; 
    if(u) u -> votesUp++;
}

//Função que incrementa o numero de votos (favoritos) de uma dada resposta, caso esta exista
void incrementa_favs (gpointer item_ptr){

    struct answer *u = (struct answer *) item_ptr;
    if(u) u -> favoritos++;
}

//Função que incrementa o numero de votesDown de uma dada resposta, caso esta exista
void incrementa_votesDown (gpointer item_ptr){

    struct answer *u = (struct answer *) item_ptr;
	if(u) u -> votesDown++;
}

//Função que faz o parse do ficheiro "Posts.xml" e "passa" os valores lidos para a estrutura de dados
int parseQuestion (xmlDocPtr doc, TAD_community tq) {
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    int id_resp = -1, id = -1, votos = -1, respostas = -1, id_q = -1, comentarios = -1, sc = -1,id_user=-1;
    char title[256] = "";
    char postagem[30] = ""; 
    char tags[128] = "";
    Date data = createDate(0,0,0);
    GHashTable* questoes= g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,destroyQuestion);//g_hash_table_new(g_int_hash,g_direct_equal);
    GHashTable* resp=g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,destroyAnswer);//g_hash_table_new(g_int_hash,g_direct_equal);
    QUESTION q = NULL;
    ANSWER a = NULL;
    xmlChar *uri;

    if (doc == NULL ) {
	fprintf(stderr,"Document not parsed successfully. \n");
	return 0;
    }

    if (cur == NULL) {
	fprintf(stderr,"empty document\n");
	return 1;
    }


    cur = cur->xmlChildrenNode;
    
    while (cur != NULL){
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"row"))) {
	    uri = xmlGetProp(cur, (xmlChar*)"PostTypeId");
		    
            if (atoi((char*) uri) == 1) {
		uri = xmlGetProp(cur, (xmlChar*)"Id");
		id = atoi((char*) uri);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"Title");
		if (uri == NULL) strcpy(title, "NULL");
		else strcpy (title,(char*)uri);
		xmlFree(uri); 
		
		uri = xmlGetProp(cur, (xmlChar*)"AnswerCount");
		respostas = atoi((char*)uri);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"Score");
		votos = atoi((char*)uri);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"CreationDate");
		strcpy (postagem,(char*)uri);
		data = fromString(postagem);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"Tags");
		strcpy (tags,(char*)uri);
		xmlFree(uri);
                
		uri = xmlGetProp(cur, (xmlChar*)"OwnerUserId");
		if (uri == NULL) id_user = -2;
		else id_user = atoi((char*)uri);
		xmlFree(uri);
                        
                //incrementaPost(tq -> users, id_user);
		q = newQuest(id, title, votos, respostas, data, tags,id_user);
        	g_hash_table_insert (questoes, GINT_TO_POINTER(q->id),q); // mais eficiente
	    	incrementa_posts(tq->users, q->id_user);
	    }

	    else if (atoi((char*) uri) == 2){
		uri = xmlGetProp(cur, (xmlChar*)"CreationDate");
		strcpy (postagem,(char*)uri);
		data = fromString(postagem);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"Id");
		id_resp = atoi((char*)uri);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"OwnerUserId");
		if (uri == NULL) id = -2;
		else id = atoi((char*)uri);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"ParentId");
		id_q = atoi((char*)uri);
		xmlFree(uri);		    	
		
		uri = xmlGetProp(cur, (xmlChar*)"Score");
		sc = atoi((char*)uri);
		xmlFree(uri);
		
		uri = xmlGetProp(cur, (xmlChar*)"CommentCount");
		comentarios = atoi((char*)uri);
		xmlFree(uri);
                        
                //incrementaPost(tq -> users, id);
		a = newAnswer(data,id_resp,id_q, id, comentarios, sc);
        	g_hash_table_insert (resp, GINT_TO_POINTER(a->id_resp),a); 
		incrementa_posts(tq->users, a->id_user);
		}
	}
	cur = cur->next;
    }
    tq -> questoes = questoes;
    tq -> respostas=resp;
    return 0;
}

//Função que faz o parse do ficheiro "Users.xml" e "passa" os valores lidos para a estrutura de dados
int parseUser (xmlDocPtr doc, TAD_community tq) {
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    int id = -1, reputation = -1, posts = 0;
    char name[256] = "";
    char bio[16384] = ""; 
    GHashTable* users = g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,destroyUser);//g_hash_table_new(g_int_hash, g_direct_equal);
    xmlChar *uri;
    USER u = NULL;

    if (doc == NULL){
	fprintf(stderr,"Document not parsed successfully. \n");
	return 0;
    }

    if (cur == NULL) {
	fprintf(stderr,"empty document\n");
	return 1;
    }

    cur = cur->xmlChildrenNode;
    
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *)"row"))) {
	    uri = xmlGetProp(cur, (xmlChar*)"Id");
	    id = atoi((char*) uri);
	    xmlFree(uri);
	    
	    uri = xmlGetProp(cur, (xmlChar*)"DisplayName");
	    if (uri == NULL) strcpy(name, "NULL");
	    else strcpy (name,(char*)uri);
	    xmlFree(uri); 
	    
            uri = xmlGetProp(cur, (xmlChar*)"AboutMe");
	    if (uri == NULL) strcpy(bio, "NULL"); 
	    else strcpy (bio,(char*)uri);
	    xmlFree(uri);
	    
	    uri = xmlGetProp(cur, (xmlChar*)"Reputation");
	    reputation = atoi((char*)uri);
	    xmlFree(uri);
	    
	    u = newUser(id, reputation, name, posts, bio);
    	    g_hash_table_insert (users,GINT_TO_POINTER(u->id),u); // mais eficiente	
	}
	cur = cur->next;
    }
    
    tq -> users = users;
    return 0;
}

//Função que faz o parse do ficheiro "Votes.xml" e "passa" os valores lidos para a estrutura de dados
int parseVotes(xmlDocPtr doc, TAD_community tq){
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    int i = -2, j = -2;	
    xmlChar *uri;
    gpointer item_ptr = NULL;

    if (doc == NULL){
	fprintf(stderr,"Document not parsed successfully. \n");
	return 0;
    }

    if (cur == NULL){
	fprintf(stderr,"empty document\n");
	return 1;
	}

    cur = cur->xmlChildrenNode;
	
    while (cur != NULL){
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"row"))){
	    	uri = xmlGetProp(cur, (xmlChar*)"PostId");
	    	i = atoi((char*)uri);
	    	xmlFree(uri);
	    	
		item_ptr = g_hash_table_lookup(tq->respostas,GINT_TO_POINTER(i));
	    	if (item_ptr != NULL) {
	    		uri = xmlGetProp(cur, (xmlChar*)"VoteTypeId");
            	j = atoi((char*)uri);
            	xmlFree(uri);
	        
		if (j == 2) incrementa_votesUp (item_ptr);
            	else if (j == 3) incrementa_votesDown (item_ptr);
            	else if (j == 5) incrementa_favs (item_ptr);
	    	}      
	}

	cur = cur->next;
    
    }
	
	return 0;
}

//Função que faz o parse do ficheiro "Tags.xml" e "passa" os valores lidos para a estrutura de dados
int parseTags (xmlDocPtr doc, TAD_community tq) {
    xmlNodePtr cur = xmlDocGetRootElement(doc);
    int id = -1;
    char tag[256] = ""; 
    GHashTable* tags = g_hash_table_new_full(g_direct_hash,g_direct_equal,NULL,destroyTags);//g_hash_table_new(g_int_hash, g_direct_equal);
    xmlChar *uri;
    TAGS u = NULL;

    if (doc == NULL){
	fprintf(stderr,"Document not parsed successfully. \n");
	return 0;
    }

    if (cur == NULL) {
	fprintf(stderr,"empty document\n");
	return 1;
    }

    cur = cur->xmlChildrenNode;
    
    while (cur != NULL) {
	if ((!xmlStrcmp(cur->name, (const xmlChar *)"row"))) {
	    uri = xmlGetProp(cur, (xmlChar*)"Id");
	    id = atoi((char*) uri);
	    xmlFree(uri);
	    
	    uri = xmlGetProp(cur, (xmlChar*)"TagName");
	    if (uri == NULL) strcpy(tag, "NULL");
	    else strcpy (tag,(char*)uri);
	    xmlFree(uri); 
	    
 	    u = newTag(id,tag);
    	   g_hash_table_insert (tags,GINT_TO_POINTER(u->id),u); // mais eficiente	
	}
	cur = cur->next;
    }
    
    tq -> tags = tags;
    return 0;
}
