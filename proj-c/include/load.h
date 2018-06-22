#include <libxml/tree.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <wchar.h>
#include <ctype.h>
#include <gmodule.h>
#include "struct.h"

int parseQuestion(xmlDocPtr doc, TAD_community tq);

int parseUser (xmlDocPtr doc, TAD_community tq);

void incrementa_posts (GHashTable* a,gpointer id);

void incrementa_votesUp (gpointer item_ptr);

void incrementa_votesDown (gpointer item_ptr);

int parseVotes(xmlDocPtr doc, TAD_community tq);

int parseTags (xmlDocPtr doc, TAD_community tq); 
