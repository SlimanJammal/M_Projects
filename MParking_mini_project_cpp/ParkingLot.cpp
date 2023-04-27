//
// Created by sliman on 1/13/2020.
#include "ParkingLot.h"
#include "VehicleClass.h"
#include "UniqueArray.h"
#include "ParkingLotPrinter.h"

#include <iostream>
using namespace MtmParkingLot;
using MtmParkingLot::ParkingLot;
using namespace ParkingLotUtils;
static unsigned int calculatePrice(VehicleType type, unsigned int time);

 ParkingLot :: ParkingLot(unsigned int parkingBlockSizes[]) :motors_array(parkingBlockSizes[0]),
 cars_array(parkingBlockSizes[2]),handicap_cars_array(parkingBlockSizes[1]){


}





ParkingResult ParkingLot :: enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime) {
     VehicleClass temp_vehicle(vehicleType,licensePlate,entranceTime, false);

    unsigned int get_index;

    ParkingSpot parking_spot;
    if((cars_array.getIndex(temp_vehicle,get_index) || handicap_cars_array.getIndex(temp_vehicle,get_index)
         || motors_array.getIndex(temp_vehicle,get_index))){

        if(cars_array.operator[](temp_vehicle)){
            temp_vehicle = *(cars_array.operator[](temp_vehicle));
        } else if(handicap_cars_array.operator[](temp_vehicle)){
            temp_vehicle= *(handicap_cars_array.operator[](temp_vehicle));
        } else if(motors_array.operator[](temp_vehicle)){
            temp_vehicle= *(motors_array.operator[](temp_vehicle));
        }
        ParkingLotPrinter::printVehicle(std::cout, temp_vehicle.getVehicleType(), temp_vehicle.getLicensePlate(),
                                        temp_vehicle.getTime());

        getParkingSpot(licensePlate,parking_spot);
        ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout,parking_spot);
        return VEHICLE_ALREADY_PARKED;
    }

    bool handicap_is_full=false;
    if(handicap_cars_array.getCount() == handicap_cars_array.getSize()){
        handicap_is_full = true;
    }

    UniqueArray<VehicleClass, Compare_license> *temp_array;
    if((vehicleType == HANDICAPPED && handicap_is_full) || vehicleType == CAR ){
        temp_array = &cars_array;
    } else if(vehicleType == HANDICAPPED){
        temp_array = &handicap_cars_array;
    } else {
        temp_array= &motors_array;
    }

    VehicleClass new_vehicle(vehicleType,licensePlate,entranceTime, false);
  try{
      temp_array->insert(new_vehicle);
  } catch(...){
      ParkingLotPrinter::printVehicle(std::cout,vehicleType,licensePlate,entranceTime);
      ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
      return NO_EMPTY_SPOT;
  }

    ParkingLotPrinter::printVehicle(std::cout,vehicleType,licensePlate,entranceTime);
    getParkingSpot(licensePlate,parking_spot);
    ParkingLotPrinter::printEntrySuccess(std::cout,parking_spot);
    return SUCCESS;


}

ParkingResult ParkingLot ::   exitParking(LicensePlate licensePlate, Time exitTime){
     VehicleClass temp_vehicle(CAR,licensePlate,exitTime, false);

  unsigned int index=0;
  ParkingSpot parking_spot;


    if(!(cars_array.getIndex(temp_vehicle,index) || handicap_cars_array.getIndex(temp_vehicle,index)
    || motors_array.getIndex(temp_vehicle,index))){
      ParkingLotPrinter::printExitFailure(std::cout,licensePlate);
        return VEHICLE_NOT_FOUND;
  }


    if(cars_array.operator[](temp_vehicle)){
        temp_vehicle = *(cars_array.operator[](temp_vehicle));
    } else if(handicap_cars_array.operator[](temp_vehicle)){
        temp_vehicle= *(handicap_cars_array.operator[](temp_vehicle));
    } else if(motors_array.operator[](temp_vehicle)){
        temp_vehicle= *(motors_array.operator[](temp_vehicle));
    }


      ParkingLotPrinter::printVehicle(std::cout,temp_vehicle.getVehicleType(),licensePlate,temp_vehicle.getTime());

      getParkingSpot(licensePlate,parking_spot);

      ParkingLotPrinter::printExitSuccess(std::cout,parking_spot,exitTime,
              getPrice(exitTime,temp_vehicle.getTime(),temp_vehicle.getVehicleType(),temp_vehicle.getFine()));
      motors_array.remove(temp_vehicle);
      cars_array.remove(temp_vehicle);
      handicap_cars_array.remove(temp_vehicle);
    return SUCCESS;
}


