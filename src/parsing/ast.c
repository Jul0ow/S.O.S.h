#include "ast.h"
#include "xmalloc.h"
#include <unistd.h>
#include <string.h>
#include "../lexing/lexer.h"
#include "../utils/list.h"

ast_node creating_ast(list* list)
{
    ast_node first = calloc(sizeof(ast_node),1);
    ast_node* current = &first;
    list_elm* p = list->head;
    while(p!=NULL)
    {
        token t = p->token;
        ast_node new = calloc(sizeof(ast_node),1);
        new->string = t->string;
        switch(t.type)
        {
            case COMMAND:
                new.type = NODE_COMMAND;
                node_command new_data = calloc(sizeof(struct node_command),1);
                new.data->node_command = &new_data;
                check_command(new);
                new_data->node = new;
                new->father = current;
                current->child = &new;
                break;
            case ARGUMENT:
                // attention : si c'est un flag, on le met directement dans la 
                // node père (la node de la commande)
                new.type = NODE_ARGUMENT;
                node_argument new_data =calloc(sizeof(struct node_argument),1);
                new.data->node_argument = &new_data;
                new_data->node = new;
                new->father = current;
                if(*current.type==NODE_ARGUMENT)
                    current->sibling = &new;
                else
                    current->child = &new;
                break;
        }
        current = &new;
        p = p->next;
    }
}

void check_command(ast_node new)
{
    /*
      vérifie si la commande entrée est valide
     */
    switch(new.string)
    {
        case "cat":
            new.data->node_command.cmd = CAT;
            break;

        //ajouter ici les prochaines commandes

        default:
            if(strlen(new.string)>2)
            {
                if(new.string[0]=='/'||
                        (new.string[1]=='/'&&new.string[0]=='.'))
                {
                    //vérifie si le fichier est bien un exécutable
                    if(access(new.string,X_OK == 0))
                        new.data->node_command.cmd = DEFAULT;
                    else
                        new.type = NODE_UNKNOWN;
                    free(new.data->node_command);
                    struct node_unknown data=
                        calloc(sizeof(struct node_unknown),1);
                    new.data->node_unknown = &data;
                    data.type = EXEC_NOT_FOUND;
                }
            }
            break;
    }
}

