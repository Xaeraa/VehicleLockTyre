// Lock Vehicle Tyre 
// by Fann

// gaktau, kepikiran buat script ini pas lagi berak njier
// tolong jangan ubah creadit ya ganteng (kata ibu-nya)/cantik.

#if !defined TRYG3D_ENABLE_VEHICLE
    #define TRYG3D_ENABLE_VEHICLE
#endif

#tryinclude <streamer.inc> // incognito
#tryinclude <3DTryg.inc>   // Abyss Morgan

#if !defined _streamer_included
    #error Required streamer.inc v2.9 by incognito (https://github.com/samp-incognito/samp-streamer-plugin/releases/tag/v2.9.6)
#endif

#if ((!defined TRYG3D_REMASTERED_INCLUDE) || (TRYG3D_VERSION != 10002))
    #error required 3DTryg.inc Gen. 2 by Abyss Morgan (https://github.com/r4sheed/3DTryg/releases/tag/v1.0.2)
#endif 

enum e_locktyre_data
{
    tObject,
    bool:tValid
};

new
    TyreData[MAX_VEHICLES][4][e_locktyre_data]
;

stock bool:IsPlayerInRangeOfVehiclePart(playerid, vehicleid, partid, Float:range = 1.0) 
{
	new Float:vx, Float:vy, Float:vz;
	Tryg3D::GetVehiclePartPos(vehicleid, partid, vx, vy, vz);
	if(!IsPlayerInRangeOfPoint(playerid, range, vx, vy, vz))
		return false; 

	return true;
}

stock bool:LockTyre_Locked(vehicleid, must = 1) 
{
    new count; 
    for(new fan; fan < 4; fan++) if(TyreData[vehicleid][fan][tValid])
        count++;

    if(count < must)
        return false; 
    
    return true;
}

stock LockTyre_Near(playerid, vehicleid, Float:range = 1.0) // near of tire
{
    for(new fan; fan < 4; fan++) if(IsPlayerInRangeOfVehiclePart(playerid, vehicleid, fan+1, range))
        return fan;

    return -1;
}

stock bool:LockTyre_Attach(vehicleid, tyre)
{
    if(!TyreData[vehicleid][tyre][tValid])
    {
        if(IsValidDynamicObject(TyreData[vehicleid][tyre][tObject]))
            DestroyDynamicObject(TyreData[vehicleid][tyre][tObject]);

        new Float:tyrepos[3], Float:vehpos[4];
        GetVehiclePos(vehicleid, vehpos[0], vehpos[1], vehpos[2]);
        GetVehicleZAngle(vehicleid, vehpos[3]);
        Tryg3D::GetVehiclePartPos(vehicleid, tyre+1, tyrepos[0], tyrepos[1], tyrepos[2]);

        new Float:attachtyre[6];
        tyrepos[0] += 0.25 * floatsin(-vehpos[3], degrees);
        tyrepos[1] += 0.25 * floatcos(-vehpos[3], degrees);

        attachtyre[0] = ((tyrepos[0] - vehpos[0]) * floatcos(vehpos[3], degrees)) + ((tyrepos[1] - vehpos[1]) * floatsin(vehpos[3], degrees));
        attachtyre[1] = ((tyrepos[1] - vehpos[1]) * floatcos(vehpos[3], degrees)) - ((tyrepos[0] - vehpos[0]) * floatsin(vehpos[3], degrees));
        attachtyre[2] = (tyrepos[2] - vehpos[2]) - 0.2;
        attachtyre[3] = 0.0;
        attachtyre[4] = -36.0;
        attachtyre[5] = (-269.001 - vehpos[3]);

        TyreData[vehicleid][tyre][tValid] = true;
        TyreData[vehicleid][tyre][tObject] = CreateDynamicObject(19094, vehpos[0], vehpos[1], vehpos[2], 0.0, 0.0, 0.0);
        for(new fan; fan < 5; fan++) SetDynamicObjectMaterial(TyreData[vehicleid][tyre][tObject], fan, 19094, "none", "none", RGBAToARGB(COLOR_YELLOW));

        AttachDynamicObjectToVehicle(TyreData[vehicleid][tyre][tObject], vehicleid, attachtyre[0], attachtyre[1], attachtyre[2], attachtyre[3], attachtyre[4], attachtyre[5]);
    }
    return true;
}

stock bool:LockTyre_Deattach(vehicleid, tyre) 
{
    if(IsValidDynamicObject(TyreData[vehicleid][tyre][tObject]))
        DestroyDynamicObject(TyreData[vehicleid][tyre][tObject]);

    TyreData[vehicleid][tyre][tValid] = false;
    return true;
}