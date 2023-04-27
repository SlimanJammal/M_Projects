#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "UniqueArray.h"
#include "VehicleClass.h"

namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;
    using namespace MtmParkingLot;
    
    class ParkingLot {
    UniqueArray<VehicleClass, Compare_license> motors_array;
    UniqueArray<VehicleClass, Compare_license> cars_array;
    UniqueArray<VehicleClass, Compare_license> handicap_cars_array;

    public:

        explicit ParkingLot(unsigned int parkingBlockSizes[]); // TODO EXPLICIT CHECK
        ~ParkingLot()= default;
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

    };
    ostream& operator<<(ostream& os, const ParkingLot& parkingLot);
}

unsigned int getPrice(Time exit_time,Time entrance_time,VehicleType vehicle_type,bool fine);






#endif //MTMPARKINGLOT_PARKINGLOT_H
