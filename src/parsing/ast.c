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
                //si le séparateur a plus d'un enfant ou que c'est la tête et
                //qu'elle a déjà un enfant
                if(*current.nb_child >1||
                        (*current.type == NODE_HEAD &&
                         *current.nb_child >0))
                {
                    //erreur de grammaire
                }
                new.type = NODE_COMMAND;
                create_command(&new);
                new->father = current;
                *current.nb_child++;
                if(*current->child ==NULL)
                *current->child = &new;
                else
                    **current->child->sibling=&new;
                break;
            case ARGUMENT:
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
                    new->father = current;
                }
                create_argument(&new);
                break;
            case NODE_AND_BOOL:
                if(is_separator(current.type))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(*current.type==NODE_ARGUMENT)
                {
                    //car un and ne peut pas séparer deux arg
                    current=*current->father;
                }
                while(is_separator(*current->fahter))
                {
                    //car le && doit avoir lieu après l'exec des précédents
                    current = *current->father;
                }
                new.type = NODE_AND_BOOL;
                create_and_bool(&new);
                new->father = *current->father;
                *current->father = &new;
                new->child = current;
                new.nb_child ++;
                break
            case NODE_OR_BOOL:
                if(is_separator(current.type))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(*current.type==NODE_ARGUMENT)
                {
                    //car un or ne peut pas séparer deux arg
                    current=*current->father;
                }
                while(is_separator(*current->fahter))
                {
                    //car le || doit avoir lieu après l'exec des précédents
                    current = *current->father;
                }
                new.type = NODE_OR_BOOL;
                create_or_bool(&new);
                new->father = *current->father;
                *current->father = &new;
                new->child = current;
                new.nb_child ++;
                break;
            case NODE_AND:
                if(is_separator(current.type))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                else if(*current.type==NODE_ARGUMENT)
                {
                    //car un and ne peut pas séparer deux arg
                    current=*current->father;
                }
                while(is_separator(*current->fahter))
                {
                    //car le & doit avoir lieu après l'exec des précédents
                    current = *current->father;
                }
                new.type = NODE_AND;
                create_and(&new);
                new->father = *current->father;
                *current->father=&new;
                new->child = current;
                new.nb_child++;
                break;
            case NODE_DRIGHT_CHEVRON:
                if(is_separator(current.type))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                while(is_separator(*current->fahter))
                {
                    //car le < doit avoir lieu après l'exec des précédents
                    current = *current->father;
                }
                new.type = NODE_DRIGHT_CHEVRON;
                create_dright_chevron(&new);
                new->father = *current->father;
                *current->father=&new;
                new->child = current;
                new.nb_child++;
                break;
            case NODE_DLEFT_CHEVRON:
                if(is_separator(current.type))
                {
                    //erreur de grammaire(deux separateur se suivent)
                }
                while(is_separator(*current->fahter))
                {
                    //car le < doit avoir lieu après l'exec des précédents
                    current = *current->father;
                }
                new.type = NODE_DLEFT_CHEVRON;
                create_dleft_chevron(&new);
                new->father = *current->father;
                *current->father=&new;
                new->child = current;
                new.nb_child++;
                break;
        }
        current = &new;
        p = p->next;
    }
}

void free_ast(ast_node node)
{
    //free récursivement le contenu des nodes, avant de free les nodes
    //elle-même (il faut commencer depuis la racine)
    switch(node.type)
    {
        case NODE_HEAD:
            free(node.data->node_head);
            break;
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
    //vérifie si la node est un séparateur
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

void create_or_bool(ast_node* new)
{
    struct node_or_bool new_data = calloc(sizeof(struct node_or_bool),1);
    *new.data->node_or_bool=&new_data;
    new_data->node = new;
}

void create_dleft_chevron(ast_node* new)
{
    struct node_dleft_chevron new_data = 
        calloc(sizeof(struct node_dleft_chevron),1);
    *new.data->node_dleft_chevron=&new_data;
    new_data->node = new;
}

void create_dright_chevron(ast_node* new)
{
    struct node_dright_chevron new_data = 
        calloc(sizeof(struct node_dright_chevron),1);
    *new.data->node_dright_chevron=&new_data;
    new_data->node = new;
}

void create_and(ast_node* new)
{
    struct node_and new_data = calloc(sizeof(struct node_and),1);
    *new.data->node_and=&new_data;
    new_data->node = new;
}

void create_and_bool(ast_node* new)
{
    struct node_and_bool new_data = calloc(sizeof(struct node_and_bool),1);
    *new.data->node_and_bool = &new_data;
    new_data->node = new;
}

void create_argument(ast_node *new)
{
    struct node_argument new_data =calloc(sizeof(struct node_argument),1);
    *new.data->node_argument = &new_data;
    new_data->node = new;

}

void create_command(ast_node *new)
{
    struct node_command new_data = calloc(sizeof(struct node_command),1);
    *new.data->node_command = &new_data;
    check_command(new);
    new_data->node = new;
}

void check_command(ast_node* new)
{
    /*
       vérifie si la commande entrée est valide
       */
    switch(*new.string)
    {
        case "cat":
            *new.data->node_command.cmd = CAT;
            break;

            //ajouter ici les prochaines commandes

        default:
            if(strlen(*new.string)>2)
            {
                if(*new.string[0]=='/'||
                        (*new.string[1]=='/'&&*new.string[0]=='.'))
                {
                    //vérifie si le fichier est bien un exécutable
                    if(access(*new.string,X_OK == 0))
                        *new.data->node_command.cmd = DEFAULT;
                    else
                        *new.type = NODE_UNKNOWN;
                    free(*new.data->node_command);
                    struct node_unknown data=
                        calloc(sizeof(struct node_unknown),1);
                    *new.data->node_unknown = &data;
                    data.type = EXEC_NOT_FOUND;
                }
            }
            break;
    }
}

