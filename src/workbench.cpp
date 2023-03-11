//
// Created by Centauria V. CHEN on 2023/3/11.
//

#include "workbench.h"

WorkBench::WorkBench()
{
}
WorkBench::WorkBench(uint8_t type, double x, double y)
{
}
double WorkBench::ETA()
{
    return 0;
}
bool WorkBench::is_free(int index)
{
    return false;
}


void WrokBench::setType(int type){
    this->type = type;
}

int WrokBench::getType(){
    return type;
}


void WrokBench::setCoordinate(Vector2D coordinate){
    double x = coordinate.get_x();
    double y = coordinate.get_y();
    this->coordinate.set_x(x);
    this->coordinate.set_y(y); 
}


Vector2D WrokBench::getCoordinate(){
    return coordinate;
}


void WrokBench::setMaterials(int materials){
    this->materials = materials;
}

int WrokBench::getMaterials(){
    return materials;
}
void WrokBench::setProduct_status(bool product_status){
    this->product_status = product_status;
}

bool WrokBench::getProduct_status(){
    return product_status;
}