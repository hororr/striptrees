
#include "xmas.h"




int cprg = 0;

class Program {
 public:
 virtual void Init(void) {};
 virtual void Update(void) {};
};

class Prg1 : public Program {
  public:
  void Init(void) {
    led_clear();    
    Serial.println("Prg1");
  }

  void Update(void) {
    if (fade(3141)) { //  {  // állítható: base color, spd, új elemek száma, törölt elemek száma, shift van/nincs
      rnd_clear(100);
      rnd_add(40);
      shift_dn();
    }
  }
};

class Prg2 : public Program {
  private:
   int offs;
  public:
  void Init(void) {
    led_clear();    
    offs=0;
    Serial.println("Prg2");
  }

  void Update(void) { // állítható: szín periódus, világosság , sebesség (offs inkr)
    for (int i = 0; i < NUMPIXELS; i++) {
      colorwheel(i, (i + offs) % 256);
    }
    copy_b();
    offs++;
  }
};

class Prg3 : public Program {
  private:
   int offs;
   int startindx;
  public:
  void Init(void) {
    led_clear();    
    offs=0;
    startindx = 0;
    Serial.println("Prg3");
  }

  void Update(void) { // csóva hossz, sebesség, szín, véletlen indulás?, irány
    int l = 6;
    if ((offs % 2) == 0) {
      if (startindx < l) {
        if (startindx == 0) 
          setPixel1b(NUMPIXELS-1,255,255,255,255); 
            else
          setPixel1b(NUMPIXELS-1, 128 * (l - startindx) / l, 255, 255, 255);
        startindx++;
      }
      
      if (rndStart(94))
        startindx = 0;
    
      shift_dn();
      copy_b();
    }
    offs++;
  }
};

class Prg4 : public Program {
  private:
  public:
  void Init(void) {
    led_clear();    
    Serial.println("Prg4");
  }

  void Update(void) {
   if (fade(3141)) {
      led_clear();
      for (int i = 0; i < 100; i++) {
        colorwheel(random(NUMPIXELS), random(255));
      }
    }
  }
};

class Prg5 : public Program {
  private:
  int startindx;
  int len;
  int offs;
  public:
  void Init(void) {
    led_clear();    
    startindx=0;
    offs=0;
    len=4;
    Serial.println("Prg5");
  }

  void Update(void) {
   if ((offs % 2) == 0) {
      if (startindx < len) {
        if (startindx == 0) 
          colorwheel(NUMPIXELS-1,offs % 256,255);
            else
          colorwheel(NUMPIXELS-1,offs % 256,128*(len-startindx)/len);
        startindx++;
      }
      
      if (rndStart(94))
        startindx = 0;
    
      shift_dn();
      copy_b();
    }
    offs++;
  }
};

class Prg6 : public Program {
  private:
    float pos[10];
    float vel[10];
    int st[10];
    int col[10];
  public:
  void Init(void) {
    for(int i=0;i<10;i++) st[i]=0;
    led_clear();    
    Serial.println("Prg6");
  }

  void Update(void) {
      led_clear();
      for(int i=0;i<10;i++) {
      if (st[i]==0) {
          if (rndStart(98)) { // random start
              vel[i] = random(30,60)/20.0;
              col[i] = (int)random(255);
              st[i]=1;
          }
      }
      pos[i] += vel[i];
      vel[i] -= 0.015;
      if (vel[i]<-2.0) { 
          vel[i] = -2.0; 
      }
      if (pos[i]<0) {
        st[i]=0; pos[i]=0; vel[i]=0;
      }
      if ((st[i]!=0)&&(pos[i]< NUMPIXELS)) {
        colorwheel((int)pos[i],col[i],255);
        float v = 2.0*vel[i]+0.5; // csóva hossza
        int di = 0;
        for(int j=0;j<abs(v);j++) {
             colorwheel((int)(pos[i]+di),(col[i]+16*di),255);//(int)(255/(abs(di)+1)));
          if (v<0) di++; else di--;
        };
      }
      }
      copy_b();
  }
};

class Prg7 : public Program {
  private:
  public:
  void Init(void) {
    led_clear();    
    Serial.println("Prg7");
  }

