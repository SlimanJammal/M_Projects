//
// Created by slima on 12/6/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matamazom.h"
#include "products.h"
#include "amount_set.h"
#include "set.h"

struct product{

    unsigned int id;
    char *name;
    double profit;
    MatamazomAmountType amountType;
    MtmProductData  customData;
    MtmCopyData copyData;
    MtmFreeData freeData;
    MtmGetProductPrice prodPrice;


};


unsigned int get_product_id(Product product){
    return product->id;
}

bool productContains(AmountSet set,unsigned int id){

    AS_FOREACH(Product,iterator,set){
        if(get_product_id(iterator)==id){
            return true;
        }

    }

    return false;
}




bool productContainsInOrder(Set set,unsigned int id){

    SET_FOREACH(Product,iterator,set){
        if(get_product_id(iterator)==id){
            return true;
        }

    }

    return false;
}




bool addProductToWarhouse(AmountSet products,unsigned int id,const char *name,double amount,
                          MatamazomAmountType amountType,MtmProductData customData, MtmCopyData copyData,
                          MtmFreeData freeData, MtmGetProductPrice prodPrice){

    Product new_product= newProductCreat(id, name,amount,amountType,customData,copyData,freeData,prodPrice);
    if(!new_product){
        return false;
    }
    if(asRegister(products,new_product) != AS_SUCCESS){
        freeProduct(new_product);
        return false;
    }
    asChangeAmount(products,new_product,amount);

    freeProduct(new_product);
    return true;
}





Product newProductCreat(unsigned int id,const char *name,double amount,
                        MatamazomAmountType amountType,MtmProductData customData, MtmCopyData copyData,
                        MtmFreeData freeData, MtmGetProductPrice prodPrice){

    Product New_product=malloc(sizeof(*New_product));
    if(!New_product){
        return NULL;
    }

    New_product->id=id;
    New_product->name=malloc(strlen(name)+1);
    if(!New_product->name){
        free(New_product);
        return NULL;
    }
    strcpy(New_product->name,name);
    New_product->profit=0;
    New_product->amountType=amountType;
    New_product->customData=copyData(customData);
    New_product->copyData=copyData;
    New_product->freeData=freeData;
    New_product->prodPrice=prodPrice;

    return New_product;
}

MatamazomAmountType getAmountType(AmountSet set,unsigned int productId){

    AS_FOREACH(Product,iterator,set) {
        if (get_product_id(iterator) == productId) {
            return iterator->amountType;
        }
    }
    return MATAMAZOM_INTEGER_AMOUNT;//TODO

}



Product getProductById(AmountSet products,unsigned int id){

    AS_FOREACH(Product ,iterator,products){
        if(GetProductId(iterator)==id){
            return iterator;
        }
    }
    return NULL;///////////////////////////////////////////////////////////////////////////////////////////////////////////
}



unsigned int GetProductId(Product product){
    return product->id;
}



double getProfit(Product product){

    return product->profit;

}


double getProductPrice(Product product,double amount){
    return product->prodPrice(product->customData,amount);

}



void changeProfit(double profits,Product product){

    product->profit=profits;



}

int compareProduct(void* product1, void* product2) {
    Product copyproduct1 = (Product) product1;
    Product copyproduct2 = (Product) product2;
    return ((int) copyproduct1->id - (int) copyproduct2->id);
}
/*
ASElement copy_set_elements(products product) {
    return product->copyData(product->product_name);
}
*/
void freeProduct(void* product1) {
    Product product =(Product) product1;
    if(!product){
        return;
    }
    product->freeData(product->customData);
    free(product->name);
    free(product);
}

void* copyProduct(void* newproduct) {
    /*
    if(!product){
        return NULL;
    }
    products newProducta=create_product(product->id,product->product_name,)
     */
    Product product = (Product)newproduct;
    Product new_product = malloc(sizeof(*product));
    if (!new_product) {
        return NULL;
    }
    new_product->name = malloc((strlen(product->name) + 1));
    if (!new_product->name) {
        free(new_product);
        return NULL;
    }
    strcpy(new_product->name,product->name);
    new_product->id = product->id;
    new_product->profit = product->profit;
    new_product->prodPrice=product->prodPrice;
    new_product->copyData = product->copyData;
    new_product->freeData = product->freeData;
    new_product->amountType=product->amountType;
    new_product->customData = product->copyData(product->customData);
    return (void*)new_product;

}


char* getProductName(AmountSet products, unsigned int productId){

    Product productt=getProductById(products,productId);
    if(!productt){
        return NULL;
    }
    return productt->name;

}

MtmProductData getProductCostumData(Product get_product){
    return get_product->customData;
}

