//
// Created by slima on 12/4/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matamazom.h"
#include "matamazom_print.h"
#include "set.h"
#include "amount_set.h"
#include "products.h"
#include "orders.h"
#include <math.h>

/////////////////defines




/////////////////struct
struct Matamazom_t{
    AmountSet products;
    Set orders;
};


///////////////

static bool AmountCheck(double amount,MatamazomAmountType amountType);
static bool NameCheck(const char* name);
static bool ContainsOrder(Matamazom matamazom,  unsigned int orderId);
static unsigned int new_id=0;
/////////////////////
Matamazom matamazomCreate()
{
    Matamazom new_inventory = malloc(sizeof(*new_inventory));
    if(!new_inventory) return NULL;

    new_inventory -> products = asCreate(copyProduct,freeProduct,compareProduct);////////////////
    if(!new_inventory->products){
        return NULL;
    }
    new_inventory -> orders = setCreate(copy_order,free_orders,compare_orders);////////////////
    if(!new_inventory->orders) {
        return NULL;
    }
    return new_inventory;
}


void matamazomDestroy(Matamazom matamazom){
    if(!matamazom){
        return;
    }
    asDestroy(matamazom->products);
    setDestroy(matamazom->orders);

    free(matamazom);

}



MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id, const char *name, const double amount,
                              const MatamazomAmountType amountType,MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice){

    if(!matamazom || !name || !customData || !copyData || !freeData || !prodPrice){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(!NameCheck(name)){
        return MATAMAZOM_INVALID_NAME;

    }
    if(!(AmountCheck(amount,amountType))|| amount<0){
        return MATAMAZOM_INVALID_AMOUNT;
    }
    if(productContains(matamazom->products,id)){
        return MATAMAZOM_PRODUCT_ALREADY_EXIST;
    }

    if(addProductToWarhouse(matamazom->products,id, name,amount,amountType,customData,copyData,freeData,prodPrice)){
        return MATAMAZOM_SUCCESS;
    }else return MATAMAZOM_OUT_OF_MEMORY;




}



MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount){

    if(!matamazom){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(!productContains(matamazom->products,id)){
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    if(!AmountCheck(amount,getAmountType(matamazom->products,id))){
        return MATAMAZOM_INVALID_AMOUNT;
    }///////////////////////////
    double getAmount=0;
////////get product by id
    asGetAmount(matamazom->products,getProductById(matamazom->products,id),&getAmount);
    if(amount<0 && -amount>getAmount){
        return MATAMAZOM_INSUFFICIENT_AMOUNT;
    }
    if(asChangeAmount(matamazom->products,getProductById(matamazom->products,id),amount)==AS_SUCCESS){
        return MATAMAZOM_SUCCESS;
    }

    return MATAMAZOM_NULL_ARGUMENT;

}




MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id){
    if(!matamazom){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(!productContains(matamazom->products,id)){
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    Product  tempProduct=getProductById(matamazom->products,id);

    if(asDelete(matamazom->products,tempProduct)==AS_SUCCESS){
        SET_FOREACH(Order,iteratorr,matamazom->orders){
            deleteProductFromOrder(matamazom->orders,GetOrderId(iteratorr),id);///////TODO
        }

        return MATAMAZOM_SUCCESS;
    }
    return MATAMAZOM_NULL_ARGUMENT;
}



unsigned int mtmCreateNewOrder(Matamazom matamazom){
    if(!matamazom){
        return 0;
    }

    new_id++;
    Order  newOrder=createOrder(new_id);
    if(!(newOrder)){
        return 0;
    }
    if(setAdd(matamazom->orders,newOrder) != SET_SUCCESS){//TODO
        free_orders(newOrder);
        return 0;
    }
    free_orders(newOrder);//////////////////////////////////////////////////CHECK WHY

    return new_id;
}




MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                              const unsigned int productId, const double amount) {
    if (!matamazom || !orderId || !productId) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if (!ContainsOrder(matamazom, orderId)){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    if (!productContains(matamazom->products, productId)) {
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    if (amount == 0.0) {
        return MATAMAZOM_SUCCESS;
    }
    if (!AmountCheck(amount, getAmountType(matamazom->products,productId))) {
        return MATAMAZOM_INVALID_AMOUNT;
    }

    if (amount > 0 && !ContainsProductInOrder(matamazom->orders, orderId, productId)) {
        AddProductInOrder(matamazom->products,matamazom->orders, orderId, productId,amount);
        return MATAMAZOM_SUCCESS;
    }
    if(amount<0 && getProductAmountInOrder(matamazom->products,matamazom->orders,orderId,productId) <= (-amount) ){

        asDelete(getOrderSet(matamazom->orders,orderId),getProductById(matamazom->products,productId));
        return MATAMAZOM_SUCCESS;
    }


    changeAmountInOrder(matamazom->orders,orderId,productId,amount);
    return MATAMAZOM_SUCCESS;






}

MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId){
    if(!matamazom){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Order orderGet= GetOrderByIdd(matamazom->orders,orderId);
    if(orderGet== NULL){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    AmountSet order_set= getOrderSet(matamazom->orders,orderId);
    Product iterator= asGetFirst(order_set);

    double amount = 0, amountt = 0, amounttt = 0;

    while(iterator) {
        asGetAmount(order_set, iterator, &amount);
        asGetAmount(matamazom->products, iterator, &amountt);
        if ((amountt - amount) < 0) {
            return MATAMAZOM_INSUFFICIENT_AMOUNT;
        }
        iterator = asGetNext(order_set);
    }
    iterator=asGetFirst(order_set);

    while(iterator){

        // unsigned int productIdd = GetProductId(iterator);
        asGetAmount(order_set,iterator,&amounttt);
        double profits= getProfit(iterator);
        profits += getProductPrice(iterator,amounttt);
        AS_FOREACH(Product,iterator2,matamazom->products){
            if(compareProduct(iterator,iterator2)==0){
                changeProfit(profits,iterator2);
                asChangeAmount(matamazom->products,iterator2,-1 * amounttt);
            }
        }

        iterator=asGetNext(order_set);
    }
    setRemove(matamazom->orders,orderGet);


    return  MATAMAZOM_SUCCESS;

}


MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId){

    if(!matamazom){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(!ContainsOrder(matamazom,orderId)){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    Order temp=GetOrderByIdd(matamazom->orders,orderId);
    setRemove(matamazom->orders,temp);


    return MATAMAZOM_SUCCESS;
}




MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output){
    if(!matamazom || !output){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(asGetSize(matamazom->products)==0) {
        fprintf(output, "Inventory Status:\n");
        return MATAMAZOM_SUCCESS;
    }
    fprintf(output, "Inventory Status:\n");
    AS_FOREACH(Product ,iterator,matamazom->products){
        unsigned int product_id=GetProductId(iterator);
        double product_amount=0;
        asGetAmount(matamazom->products,getProductById(matamazom->products,product_id),&product_amount);
        mtmPrintProductDetails(getProductName(matamazom->products,product_id),product_id,product_amount,
                               getProductPrice(getProductById(matamazom->products,product_id),1),output);
    }
    return MATAMAZOM_SUCCESS;

}



MatamazomResult mtmPrintOrder(Matamazom matamazom, const unsigned int orderId, FILE *output){
    if(!matamazom || !output){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(!ContainsOrder(matamazom,orderId)){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    if(asGetSize(getOrderSet(matamazom->orders,orderId))==0){
        mtmPrintOrderHeading(orderId,output);
        mtmPrintOrderSummary(0,output);
        return MATAMAZOM_SUCCESS;
    }
    double total_price=0;
    mtmPrintOrderHeading(orderId,output);
    AS_FOREACH(Product ,iterator,getOrderSet(matamazom->orders,orderId)){
        mtmPrintProductDetails(getProductName(matamazom->products,GetProductId(iterator)),GetProductId(iterator),
                          getProductAmountInOrder(matamazom->products,matamazom->orders,orderId,GetProductId(iterator))
  ,getProductPrice(iterator,
          getProductAmountInOrder(matamazom->products,matamazom->orders,orderId,GetProductId(iterator))),output);

   total_price+=getProductPrice(iterator,
           getProductAmountInOrder(matamazom->products,matamazom->orders,orderId,GetProductId(iterator)));
    }

    mtmPrintOrderSummary(total_price,output);
    return MATAMAZOM_SUCCESS;
}




MatamazomResult mtmPrintBestSelling(Matamazom matamazom, FILE *output){
    if(!matamazom || !output){
        return MATAMAZOM_NULL_ARGUMENT;
    }


    fprintf(output,"Best Selling Product:\n");

    unsigned int biggest_profit_id=0;
    double biggest_profit=0;
    AS_FOREACH(Product,iterator,matamazom->products){
        if(getProfit(iterator)>biggest_profit){
            biggest_profit_id = GetProductId(iterator);
            biggest_profit = getProfit(iterator);
        }
        if(getProfit(iterator)==biggest_profit){
            if(GetProductId(iterator)<biggest_profit_id){
                biggest_profit_id=GetProductId(iterator);
                biggest_profit=getProfit(iterator);
            }
        }

    }
    if(biggest_profit==0){
        fprintf(output,"none\n");
        return MATAMAZOM_SUCCESS;
    }
    mtmPrintIncomeLine(getProductName(matamazom->products,biggest_profit_id),biggest_profit_id,biggest_profit,output);
    return MATAMAZOM_SUCCESS;
}



MatamazomResult mtmPrintFiltered(Matamazom matamazom, MtmFilterProduct customFilter, FILE *output){
    if(!matamazom || !output || !customFilter){
        return MATAMAZOM_NULL_ARGUMENT;
    }

    AS_FOREACH(Product ,iterator,matamazom->products){
        unsigned int product_id=GetProductId(iterator);
        double product_amount=0;
        asGetAmount(matamazom->products,getProductById(matamazom->products,product_id),&product_amount);

        if(customFilter(product_id,getProductName(matamazom->products,product_id),product_amount,
                        getProductCostumData(iterator))){
            mtmPrintProductDetails(getProductName(matamazom->products, product_id), product_id, product_amount,
                            getProductPrice(getProductById(matamazom->products, product_id), 1), output);
        }
    }
    return MATAMAZOM_SUCCESS;

}


///==================================================================================
///==================================================================================
///Static Functions.


static bool AmountCheck(double amount,const MatamazomAmountType amountType){
    amount=fabs(amount);
    if(amountType==MATAMAZOM_INTEGER_AMOUNT){

        if(fabs(amount-(int)amount)<=0.0010000000000000009 ||
        (1-fabs(amount-(int)amount)) <=0.0010000000000000009  ){
            return true;
        }else return false;


    }
    int amountt =(int)amount;
    if(amountType==MATAMAZOM_HALF_INTEGER_AMOUNT){
        if((fabs(amount-amountt)<=0.0010000000000000009)||
        ((fabs(1-fabs(amount+0.5-amountt)))<=0.0010000000000000009) ||
        (1-fabs(amount-amountt) <=0.0010000000000000009)){
            return true;
        } else return false;

    }
    if(amountType==MATAMAZOM_ANY_AMOUNT){
        return true;
    }
    return false;


}

bool NameCheck(const char* name){
    if(!(*name)){
        return false;
    }

    if(!((*name)<='9' && (*name)>='0') && !((*name)>='a' &&(*name)<'z' ) && !((*name)>='A' &&(*name)<='Z' )){
        return false;
    }
    return true;
}




static bool ContainsOrder(Matamazom matamazom,  unsigned int orderId){
    if(!matamazom)
        return false;


    SET_FOREACH(Order,iterator,matamazom->orders){
        if(GetOrderId(iterator)==orderId)
            return true;
    }
    return false;
}
