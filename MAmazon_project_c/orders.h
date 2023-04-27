//
// Created by slima on 12/6/2019.
//

#ifndef HW1_ORDERS_H
#define HW1_ORDERS_H
#include <stdbool.h>
#include <stdlib.h>
#include "amount_set.h"
#include "matamazom.h"
#include "set.h"
typedef struct order *Order;

Order AddProductInOrder(AmountSet products,Set orders,unsigned int orderId,unsigned int productId, double amount);
unsigned int GetOrderId(Order order);


bool addOrdertoWarehouse(Matamazom matamazom);


Order createOrder(unsigned int newId);

bool ContainsProductInOrder(Set orders,unsigned int orderId, unsigned int productId);



AmountSet getOrderSet(Set orders, unsigned int orderId);

double getProductAmountInOrder(AmountSet products,Set orders,unsigned int orderId,unsigned int productId);
bool freeOrder(Order orderr);

int compare_orders(void *order11, void *order22);
void* copy_order(void* order11);
void free_orders(void* order11);
Order GetOrderByIdd(Set orders,  unsigned int orderId);
void deleteProductFromOrder(Set orders, unsigned int order_id,unsigned int prod_id);
bool changeAmountInOrder(Set orders,unsigned int orderId,unsigned int productId, double amount);


#endif //HW1_ORDERS_H
