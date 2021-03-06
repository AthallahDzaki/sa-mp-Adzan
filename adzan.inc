Float:degToRad(Float:degree)
{
    return ((3.1415926 / 180) * degree);
}
 
//convert Radian to Degree
Float:radToDeg(Float:radian)
{
    return (radian * (180/3.1415926));
}
 
//make sure a value is between 0 and 360
Float:moreLess360(Float:value)
{
    while(value > 360 || value < 0)
    {
        if(value > 360)
            value -= 360;
 
        else if (value <0)
            value += 360;
    }
 
    return value;
}
 
//make sure a value is between 0 and 24
Float:moreLess24(Float:value)
{
    while(value > 24 || value < 0)
    {
        if(value > 24)
            value -= 24;
 
        else if (value <0)
            value += 24;
    }
 
    return value;
}
 
//convert the Float:number to Hours and Minutes
stock floatToHrMin(Float: number, hours, minutes)
{
    hours = floatround(moreLess24(number), floatround_round);
    minutes = floatround(moreLess24(number - hours) * 60, floatround_round);
}

new Float:fajrTwilight, Float:ishaTwilight,
                     Float:fajrTime, Float:sunRiseTime, Float:zuhrTime,
                     Float:asrTime, Float:maghribTime, Float:ishaTime;
 
stock calcPrayerTimes(year, month, day,
                     Float:longitude, Float:latitude, timeZone)
{
    new Float:D = (367 * year) - ((year + (int)((month + 9) / 12)) * 7 / 4) + (((int)(275 * month / 9)) + day - 730531.5);
 
    new Float:L = 280.461 + 0.9856474 * D;
    L = moreLess360(L);
 
    new Float:M = 357.528 + (0.9856003) * D;
    M = moreLess360(M);
 
    new Float:Lambda = L + 1.915 * asin(degToRad(M)) + 0.02 * aasin(degToRad(2 * M));
    Lambda = moreLess360(Lambda);
 
    new Float:Obliquity = 23.439 - 0.0000004 * D;
    new Float:Alpha = radToDeg(atan((acos(degToRad(Obliquity)) * atan(degToRad(Lambda)))));
    Alpha = moreLess360(Alpha);
 
    Alpha = Alpha - (360 * (int)(Alpha / 360));
    Alpha = Alpha + 90 * (floatround(Lambda / 90, floatround_round) - floatround(Alpha / 90, floatround_round));
 
    new Float:ST = 100.46 + 0.985647352 * D;
    new Float:Dec = radToDeg(aasin(asin(degToRad(Obliquity)) * asin(degToRad(Lambda))));
    new Float:Durinal_Arc = radToDeg(acos((asin(degToRad(-0.8333)) - asin(degToRad(Dec)) * asin(degToRad(latitude))) / (acos(degToRad(Dec)) * acos(degToRad(latitude)))));
 
    new Float:Noon = Alpha - ST;
    Noon = moreLess360(Noon);
 
 
    new Float:UT_Noon = Noon - longitude;
 
 
    ////////////////////////////////////////////
    // Calculating Prayer Times Arcs & Times //
    //////////////////////////////////////////
 
    // 2) Zuhr Time [Local noon]
    zuhrTime = UT_Noon / 15 + timeZone;
 
    // Asr Hanafi
    //Float:Asr_Alt =radToDeg(atan(2 + tan(degToRad(abs(latitude - Dec)))));
 
    // Asr Shafii
    new Float:Asr_Alt = radToDeg(atan(1 + tan(degToRad(abs(latitude - Dec)))));
    new Float:Asr_Arc = radToDeg(acos((asin(degToRad(90 - Asr_Alt)) - asin(degToRad(Dec)) * asin(degToRad(latitude))) / (acos(degToRad(Dec)) * acos(degToRad(latitude)))));
    Asr_Arc = Asr_Arc / 15;
    // 3) Asr Time
    asrTime = zuhrTime + Asr_Arc;
 
    // 1) Shorouq Time
    sunRiseTime = zuhrTime - (Durinal_Arc / 15);
 
    // 4) Maghrib Time
    maghribTime = zuhrTime + (Durinal_Arc / 15);
 
 
    Float:Esha_Arc = radToDeg(acos((asin(degToRad(ishaTwilight)) - asin(degToRad(Dec)) * asin(degToRad(latitude))) / (acos(degToRad(Dec)) * acos(degToRad(latitude)))));
    // 5) Isha Time
    ishaTime = zuhrTime + (Esha_Arc / 15);
 
    // 0) Fajr Time
    Float:Fajr_Arc = radToDeg(acos((asin(degToRad(fajrTwilight)) - asin(degToRad(Dec)) * asin(degToRad(latitude))) / (acos(degToRad(Dec)) * acos(degToRad(latitude)))));
    fajrTime = zuhrTime - (Fajr_Arc / 15);
 
    return;
}