  void Update(void) {
      if (fade(3141)) {
      led_clear();
      int range = 42; // 42-ig jó
      int base = 212; // piros/kék
      //int base = 212-85; // zöld/kék
      //int base = 212-2*85; // piros/zöld
    
      for(int i=0;i<100;i++) {
        colorwheel((int) random(NUMPIXELS),base + (int)random(-range,range),(int)random(64,255));
     }
     shift_dn();
  }
  }
};

class Prg8 : public Program {
  private:
      int sta[127];
      int colo[127];
      int poso[127];
  public:
  void Init(void) {
    for(int i=0;i<127;i++) sta[i]=0;
    led_clear();    
    Serial.println("Prg8");
  }

  int cfunc(int st) {
    float fst = st;
    int r =(int)(-0.25*fst*fst + 16.0*fst);
    if (r>255) r=255;
    if (r<0) r=0;
    return r;
  }
  
  void Update(void) {
      for(int i=0;i<127;i++) {
        if (sta[i]==0) {
          if (rndStart(97)) { 
              sta[i]++;
              poso[i]=(int)random(NUMPIXELS);
              colo[i]=(int)random(255);
          }
        } else {
          int b = cfunc(sta[i]);
          if (sta[i]==5) {
            if (rndStart(98)) sta[i]++;
          } else sta[i]++;
          if (sta[i]==65) sta[i]=0;
          if (colo[i]>127) setPixel1b(poso[i],b,255,255,255);
           else setPixel1b(poso[i],b,0,0,255);
        }
      }
    copy_b();
  }
};

class Prg9 : public Program {
  private:
    int stt;
    int seq[NUMPIXELS];
    int sss;
  public:
  void Init(void) {
    stt=0;
    sss=0;
    led_clear();    
    Serial.println("Prg9");
  }

  void Update(void) {
      int c;
  if (fade(1000)) {
    int n = (int)random(5,10);
    for(int j=0;j<n;j++) {
  switch(stt) {
    case 2:
    case 0: // seq generalas
    for(int i=0;i<NUMPIXELS;i++) seq[i]=i;
    for(int i=0;i<100;i++) {
      int a = (int)random(NUMPIXELS);
      int b = (int)random(NUMPIXELS);
      c = seq[a];
      seq[a] = seq[b];
      seq[b] = c;
    }
    sss=0;
    stt++;
    break;
    case 1:
    c = seq[sss];
    c |= 1;
    if (c<(NUMPIXELS/3)) 
       setPixel1b(c,127,0,255,0);
       else if (c<(2*NUMPIXELS/3)) setPixel1b(c,127,255,255,255); 
       else setPixel1b(c,127,255,0,0);
    sss++;
    if (sss>=NUMPIXELS) stt++;
    break;
    case 3:
        c = seq[sss];
        c |= 1;
        setPixel1(c,0,0,0);
        sss++;
        if (sss>=NUMPIXELS) stt++;
    break;
    default:
    sss=0;
    stt=0;
    break;
  }
    }
  }

  }
};

Prg1 Program1 = Prg1();
Prg2 Program2 = Prg2();
Prg3 Program3 = Prg3();
Prg4 Program4 = Prg4();
Prg5 Program5 = Prg5();
Prg6 Program6 = Prg6();
Prg7 Program7 = Prg7();
Prg8 Program8 = Prg8();
Prg9 Program9 = Prg9();

Program * prg_list[] = { &Program1, &Program2, &Program3, &Program4, &Program5,  &Program7, &Program8 , &Program9,0 };
// prg6 nagyon villog

void nextPrg(void) {
  cprg++;  
  if (!prg_list[cprg]) cprg=0;
  prg_list[cprg] -> Init();
}

int getcPrg(void) {
 return cprg;
}

void startPrg(void) {
  cprg = 0;
  prg_list[0] -> Init();
}

void startPrg(int _prg) {
  if ( _prg >= 8)
    _prg=0;
    
  cprg = _prg;
  prg_list[_prg] -> Init();
  
}


void updatePrg(void) {
  if (!prg_list[cprg]) cprg=0;
  prg_list[cprg] -> Update();
}