ParkingResult ParkingLot ::  getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const{
    Time trash=0;
    VehicleClass vehicle_check_two(MOTORBIKE,licensePlate,trash, false);
   unsigned int index=0;
    using namespace ParkingLotUtils;
   if(cars_array.getIndex(vehicle_check_two,index)) {
       ParkingSpot new_spot(CAR,index);
       parkingSpot=new_spot;

        return SUCCESS;

    } else if (motors_array.getIndex(vehicle_check_two,index)){
       ParkingSpot new_spot(MOTORBIKE,index);
       parkingSpot=new_spot;

        return SUCCESS;

    } else if (handicap_cars_array.getIndex(vehicle_check_two,index)){
       ParkingSpot new_spot(HANDICAPPED,index);
       parkingSpot=new_spot;
        return SUCCESS;

    }else {
        return VEHICLE_NOT_FOUND;
    }


 }


void ParkingLot ::  inspectParkingLot(Time inspectionTime){
     Time timothy;

unsigned int timothy_int=0;
unsigned int blocks_array[3]={0};
blocks_array[0]= cars_array.getSize();
blocks_array[1]=motors_array.getSize();
blocks_array[2]=handicap_cars_array.getSize();
unsigned int fined_counter=0;
VehicleClass *temp_vehicle=nullptr;
for( int i=0 ; i<3 ;i++) {
    for (unsigned int n = 0; n <= blocks_array[i]; n++) {
        if(i==0){
          temp_vehicle=  cars_array.getElementById(n);
        }
        if(i==1){
           temp_vehicle= motors_array.getElementById(n);
        }
        if(i==2){
            temp_vehicle= handicap_cars_array.getElementById(n);
        }
        if(temp_vehicle == NULL) continue;
           timothy=(inspectionTime)-(temp_vehicle->getTime());
           timothy_int=timothy.toHours();
           if (timothy_int>24 && !temp_vehicle->getFine()) {
               temp_vehicle->fineSet();
               fined_counter++;
           }

    }

}

ParkingLotPrinter::printInspectionResult(std::cout,inspectionTime,fined_counter);


 }


//static bool sort_arrays(VehicleClass& print_vehicle,const VehicleClass& temp,
//        unsigned int sizes_array);

