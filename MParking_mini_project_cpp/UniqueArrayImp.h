//
// Created by slima on 1/7/2020.
//

#ifndef MTAM_234124_UNIQUEARRAYIMP_H
#define MTAM_234124_UNIQUEARRAYIMP_H

#include <iostream>


template <class Element,class Compare>
UniqueArray<Element, Compare> :: UniqueArray(const unsigned int size) : data(new Element*[size]) ,array_size(size){
for(int i=0; i<size;i++){
    data[i]=NULL;
}
}

template <class Element,class Compare>
UniqueArray<Element, Compare> :: UniqueArray(const UniqueArray& other) : data(new Element*[other.array_size]) ,
 array_size(other.array_size){
for (int i = 0; i<array_size; i++){
    if(!other.data[i]){
        data[i]=NULL;
        continue;
    }
    data[i]= new Element(*(other.data[i]));
}
}

template <class Element,class Compare>
UniqueArray<Element, Compare> :: ~UniqueArray(){
    for(int j=0;j<array_size;j++){
        delete data[j];
    }
    delete [] data; ////af7s
}


template <class Element,class Compare>
unsigned int UniqueArray<Element, Compare> :: insert(const Element& element){

unsigned int index=0;

if(getIndex(element,index)){
    return index;
} else {
    for(int i=0; i<array_size;i++){
        if(data[i]==NULL){
            Element* ptr = new Element(element);//TODO
            data[i]= ptr;
            return i;
        }
    }
}
    throw UniqueArrayIsFullException();
}

template <class Element,class Compare>
bool UniqueArray<Element, Compare> :: getIndex(const Element& element, unsigned int& index) const{
    Compare CompareFunction;

    for (int i = 0; i <array_size ; i++) {
        if(data[i]==NULL){
            continue;
        }
        if (CompareFunction(element,*(data[i]))){
            index=i;
            return true;
        }
    }
    return false;
}

template <class Element,class Compare>
const Element* UniqueArray<Element, Compare> :: operator[] (const Element& element) const{
    unsigned int index = 0;
    if (getIndex(element,index)){
        return data[index];
    }
    return NULL;
}


template <class Element,class Compare>
bool UniqueArray<Element, Compare> ::remove(const Element& element){
    unsigned int index=0;
    if(getIndex(element,index)){
        Element* tmp=data[index];
        data[index]=NULL;
        delete tmp;

        return true;
    }
    return false;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare> :: getCount() const{
    unsigned int counter=0;
    for (int i=0; i<array_size; i++){
        if(data[i]!= NULL){
            counter++;
        }
    }
    return counter;
}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare> :: getSize() const{
    return array_size;
}

template <class Element, class Compare>
UniqueArray<Element, Compare> UniqueArray<Element, Compare> :: filter(const Filter& f) const{
    UniqueArray new_array(*this);
//    UniqueArray new_array(*this); TODO
    for(int i=0;i<array_size;i++){
        if(!f.operator()(*(data[i]))){
            delete new_array.data[i];
            new_array.data[i]=NULL;
        }
    }
    return new_array;
}


template <class Element, class Compare>
Element* UniqueArray<Element, Compare> ::getElementById(unsigned int id) const  {

    if(id >= this->array_size){
        return NULL;
    }


    return data[id];

}






#endif //MTAM_234124_UNIQUEARRAYIMP_H
