//
// Created by slima on 12/6/2019.
//

#ifndef HW1_PRODUCTS_H
#define HW1_PRODUCTS_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "amount_set.h"
#include "matamazom.h"
#include "set.h"



typedef struct product *Product;



bool productContains(AmountSet set,unsigned int id);


bool addProductToWarhouse(AmountSet products,unsigned int id,const char *name,double amount,
                          MatamazomAmountType amountType,MtmProductData customData, MtmCopyData copyData,
                          MtmFreeData freeData, MtmGetProductPrice prodPrice);

Product newProductCreat( unsigned int id,const char *name,double amount,
                         MatamazomAmountType amountType, MtmProductData customData, MtmCopyData copyData,
                         MtmFreeData freeData, MtmGetProductPrice prodPrice);
MatamazomAmountType getAmountType(AmountSet products,unsigned int productId);

Product getProductById(AmountSet products,unsigned int id);

bool productContainsInOrder(Set set,unsigned int id);


unsigned int GetProductId(Product product);

double getProfit(Product product);
double getProductPrice(Product product,double amount);
void changeProfit(double profits,Product product);


int compareProduct(void* product1, void* product2);


void freeProduct(void* product1);


void* copyProduct(void* newproduct);



char* getProductName(AmountSet products, unsigned int productId);


MtmProductData getProductCostumData(Product get_product);
#endif //HW1_PRODUCTS_H
