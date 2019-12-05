void setup() {
  Serial.begin(115200);
}

void loop() {
 // Serial.println("nilai YTDcount = "+String(YTDcount));
 // Serial.println("nilai TahunBerlalu = "+String(TahunBerlalu));
  int TimeNow=RTCnow();
  int TimeStop=RTCnext(TimeNow);
  Serial.println("TimeNow = "+String(TimeNow)+", TimeStop = "+String(TimeStop));
  delay(5000);
}

//---------------BEGINNING OF TIMIMG SECTION ------------------------------//
int RTCnow()
{
  int hari=5;
  int bulan=12;
  int tahun=2019;
  
  int YTDcount = DayYTD(hari,bulan,tahun);
  int TahunBerlalu= DayLewat(tahun);
  int YTDtotal = YTDcount+TahunBerlalu;
  return YTDtotal;
}

int DayYTD(int hr,int bln,int th)
{
  boolean kab= isKabisat(th);
  int febDay;
  if(kab)
    febDay=29;
  else
    febDay=28;
  //-----------------------------//
  if(bln==1)
    return hr;
  else if(bln==2)
    return 31+hr;
  else if(bln==3)
    return 31+febDay+hr;
  else if(bln==4)
    return 31+febDay+31+hr;
  else if(bln==5)
    return 31+febDay+31+30+hr;
  else if(bln==6)
    return 31+febDay+31+30+31+hr;
  else if(bln==7)
    return 31+febDay+31+30+31+30+hr;
  else if(bln==8)
    return 31+febDay+31+30+31+30+31+hr;
  else if(bln==9)
    return 31+febDay+31+30+31+30+31+31+hr;
  else if(bln==10)
    return 31+febDay+31+30+31+30+31+31+30+hr;
  else if(bln==11)
    return 31+febDay+31+30+31+30+31+31+30+31+hr;
  else if(bln==12)
    return 31+febDay+31+30+31+30+31+31+30+31+30+hr;
}

int DayLewat(int th)
{
  int count = 100000;
  for (int i=2019;i<th;i++)
  {
    if(isKabisat(i))
      count+=366;
    else
      count+=365;
  }
  return count;
}

boolean isKabisat(int th)
{
  if(th%4==0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int RTCnext(int rtcnow)
{
  int x=100335; // Nilai awal
  while(x<=rtcnow)
  {
    x+=7;
  }
  return x;
}

//---------------END OF TIMING SECTION-----------//