ostream& MtmParkingLot::operator<<(ostream& os, const ParkingLot& parkingLot) {
    UniqueArray<VehicleClass, Compare_license> arrays[3] = {parkingLot.motors_array, parkingLot.cars_array,
                                                            parkingLot.handicap_cars_array};
    ParkingLotPrinter ::printParkingLotTitle(std::cout);
    unsigned int size_array[3] = {0};

    size_array[0] = parkingLot.motors_array.getSize();
    size_array[1] = parkingLot.cars_array.getSize();
    size_array[2] = parkingLot.handicap_cars_array.getSize();
unsigned int all_sizes = parkingLot.motors_array.getCount() + parkingLot.handicap_cars_array.getCount()
        + parkingLot.cars_array.getCount();
    ParkingSpot temp_spot;
    ParkingSpot check_spot;
    ParkingSpot print_spot;

    VehicleClass *temp_vehicle = nullptr;
    VehicleClass *print_vehicle = nullptr;
    VehicleClass *min_vehicle = nullptr;
    int counter =0;
        for (int i = 0; i < 3; i++) {

            for (unsigned int j = 0; j < size_array[i]; j++) {
                if (!arrays[i].getElementById(j)) {
                    continue;
                }
                if(counter == 0 && temp_vehicle == nullptr){
                    temp_vehicle = arrays[i].getElementById(j);
                    counter++;
                    continue;
                }

                parkingLot.getParkingSpot(arrays[i].getElementById(j)->getLicensePlate(),check_spot);
                parkingLot.getParkingSpot(temp_vehicle->getLicensePlate(),temp_spot);
                if ( check_spot < temp_spot){
                    temp_vehicle = arrays[i].getElementById(j);
                }
            }

        }

int counter_two = 0;
    for (int i = 0; i < 3; i++) {

        for (unsigned int j = 0; j < size_array[i]; j++) {
            if (!arrays[i].getElementById(j)) {
                continue;
            }
            if(counter_two == 0 && min_vehicle == nullptr){
                min_vehicle = arrays[i].getElementById(j);
                counter_two++;
                continue;
            }

            parkingLot.getParkingSpot(arrays[i].getElementById(j)->getLicensePlate(),check_spot);
            parkingLot.getParkingSpot(min_vehicle->getLicensePlate(),print_spot);
            if (print_spot < check_spot){
                min_vehicle = arrays[i].getElementById(j);
            }
        }

    }

for(unsigned int n=0;n < all_sizes;n++){
    print_vehicle = min_vehicle;
    if( n == 0){
        ParkingLotPrinter::printVehicle(std::cout, temp_vehicle->getVehicleType(),
                                        temp_vehicle->getLicensePlate(),
                                        temp_vehicle->getTime());
        parkingLot.getParkingSpot(temp_vehicle->getLicensePlate(), print_spot);
        ParkingLotPrinter::printParkingSpot(std::cout, print_spot);
        continue;
    }
    for (int i = 0; i < 3; i++) {

        for (unsigned int j = 0; j < size_array[i]; j++) {
            if (!arrays[i].getElementById(j)) {
                continue;
            }


            parkingLot.getParkingSpot(arrays[i].getElementById(j)->getLicensePlate(),check_spot);
            parkingLot.getParkingSpot(temp_vehicle->getLicensePlate(),temp_spot);
            parkingLot.getParkingSpot(print_vehicle->getLicensePlate(),print_spot);
            if (temp_spot < check_spot && check_spot < print_spot ) {

                print_vehicle = arrays[i].getElementById(j);
            }


        }

    }

    ParkingLotPrinter::printVehicle(std::cout, print_vehicle->getVehicleType(),
                                    print_vehicle->getLicensePlate(),
                                    print_vehicle->getTime());
    parkingLot.getParkingSpot(print_vehicle->getLicensePlate(), print_spot);
    ParkingLotPrinter::printParkingSpot(std::cout, print_spot);

    temp_vehicle = print_vehicle;
}
    return os;
 }

unsigned int getPrice(Time exit_time,Time entrance_time,VehicleType vehicle_type,bool fine){

    Time time_check =(exit_time - entrance_time);
    unsigned int time_check_int=time_check.toHours();
    unsigned int initial_price=0;
    switch (vehicle_type){
        case CAR:initial_price= calculatePrice(CAR,time_check_int);
            break;
        case MOTORBIKE:initial_price= calculatePrice(MOTORBIKE,time_check_int);
            break;
        case HANDICAPPED:if (time_check_int == 0){
            initial_price = 0 ;
        } else  initial_price= 15;
        break;
        default:
            return 0; //todo check

    }


    if(fine){
        return initial_price+250;
    } else return initial_price;



}

//
static unsigned int calculatePrice(VehicleType type, unsigned int time){
unsigned int price=0;

     if(time < 6){
         price = (time-1)*5 +10;

      if(type == CAR){
          price = price*2;
      }

} else if (type==CAR) {

         price =70;

     } else price=35;

     if(time == 0){
         price=0;
     }
    return price;

}



