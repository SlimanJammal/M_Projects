#include <stdio.h>
#include "orders.h"
#include "products.h"
#include <assert.h>
struct order{
    unsigned int orderId;
    AmountSet products;



};




Order AddProductInOrder(AmountSet products,Set orders,unsigned int orderId,unsigned int productId, double amount){

    if(!products){
        return NULL;
    }

    Order find_order=GetOrderByIdd(orders,orderId);
    assert(find_order!=NULL);

    Product find_product=getProductById(products,productId);

    asRegister(find_order->products,find_product);
    asChangeAmount(find_order->products,find_product,amount);///////////////////////////////////////////////////////////

    return NULL; //TODO

}







unsigned int GetOrderId(Order order){
    return order ->orderId;
}






Order createOrder(unsigned int newId){

    Order new_order = malloc(sizeof(*new_order));
    if(!new_order){
        return NULL;
    }
    new_order->orderId=newId;
    new_order->products=asCreate(copyProduct,freeProduct,compareProduct);///////////////////////////////////////////////////////////
    return new_order;
}


bool ContainsProductInOrder(Set orders,unsigned int orderId, unsigned int productId){//////////////////////////////////////////////////

    Order get_order=GetOrderByIdd(orders,orderId);


    AS_FOREACH(Product ,iterator,get_order->products){
        if(GetProductId(iterator)==productId)
            return true;
    }

    return false;



}





AmountSet getOrderSet(Set orders, unsigned int orderId){///////////////////////////////////////////////////////////////


    Order gett_Order=GetOrderByIdd(orders,orderId);//TODO
    if(!gett_Order)
    {
        return NULL;
    }
    return gett_Order->products;


}



double getProductAmountInOrder (AmountSet products,Set orders,unsigned int orderId,unsigned int productId){


    double amount_to_get=0;
    AmountSet set_to_get=getOrderSet(orders,orderId);

    asGetAmount(set_to_get,getProductById(products,productId),&amount_to_get);
    return amount_to_get;



}


bool freeOrder(Order orderr){

    if(!orderr){
        return false;
    }
    asDestroy(orderr->products);
    return true;
}

int compare_orders(void *order11, void *order22) {
    Order order1 = (Order) order11;
    Order order2 = (Order) order22;
    unsigned int id1 = order1->orderId;
    unsigned int id2 = order2->orderId;
    if ((id1 - id2) > 0) {
        return 1;
    }
    if ((id1 - id2) < 0) {
        return -1;
    }
    if (id1 == id2) {
        return 0;
    }
    return 0;
}

void* copy_order(void* order11) {
    Order order1=(Order) order11;
    Order new_order = malloc(sizeof(*new_order));
    if (!new_order) {
        return NULL;
    }
    new_order->orderId = order1->orderId;
    new_order->products = asCopy(order1->products);

    return (void*)new_order;
}

void free_orders(void* order11) {
    Order order1=(Order)order11;
    if (!order1) {
        return;
    }
    asDestroy(order1->products);
    free(order1);
}






/*static GetProductById(Matamazom matamazom,unsigned int id){
    if(!matamazom )
        return NULL;
    Product iteratortemp2=NULL ;

SET_FOREACH(Product, iteratortemp2,matamazom->product){
    if(get_product_id(iteratortemp2)==id){
        return iteratortemp2;
    }
    return NULL;
}



}*/

Order GetOrderByIdd(Set orders,  unsigned int orderId){
    if(!orders)
        return NULL;



    SET_FOREACH(Order,iterator,orders){
        if(iterator->orderId==orderId)
            return iterator;
    }
    return NULL;
}


void deleteProductFromOrder(Set orders, unsigned int order_id,unsigned int prod_id){

    if(!orders){
        return;
    }
    AmountSet set=getOrderSet(orders,order_id);
    if(asDelete(set,getProductById(set,prod_id)) != AS_SUCCESS){
        return;
    }


}


bool changeAmountInOrder(Set orders,unsigned int orderId, unsigned int productId, double amount){

    SET_FOREACH(Order ,iterator,orders){
        if(iterator->orderId==orderId){
            AS_FOREACH(Product,iterator2,iterator->products){
                if(productId==GetProductId(iterator2)){

                    asChangeAmount(getOrderSet(orders,orderId),iterator2,amount);
                    return true;
                }
            }

        }



    }
    return false;

}
