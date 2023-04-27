//
// Created by slima on 1/13/2020.
//

#ifndef HW33_VEHICLECLASS_H
#define HW33_VEHICLECLASS_H
#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"

using namespace ParkingLotUtils;

namespace MtmParkingLot {
    class VehicleClass {

        VehicleType vehicle;
        LicensePlate license;
        Time parking_time;
        bool parking_fine;

    public:
        VehicleClass(VehicleType new_vehicle, LicensePlate new_license, Time new_parking_time,bool fine) :
        vehicle(new_vehicle),license(new_license), parking_time(new_parking_time),parking_fine(fine) {

        }
         LicensePlate getLicensePlate(){
             return  license;
         }

         Time getTime(){
             return parking_time;
         }

         void fineSet(){
             parking_fine= true;
         }

         VehicleType getVehicleType(){
             return vehicle;
         }

         bool getFine(){
             return parking_fine;
         }

        ~VehicleClass() = default;


    };

    class Compare_license {
    public:
        bool operator()(VehicleClass vehicle_a,VehicleClass vehicle_b){
            {

                LicensePlate license_a= vehicle_a.getLicensePlate();
                LicensePlate license_b= vehicle_b.getLicensePlate();

                return license_a == license_b;
            }
        }
    };

}




















#endif //HW33_VEHICLECLASS_H
