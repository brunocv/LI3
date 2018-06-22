#include <stdio.h>
#include <stdlib.h>
#include "load.h"
#include "date.h"
#include "pair.h"
#include "interface.h"
#include <time.h>

int main () {

    clock_t t;
    t = clock();

    //char *path = "/Users/JoaoPimentel/downloads/dump exemplo/ubuntu/";
    //char *path = "/Users/JoaoPimentel/downloads/dump exemplo/android/";
    //char *path = "/home/user/Desktop/android/";
    char *path = "/home/user/Desktop/ubuntu/";
    
    TAD_community tq = init();
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took %f seconds to init\n\n", time_taken);
    
    printf("****************************************************************************\n\n");

    
    t = clock();
    tq = load(tq,path);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took %f seconds to load\n\n", time_taken);
    
    printf("****************************************************************************\n\n");


    t = clock();
    //STR_pair s = info_from_post(tq, 3678); /* Exemplo com uma pergunta */

    STR_pair s = info_from_post(tq, 2); /*Exemplo com uma resposta */
    printf("%s\n",get_fst_str(s));
    printf("%s\n",get_snd_str(s));

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took %f seconds for query 1\n\n", time_taken);

    printf("****************************************************************************\n\n");
    
    t = clock();
    LONG_list l = top_most_active(tq, 10);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    for (int i = 0; i < 10; i++){
        printf ("Top %d: %ld\n",i+1,get_list(l, i));
    }
    printf("Took %f seconds for query 2\n\n", time_taken);

    printf("****************************************************************************\n\n");

    t = clock();
    LONG_pair p = total_posts(tq, createDate(28,2,2016),createDate(2,3,2016));
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Questões: %ld\n", get_fst_long(p));
    printf("Respostas: %ld\n", get_snd_long(p));
    printf("Took %f seconds for query 3\n\n", time_taken);
   
    printf("****************************************************************************\n\n");
    
    t = clock();
    LONG_list L4 = questions_with_tag(tq, "ssh" ,createDate(28,2,2016),createDate(2,3,2016));
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    for (int i = 0; i < get_size(L4); i++){
        printf ("With tag (%d): %ld\n",i+1,get_list(L4, i));
    }
    printf("Took %f seconds for query 4\n\n", time_taken);
    
    printf("****************************************************************************\n\n");

    t = clock();
    USER u = get_user_info(tq,30); 
    long *posts;
    posts = get_10_latest_posts(u); 
    t = clock() - t;
    printf("%s\n",get_bio(u));
    for (int j = 0; j < 10; j++)
        printf("\n Post %d  - %ld \n",j,posts[j]);
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took %f seconds for query 5\n\n", time_taken);

    printf("****************************************************************************\n\n");

    t = clock();
    LONG_list l2 = most_voted_answers(tq, 30,createDate(6,9,2017), createDate(8,11,2017));
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    for (int i = 0; i < 30; i++){
        printf ("Top %d: %ld\n",i+1,get_list(l2, i));
    }
    printf("Took %f seconds for query 6\n\n", time_taken);

    printf("****************************************************************************\n\n");

    t = clock();
    LONG_list l3 = most_answered_questions(tq, 30,createDate(6,9,2017), createDate(29,10,2017));
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    for (int i = 0; i < 30; i++){
        printf ("Top %d: %ld\n",i+1,get_list(l3, i));
    }
    printf("Took %f seconds for query 7\n\n", time_taken);

    printf("****************************************************************************\n\n");
    
    t = clock();
    LONG_list l8 = contains_word(tq,"glib", 10);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    for (int i = 0; i < 10; i++){
        printf ("Top %d: %ld\n",i+1,get_list(l8, i));
    }
    printf("Took %f seconds for query 8\n\n", time_taken);

    printf("****************************************************************************\n\n");
    
    //29 27 tem de dar 2 no android
    //tem de dar 32 valores com o que está na main, no ubuntu, testar isto no android!!
    t = clock();
    LONG_list lista= both_participated(tq, 15811, 449, 50);
    //LONG_list lista= both_participated(tq,29, 27, 50);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    for (int i = 0; i < 50; i++){
        printf ("LONG [%d] = %ld\n",i,get_list(lista, i));
    }
    printf("Took %f seconds for query 9\n\n", time_taken);
    
    printf("****************************************************************************\n\n");

    t = clock();
    long l4 = better_answer(tq,6424);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf ("Melhor resposta tem id de: %ld\n",l4);
    printf("Took %f seconds for query 10\n\n", time_taken);

    printf("****************************************************************************\n\n");

    t = clock();
    LONG_list li= most_used_best_rep (tq, 50, createDate(15,9,2015), createDate(29,10,2017));
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    for (int i = 0; i < 50; i++){
        printf ("LONG [%d] = %ld\n",i,get_list(li, i));
    }
    printf("Took %f seconds for query 11\n\n", time_taken);
 
    printf("****************************************************************************\n\n");
    
    t = clock();
    tq = clean(tq);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Took %f seconds to clean\n\n", time_taken);

    printf("****************************************************************************\n\n");


    return 0;
}
