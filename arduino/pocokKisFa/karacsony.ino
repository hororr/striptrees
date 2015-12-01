int i;
int nLed = 9; //ledek száma
int leds[] =   {2,3,4,5,6,7,8,9,10}; //ezek a lábak vannak bekötve a ledek pozitívjára
int active[] = {0,0,0,0,0,0,0,0,0}; //alapbol semmi sem vilagit

void setup() {
  //initelni kell minden pint különben nem kapnak áramot a lábak
  for(i = 0; i < nLed; i++){
    pinMode(leds[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // Serial feldolgozás
  if(Serial.available() > 0){
    char c = Serial.read();
    if(c == 'x'){ //kikapcsolja mindet
      for(i = 0; i < nLed; i++){
        active[i] = 0;
      }
    } else{ 
      int a = c - '0'; //konvertálja a karaktert számmá (SZOVAL CSAK EGYJEGYÜT!!)
      if(a >= 0 && a <= nLed - 1){
        active[a] = active[a] ? 0 : 1; //be/kikapcsolja
      }
    }
  }
  // Led-ek müködtetése
  // Az arduino nem bír el 9 led-et saját tápról mert megsül.
  // Egy kokány hack erre, hogyha sorba mész körbe a ledeken és
  //   mindet csak 1ms-re villantod fel. Tehát egyszerre csak 1
  //   ég.
  //   Vagyis a kör 9 lednél 9 ms-enként ismétlödik, de mivel a szem
  //   asszem kb 20-30 ms onként olvas be képet az agyba, ezért folyamatosnak
  //   tűnik.
  // Annyi a baj csak ezzel hogy kevéssé lesz fényes. Ez növelhető ha egyszerre
  //   többet is megvilagitasz, de ilyenkor számolni kell hogy mennyit lehet
  //   max mert különben megsül. Alap ledböl elvileg 3-4 et lehetne(mások azt
  //   írták én nem számoltam ki).
  for(i = 0; i < nLed; i++){
    if(active[i]){ 
      digitalWrite(leds[i], HIGH);
      delay(1);    
      digitalWrite(leds[i], LOW);
    }
  }
  
}
