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
    current.type = NODE_HEAD;
    list_elm* p = list->head;
    while(p!=NULL)
    {
        token t = p->token;
        ast_node new = calloc(sizeof(ast_node),1);
        new->string = t->string;
        switch(t.type)
        {
            case COMMAND:
                //remonte jusqu'au dernier séparateur/la tête
                while(!is_separator(current)&&
                        *current.type!=NODE_HEAD)
                {
                    current = current->father;
                }
                //si le séparateur a plus d'un enfant ou 
                if(*current.nb_child >1||
                        (*current.type == NODE_HEAD &&
                         *current.nb_child >0))
                {
                    //erreur de grammaire
                }
                new.type = NODE_COMMAND;
                create_command(new);
                new->father = current;
                *current.nb_child++;
                if(*current->child ==NULL)
                *current->child = &new;
                else
                    **current->child->sibling=&new;
                break;
            case ARGUMENT:
                // attention : si c'est un flag, on le met directement dans la 
                // node père (la node de la commande)
                new.type = NODE_ARGUMENT;
                new->father = current;
                //check si le current est un argument, ou si le père est un 
                //séparateur ou une commande et que son premier fils est diffé
                //de NULL
                if(*current.type==NODE_ARGUMENT||
                        ((is_separator(*current->father)&&
                          **current->father.nb_child == 1)||
                         **current->father==NODE_COMMAND))
                {
                    current->sibling = &new;
                    new->father = current->father;
                    *current->father.nb_child++;
                }
                else if(is_separator(*current->father)&&
                        **current->father.nb_child>1)
                {
                    //error dans la grammaire
                }
                else
                {
                    current->child = &new;
                    new-father = current;
                }
                break;
            case NODE_AND_BOOL:
                
                break
            case NODE_OR_BOOL:
                break;
            case NODE_AND:
                break;
            case NODE_DRIGHT_CHEVRON:
                break;
            case NODE_DLEFT_CHEVRON:
                break;
        }
        current = &new;
        p = p->next;
    }
}

void free_ast(ast_node node)
{
    switch(node.type)
    {
        case NODE_AND_BOOL:
            free(node.data->node_and_bool);
            break;
        case NODE_OR_BOOL:
            free(node.data->node_or_bool);
            break;
        case NODE_AND:
            free(node.data->node_and);
            break;
        case NODE_DRIGHT_CHEVRON:
            free(node.data->node_dright_chevron);
            break;
        case NODE_DLEFT_CHEVRON:
            free(node.data->node_dleft_chevron);
            break;
        case NODE_UNKNOWN:
            free(node.data->node_unknown);
            break;
        case NODE_COMMAND:
            free(node.data->node_command);
            break;
        case NODE_ARGUMENT:
            free(node.data->node_argument);
            break;
        case NODE_SEMI_COLON:
            free(node.data->node_semi_colon);
            break;
        case NODE_LEFT_PAREN:
            free(node.data->node_left_paren);
            break;
        case NODE_RIGHT_PAREN:
            free(node.data->node_right_paren);
            break;
        case NODE_PIPE:
            free(node.data->node_pipe);
            break;
        case NODE_QUOTE:
            free(node.data->node_quote);
            break;
        case NODE_DOUBLE_QUOTES:
            free(node.data->node_double_quotes);
            break;
        case NODE_BACKTICK:
            free(node.data->node_backtick);
            break;
        default:
            //an error occured
    }
    free_ast(*node->child);
    free_ast(*node->sibling);
    free(node);
}

int is_separator(ast_node* current)
{
    switch(*current.type)
    {
        case NODE_AND_BOOL:
        case NODE_OR_BOOL:
        case NODE_AND:
        case NODE_DRIGHT_CHEVRON:
        case NODE_DLEFT_CHEVRON:
            return 1;
        default:
            return 0;
    }
}

void create_argument(ast_node new)
{
    node_argument new_data =calloc(sizeof(struct node_argument),1);
    new.data->node_argument = &new_data;
    new_data->node = new;

}

void create_command(ast_node new)
{
    node_command new_data = calloc(sizeof(struct node_command),1);
    new.data->node_command = &new_data;
    check_command(new);
    new_data->node = new;
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

