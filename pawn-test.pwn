#include <a_samp> // bisa openmp
#include <izcmd> // Zeex
#include <EVF2> // bisa make yang bawaan github authornya (Abyss Morgan)
#include <samp-locktyre>

static fanstr[500];
#define SCM(%0,%1,%2) \
                        format(fanstr, sizeof(fanstr), ""%2); \
                        SendClientMessage(%0, %1, fanstr)

#define SendCustomMessage(%0, %1, %2) \
                                        SCM(%0, 0xC6E2FFFF, %1": {FFFFFF}"%2)

#define SendErrorMessage(%0, %1) \ 
                                    SCM(%0, 0xC8C8C8FF, "ERROR: "%1)

// command
CMD:checkfann(playerid, params[]) 
{
	new vehicleid, partid, Float:range;
	if(sscanf(params, "ddf", vehicleid, partid, range)) 
		return SendSyntaxMessage(playerid, "/checkfann [vehicleid] [partid] [range]");

	if(!IsValidVehicle(vehicleid)) 
		return SendErrorMessage(playerid, "There is no vehicle with that id!");

	if(partid < 1 || partid > 9)
		return SendErrorMessage(playerid, "There is only 1 - 9 part of vehicle!");

	SendCustomMessage(playerid, "CHECK", "You are %s of that part of vehicle!", IsPlayerInRangeOfVehiclePart(playerid, vehicleid, partid, range) ? "{00ff00}Near{ffffff}" : "{ff0000}Not Near{ffffff}");
	return 1;
}

CMD:locktyre(playerid, params[])
{
	if(IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "You can't use this when in vehicle!");

	new vehicleid = GetNearestVehicleToPlayer(playerid, 4.5, false);
	if(vehicleid == INVALID_VEHICLE_ID)
		return SendErrorMessage(playerid, "Kamu tidak berada didekat Kendaraan apapun.");

	new tyre;
	if((tyre = LockTyre_Near(playerid, vehicleid)) == -1)
		return SendErrorMessage(playerid, "You are not near to the tire of vehicle!");

	ApplyAnimation(playerid,"BOMBER","BOM_Plant",4.0,false,false,false,false,0,SYNC_ALL);
	if(TyreData[vehicleid][tyre][tValid])
	{
		LockTyre_Deattach(vehicleid, tyre); 
		SendCustomMessage(playerid, "INFO", "You have deattached lock tire ffrom this vehicle tire.", ReturnName(playerid));
		if(LockTyre_Locked(vehicleid, 0)) SetVehicleSpeedCap(vehicleid, 0); // vehicle can move normally after vehicle was deattached from any lock tire
	}
	else
	{
		SetVehicleSpeedCap(vehicleid, 1); // logic to stuck the vehicle
		LockTyre_Attach(vehicleid, tyre); 
		SendCustomMessage(playerid, "INFO", "You have attached lock tire to this vehicle tire.", ReturnName(playerid));
	}
	return 1;
}