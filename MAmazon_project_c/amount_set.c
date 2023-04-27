#include <stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "amount_set.h"
#define INITIAL_SIZE 0


// sliman_majd_2k19

typedef  struct node {
    ASElement  element_node;
    double amount;
    struct node* next;


}*Node;

struct AmountSet_t{
    Node head;
    Node iterator;
    CopyASElement copy;
    FreeASElement free;
    CompareASElements compare;



};
static Node CreateNewNode(ASElement element,CopyASElement copy);

static void IteratorsReset(AmountSet set);
///////////////////////

AmountSet asCreate(CopyASElement copyElement, FreeASElement freeElement,CompareASElements compareElements){
    if (copyElement==NULL || freeElement==NULL ||compareElements==NULL){
        return NULL;
    }

    AmountSet set= malloc(sizeof(*set));
    if(!set) return NULL;

    set->head=NULL;
    set->iterator=NULL;
    set->copy=copyElement;
    set->free=freeElement;
    set->compare=compareElements;
    return set;



}


static Node CreateNewNode(ASElement element,CopyASElement copy){
    Node new_node= malloc(sizeof(*new_node));
    if(!new_node)return NULL;
    new_node->element_node=copy(element);
    new_node->next=NULL;
    new_node->amount=INITIAL_SIZE;
    return new_node;
}

void asDestroy(AmountSet set){

    if(!set){
        return;
    }

    if(asClear(set)==AS_NULL_ARGUMENT){

        return;
    }
    free(set);


}



AmountSet asCopy(AmountSet set){
    if(!set){

        return NULL;
    }

    AmountSet new_set= asCreate(set->copy, set->free,set->compare);
    if(!new_set){
        return NULL;
    }
    Node node_to_copy = set->head;
    while(node_to_copy){
        if(asRegister(new_set,node_to_copy->element_node) != AS_SUCCESS)
        {
            asDestroy(new_set);
            return NULL;
        }
        asChangeAmount(new_set,node_to_copy->element_node,node_to_copy->amount);

        node_to_copy=node_to_copy->next;

    }

    return new_set;
}









int asGetSize(AmountSet set){
    if(set==NULL){
        return  -1;
    }
    Node current_ptr=set->head;
    if(!current_ptr){
        return INITIAL_SIZE;
    }
    int counter=INITIAL_SIZE;
    while(current_ptr->next){
        counter++;
        current_ptr=current_ptr->next;
    }
    counter++;

    return counter;
}


bool asContains(AmountSet set, ASElement element)  {
    if(!set || !element || !set->head){
        return false;
    }
    Node current=set->head;

    while(current){

        if (!(set->compare(current->element_node, element)))
        {
            return true;
        }

        current=current->next;
    }
    return false;
}


AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount){
    if(!set || !element || !outAmount){
        return AS_NULL_ARGUMENT;
    }
    if(!asContains(set,element) || !(set->head)){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    Node current=set->head;

    while(current){
        if(!(set->compare(current->element_node,element))){
            *outAmount=(current->amount);
            return AS_SUCCESS;
        }
        current=current->next;
    }
    return AS_ITEM_DOES_NOT_EXIST;
}





AmountSetResult asRegister(AmountSet set, ASElement element){

    if (!set || !element) {
        return AS_NULL_ARGUMENT;
    }
    if (asContains(set, element) == true) {
        return AS_ITEM_ALREADY_EXISTS;
    }
    Node new=CreateNewNode(element,set->copy);
    if(!new) return AS_NULL_ARGUMENT;
    if (asGetSize(set) == 0)
    {
        set->head = new;
        new->next = NULL;
        set->iterator = new;
        return AS_SUCCESS;
    }
    Node ptr = set->head;
    Node prev_ptr =NULL;////////////////
    if (set->compare(ptr->element_node,element)>0)
    {
        new->next=set->head;
        set->head=new;
        return AS_SUCCESS;
    }
    while (ptr->next != NULL && set->compare(ptr->element_node, element) < 0)
    {
        prev_ptr = ptr;
        ptr = ptr->next;
    }
    if (ptr!=NULL && set->compare(ptr->element_node,element)>0)
    {
        new->next=ptr;
        prev_ptr->next=new;
        return AS_SUCCESS;
    }
    if(ptr || ptr->next==NULL )///////////////////
    {
        if(set->compare(ptr->element_node,element)<0)
        {
            ptr->next = new;
            new->next = NULL;
            return AS_SUCCESS;
        }
    }

    return  AS_SUCCESS;
}



AmountSetResult asChangeAmount (AmountSet set, ASElement element, const double amount){
    if(!set|| !element  ) {
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element) ==false){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    Node ptr_change =set->head;
    while (set->compare(ptr_change->element_node,element) !=0) {
        ptr_change = ptr_change->next;
        if (ptr_change == NULL) {
            return AS_ITEM_DOES_NOT_EXIST;
        }
    }
    if((amount+(ptr_change->amount))<0)
    {
        return AS_INSUFFICIENT_AMOUNT;
    }
    ptr_change->amount += amount;
    return AS_SUCCESS;


}


AmountSetResult asDelete(AmountSet set, ASElement element) {
    if (!set || !element) {
        return AS_NULL_ARGUMENT;
    }

    if ((!asContains(set, element))) {
        return AS_ITEM_DOES_NOT_EXIST;
    }
    IteratorsReset(set);
    if(asGetSize(set)==1)
    {
        asClear(set);
        return AS_SUCCESS;
    }

    Node current=set->head;
    set->iterator=set->iterator->next;
    if(!set->compare(set->head->element_node,element)){
        free(set->head);
        set->head=set->iterator;
        return AS_SUCCESS;

    }
    IteratorsReset(set);
    while(current){
        int compare_value;
        set->iterator=set->iterator->next;
        if(!set->iterator){
            return AS_SUCCESS;
        }
        compare_value=set->compare(set->iterator->element_node,element);
        if(!compare_value){
            current->next=set->iterator->next;
            set->free(set->iterator->element_node);
            free(set->iterator);

            return AS_SUCCESS;
        }
        current=current->next;
    }
    return  AS_SUCCESS;
}






AmountSetResult asClear(AmountSet set){
    if(!set){
        return AS_NULL_ARGUMENT;
    }
    Node ptr= set->head;
    while(ptr){
        Node to_delete=ptr;
        ptr=ptr->next;
        set->free(to_delete->element_node);
        free(to_delete);

    }
    set->head=NULL;
    return AS_SUCCESS;
}


ASElement asGetFirst(AmountSet set){
    if(!set || !(set->head) ){
        return NULL;
    }
    set->iterator=set->head;
    return set->iterator->element_node;
}



ASElement asGetNext(AmountSet set){

    if(!set || !(set->head)|| !(set->iterator)|| !(set->iterator->next)){
        return NULL;
    }

    set->iterator=set->iterator->next;

    return set->iterator->element_node;

}


static void IteratorsReset(AmountSet set){
    assert(set !=NULL);
    set->iterator=set->head;
}



