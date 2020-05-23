import processing.serial.*;
import cc.arduino.*;
Arduino arduino;
PrintWriter output; // Saves a file to PC
final float pi = 3.14159265;
final float raddeg = pi/180.0;

final int MAX = 800, Xoffset = 100; // The x pos. of the leftmost boundary of the plot area, The Maximum output size shown on the screen
int[] val = new int[MAX], ss = new int[MAX]; // Read values, Sum of the read values (Past to Present)
float[] avgval = new float[MAX]; // Average of Read Values (Averages SHOWVALNUM(#) values from past to present)
int vali, showval; // the id currently being read, the value initially read from arduino analog read
boolean init_draw = false, sumplot; // initialized draw?, Draw sum of all the values instead averages?

int prevsec; // Every second outputs a read signal of spr to file
int sum,maxsum,minsum; // max and min of sums
int curmax,curmin; // max and min of values
boolean usecurrent,maxminsuminited; // range of value sizes to max and min read value? , is max and min initialized?
boolean gettingstandard, drawstandard; // gettingstandard values, draw difference of the standard value and the current measured value
boolean[] turnedclockwise = new boolean[MAX];

float[] stdval = new float[MAX], diffval = new float[MAX]; // Standard value within range(usually air), Current value - Standard Value
float[] tmpdiff = new float[MAX];
int comeacross, initi; // initi come across times, initial val to be standardized
float maxdiff, mindiff; // max and min of difference between standard and current read values
int stdi, stdn; // the id currently being standardized, range of values(godistance*__allphase)
boolean minimum_standardized;
final int minimumstandardizerounds = 4;

float minavgerr,lefta,leftb,righta,rightb;
int mindiffi,minpeaki,peakleft,peakright;
boolean findpeaksuccess;

final int sin_min_span_rate = 5;
final int SHOWVALNUM = 10;
final int delaysec = 1;
final int godistance = 20;
final int __allphase = 4; // *note: the range of read values is godistance*__allphase
final int __clockwise = 0;
final int __counterclockwise = 1;
int phase, turndir, phasecnt;
boolean motorturns,firstrun; // does the motor turn?, has the motor initialized running?

void draw() {
  if(mousePressed&&!motorturns){
    if(ButtonOver(leftturnButtonX,leftturnButtonY,leftturnButtonSizeX,leftturnButtonSizeY))TURN_MOTOR(true,false);
    if(ButtonOver(rightturnButtonX,rightturnButtonY,rightturnButtonSizeX,rightturnButtonSizeY))TURN_MOTOR(false,true);
  }
  if(millis()%delaysec==0&&motorturns)TURN_MOTOR(false,false);

  if(!init_draw)init();
  rect(Xoffset, 50, MAX, 400);

  showval = arduino.analogRead(0); // read value
  update_minmax();
  val[vali] = showval; // update val[vali] to showval
  // updates average of SHOWVALNUM# numbers from past to present
  avgval[vali]=0.0;
  for(int i=0;i<SHOWVALNUM;i++){
    avgval[vali]+=float(val[vali<i?vali+MAX-i:vali-i]);
  }
  avgval[vali]/=SHOWVALNUM;
  // Update Standard and Diff
  stdi = turndir==__clockwise?phasecnt:stdn-phasecnt-1;
  if(minimum_standardized){
    if(turndir==__clockwise){
      turnedclockwise[stdi] = true;
      tmpdiff[stdi] = avgval[vali] - stdval[stdi];
    }else{
      diffval[stdi] = (tmpdiff[stdi] + avgval[vali] - stdval[stdi]) / 2.0;
      if(turnedclockwise[stdi]){
        if(maxdiff<diffval[stdi]){
          maxdiff=diffval[stdi];
          draw_numbers();
        }
        if(mindiff>diffval[stdi]){
          mindiff=diffval[stdi];
          mindiffi = stdi;
          draw_numbers();
        }
      }
    }
  }
  if(gettingstandard){
    initi++;
    stdval[stdi] += avgval[vali];
    if(initi==minimumstandardizerounds*2*stdn){
      for(stdi=0;stdi<stdn;stdi++){
        stdval[stdi]/=minimumstandardizerounds*2;
        diffval[stdi] = 0.0;
        turnedclockwise[stdi] = false;
      }
      gettingstandard = false;
      minimum_standardized = true;
      fill(255);
      rect(getstandardButtonX,getstandardButtonY,getstandardButtonSizeX,getstandardButtonSizeY);
      rect(checkstandardButtonX,checkstandardButtonY,checkstandardButtonSizeX,checkstandardButtonSizeY);
      fill(0);
      text("Standard",getstandardButtonX+10,getstandardButtonY+20);
      text("Standardized", Xoffset + 5, 20);
    }
  }
  // plot it
  plotit();
  // If one second passed, write the remaining data to the file
  /*if(maxminsuminited&&((prevsec+1)%60==second()||prevsec==-1)){
    prevsec = second();
    output.print(sum+"\n");
    output.flush();
  }*/
  vali=(vali+1)%MAX; // update vali
}

void update_minmax(){
  // update min max maxsum minsum
  if(showval<0)showval=0;
  else if(showval>1023)showval=1023;
  if(firstrun){
    if(curmax<showval){
      curmax = showval;
      draw_numbers();
    }
    if(curmin>showval){
      curmin = showval;
      draw_numbers();
    }
    sum = sum + showval - val[vali];
    if(maxsum<sum){
      maxsum = sum;
      draw_numbers();
    }
    if(minsum>sum){
      minsum = sum;
      draw_numbers();
    }
    ss[vali] = sum;
  }
}

void plotit(){
  if(findpeaksuccess){
    text("li="+peakleft+" ri="+peakright+" minpeaki="+minpeaki+" avgerror="+minavgerr, Xoffset+100, 80);
    
    fill(255,0,0);
    //line(peakleft*MAX/stdn+Xoffset, conv_diff_plot(diffval[peakleft]) , minpeaki*MAX/stdn+Xoffset, conv_diff_plot(diffval[minpeaki]));
    //line(minpeaki*MAX/stdn+Xoffset, conv_diff_plot(diffval[minpeaki]) , peakright*MAX/stdn+Xoffset, conv_diff_plot(diffval[peakright]));
  }else{
    fill(0);
    text("Peak not found...", Xoffset+100, 80);
  }
  fill(0);
  text("Range: " + stdn, Xoffset+10, 80);
  text("Maxdiff: " + maxdiff, Xoffset+10, 100);
  text("Mindiff: " + mindiff, Xoffset+10, 120);
  text("Sum: " + sum, Xoffset+10, 140);
  text("MaxSum: " + maxsum, Xoffset+10, 160);
  text("MinSum: " + minsum, Xoffset+10, 180);
  text("Input: " + showval, Xoffset+10, 200);
  text("Max: " + curmax, Xoffset+10, 220);
  text("Min: " + curmin, Xoffset+10, 240);
  fill(255);
  if(sumplot){
    for (int i=(vali+1)%MAX, j=0; i!=vali; i=(i+1)%MAX, j++) {
      point(j+Xoffset, conv_sum_plot(ss[i]));
      if (j>0)line(j-1+Xoffset, conv_sum_plot(ss[(i>0?i-1:MAX-1)]), j+Xoffset, conv_sum_plot(ss[i]));
    }
  }else if(drawstandard){
    for(int i=0;i<stdn;i++){
      point(i*MAX/stdn+Xoffset, conv_diff_plot(diffval[i]));
      if(i>0)line((i-1)*MAX/stdn+Xoffset, conv_diff_plot(diffval[i-1]), i*MAX/stdn+Xoffset, conv_diff_plot(diffval[i]));
    }
  }else{
    for (int i=(vali+1)%MAX, j=0; i!=vali; i=(i+1)%MAX, j++) {
      point(j+Xoffset, conv_plot(avgval[i]));
      if (j>0)line(j-1+Xoffset, conv_plot(avgval[(i>0?i-1:MAX-1)]), j+Xoffset, conv_plot(avgval[i]));
    }
  }
}

void setup() {
  // init arduino
  arduino = new Arduino(this, Arduino.list()[0], 57600);
  // sets false
  findpeaksuccess = gettingstandard = drawstandard = motorturns = firstrun = maxminsuminited = usecurrent = sumplot = false;
  // file output
  // output = createWriter("sum vs time "+year()+month()+day()+" ["+hour()+"]["+minute()+"]["+second()+"].csv");
  prevsec = -1;
  // motor turn init
  turndir = phase = phasecnt = 0;
  // val and sums init
  curmax = sum = 0;
  curmin = 1023;
  for (int i=0; i<MAX; i++)val[i]=450;
  vali=0;
  // Standard values
  stdn = godistance*__allphase;
  // set draw parameters
  surface.setResizable(true);
  background(200);
  size(1000, 500);
  stroke(0);
  strokeWeight(2);
  textFont(createFont("Times New Roman", 16, true), 16);
}

void mouseClicked() {
  if(ButtonOver(getstandardButtonX,getstandardButtonY,getstandardButtonSizeX,getstandardButtonSizeY)){
    gettingstandard = !gettingstandard;
    drawstandard = false;
    fill(255);
    rect(getstandardButtonX,getstandardButtonY,getstandardButtonSizeX,getstandardButtonSizeY);
    if(gettingstandard){
      fill(255);
      rect(Xoffset, 5, 100, 20);
      findpeaksuccess = minimum_standardized = false;
      maxdiff = mindiff = 0.0;
      comeacross = initi = 0;
      for(int i=0;i<stdn;i++)stdval[i]=0.0;
      fill(0);
      text("Stop Get",getstandardButtonX+10,getstandardButtonY+20);
    }else{
      fill(0);  
      text("Standard",getstandardButtonX+10,getstandardButtonY+20);
    }
  }
  if(ButtonOver(checkstandardButtonX,checkstandardButtonY,checkstandardButtonSizeX,checkstandardButtonSizeY)){
    drawstandard = !drawstandard;
    if(drawstandard)fill(0);
    else fill(255,255,255);
    rect(checkstandardButtonX,checkstandardButtonY,checkstandardButtonSizeX,checkstandardButtonSizeY);
    draw_numbers();
  }
  if(ButtonOver(resetmaxminsumButtonX,resetmaxminsumButtonY,resetmaxminsumButtonSizeX,resetmaxminsumButtonSizeY)){
      maxminsuminited = true;
      sum = 0;
      for(int i=0;i<MAX;i++)sum+=val[i];
      maxsum=minsum=sum;
  }
  if (ButtonOver(runpauseButtonX, runpauseButtonY, runpauseButtonSizeX, runpauseButtonSizeY)) { // runpauseButton
    motorturns = !motorturns;
    if(!firstrun){
      sum = 0;
      for(int i=0;i<MAX;i++)sum+=val[i];
      maxsum=minsum=sum;
    }
    firstrun=true;
  }
  if(ButtonOver(resetButtonX,resetButtonY,resetButtonSizeX,resetButtonSizeY)){
    phase=phasecnt=0;
    turndir=__counterclockwise;
  }
  if(ButtonOver(checkcurButtonX,checkcurButtonY,checkcurButtonSizeX,checkcurButtonSizeY)){
    usecurrent=!usecurrent;
    if(usecurrent)fill(0);
    else fill(255,255,255);
    rect(checkcurButtonX,checkcurButtonY,checkcurButtonSizeX,checkcurButtonSizeY);
    draw_numbers();
  }
  if(ButtonOver(sumplotcheckButtonX,sumplotcheckButtonY,sumplotcheckButtonSizeX,sumplotcheckButtonSizeY)){
    sumplot=!sumplot;
    if(sumplot)fill(0);
    else fill(255,255,255);
    rect(sumplotcheckButtonX,sumplotcheckButtonY,sumplotcheckButtonSizeX,sumplotcheckButtonSizeY);
    draw_numbers();
  }
}
void calpeak(){
  if(mindiffi-stdn/5<6||mindiffi+stdn/5>stdn-5){
    findpeaksuccess=false;
    return;
  }
  // mindiffi
  float lefttmperr,righttmperr,sumtmperr,a,b,tmplefta,tmprighta,tmpleftb,tmprightb,leftminerr,rightminerr;
  int peaki,tmpminpeaki=mindiffi-stdn/5, tmpmaxpeaki=mindiffi+stdn/5, leftmini=0,rightmini=0;
  minavgerr = 1024.0;
  tmplefta=tmprighta=tmpleftb=tmprightb=0.0;
  for(peaki = tmpminpeaki;peaki < tmpmaxpeaki; peaki++){
    leftminerr=rightminerr=1024.0;
    for(int lefti=1;lefti<peaki-5;lefti++){
      a = (diffval[lefti]-diffval[peaki])/(lefti-peaki);
      b = diffval[lefti]-a*lefti;
      lefttmperr=0.0;
      for(int i=lefti;i<=peaki;i++){
        lefttmperr+=abs(a*i+b-diffval[i]);
      }
      lefttmperr/=peaki-lefti+1;
      if(leftminerr>lefttmperr){
        leftminerr=lefttmperr;
        leftmini=lefti;
        tmplefta=a;
        tmpleftb=b;
      }
    }
    for(int righti=peaki+1+5;righti<stdn;righti++){
      a = (diffval[righti]-diffval[peaki])/(righti-peaki);
      b = diffval[righti]-a*righti;
      righttmperr=0.0;
      for(int i=peaki;i<=righti;i++){
        righttmperr+=abs(a*i+b-diffval[i]);
      }
      righttmperr/=righti-peaki+1;
      if(rightminerr>righttmperr){
        rightminerr=righttmperr;
        rightmini=righti;
        tmprighta=a;
        tmprightb=b;
      }
    }
    sumtmperr = (leftminerr + rightminerr)/2;
    if(minavgerr>sumtmperr){
      minavgerr=sumtmperr;
      peakleft=leftmini;
      peakright=rightmini;
      minpeaki=peaki;
      lefta=tmplefta;
      leftb=tmpleftb;
      righta=tmprighta;
      rightb=tmprightb;
    }
  }
  findpeaksuccess = true;
}

void TURN_MOTOR(boolean left, boolean right){
  phasecnt++;
  if (right||turndir==__counterclockwise&&!left) {
    switch(phase) {
    case 0:
      arduino.digitalWrite(2, Arduino.LOW);
      arduino.digitalWrite(4, Arduino.HIGH);
      break;
    case 1:
      arduino.digitalWrite(3, Arduino.HIGH);
      arduino.digitalWrite(4, Arduino.LOW);
      break;
    case 2:
      arduino.digitalWrite(3, Arduino.LOW);
      arduino.digitalWrite(5, Arduino.HIGH);
      break;
    case 3:
      arduino.digitalWrite(2, Arduino.HIGH);
      arduino.digitalWrite(5, Arduino.LOW);
      break;
    }
  } else {
    switch(phase) {
    case 0:
      arduino.digitalWrite(3, Arduino.LOW);
      arduino.digitalWrite(4, Arduino.HIGH);
      break;
    case 1:
      arduino.digitalWrite(2, Arduino.HIGH);
      arduino.digitalWrite(4, Arduino.LOW);
      break;
    case 2:
      arduino.digitalWrite(2, Arduino.LOW);
      arduino.digitalWrite(5, Arduino.HIGH);
      break;
    case 3:
      arduino.digitalWrite(3, Arduino.HIGH);
      arduino.digitalWrite(5, Arduino.LOW);
      break;
    }
  }
  phase=(phase+1)%4;
  if (phasecnt==stdn) {
    phasecnt=0;
    turndir=1-turndir;
    if(turndir==__clockwise)calpeak();
  }
}

void init(){
  init_draw = true;
  draw_numbers();
  fill(255);
  rect(runpauseButtonX,runpauseButtonY,runpauseButtonSizeX,runpauseButtonSizeY);
  rect(leftturnButtonX,leftturnButtonY,leftturnButtonSizeX,leftturnButtonSizeY);
  rect(rightturnButtonX,rightturnButtonY,rightturnButtonSizeX,rightturnButtonSizeY);
  rect(resetButtonX,resetButtonY,resetButtonSizeX,resetButtonSizeY);
  rect(checkcurButtonX,checkcurButtonY,checkcurButtonSizeX,checkcurButtonSizeY);
  rect(resetmaxminsumButtonX,resetmaxminsumButtonY,resetmaxminsumButtonSizeX,resetmaxminsumButtonSizeY);
  rect(sumplotcheckButtonX,sumplotcheckButtonY,sumplotcheckButtonSizeX,sumplotcheckButtonSizeY);
  rect(getstandardButtonX,getstandardButtonY,getstandardButtonSizeX,getstandardButtonSizeY);
  rect(checkstandardButtonX,checkstandardButtonY,checkstandardButtonSizeX,checkstandardButtonSizeY);
  fill(0);
  textFont(createFont("Times New Roman", 24, true), 24);
  text("► /",runpauseButtonX+4,runpauseButtonY+23);
  textFont(createFont("Times New Roman", 16, true), 16);
  text("▌▌",runpauseButtonX+59,runpauseButtonY+20);
  text("Rotate <<",leftturnButtonX+10,leftturnButtonY+20);
  text("Rotate >>",rightturnButtonX+10,rightturnButtonY+20);
  text("Reset  >>",resetButtonX+10,resetButtonY+20);
  text("Current",checkcurButtonX-55,checkcurButtonY+16);
  text("Reset Sum",resetmaxminsumButtonX+5,resetmaxminsumButtonY+20);
  text("Sum plot",sumplotcheckButtonX-65,sumplotcheckButtonY+16);
  text("Standard",getstandardButtonX+10,getstandardButtonY+20);
  text("Draw Std",checkstandardButtonX-70,checkstandardButtonY+16);
}

void draw_numbers(){
  fill(200);
  strokeWeight(0);
  rect(0,0,Xoffset-2,500);
  rect(Xoffset+MAX+2,0,Xoffset-2,500);
  strokeWeight(2);
  fill(0);
  if(sumplot){
    float step = (maxsum - minsum) / 10.0;
    for(float j=0.0;j<=10.0;j+=1.0){
      text(minsum + step*j, 5, 457-j*40);
      text(minsum + step*j, 910, 457-j*40);
    }
  }else if(usecurrent){ // curmin curmax
    float step = (curmax - curmin) / 10.0;
    for(float j=0.0;j<=10.0;j+=1.0){
      text(curmin + step*j, 40, 457-j*40);
      text(curmin + step*j, 910, 457-j*40);
    }
  }else if(drawstandard){
    float step = (maxdiff - mindiff) / 10.0;
    for(float j=0.0;j<=10.0;j+=1.0){
      text(mindiff + step*j, 40, 457-j*40);
      text(mindiff + step*j, 910, 457-j*40);
    }
    step = stdn / 20.0;
    for(float j=0.0;j<=20.0;j+=1.0){
      text(int(j*step), Xoffset-6+j*step*MAX/stdn, 43);
    }
  }else{
    for (float i=0.0; i<=1000.0; i+=100.0) {
      text(str(i), 50, 457-i*25/64);
      text(str(i), 910, 457-i*25/64);
    }
  }
}

int conv_plot(float x) { // <- x = 0~1023 or x = curmin~curmax
  if(usecurrent) return max(min(int((curmin-x) * 400.0 / (curmax-curmin) + 450.0),450),50);
  return max(min(int(-x * 25.0/ 64.0 + 450.0),450),50);
}
int conv_sum_plot(float x) { // <- x = 0~1023 or x = curmin~curmax
  return max(min(int((minsum-x) * 400.0 / (maxsum-minsum) + 450.0),450),50);
}
int conv_diff_plot(float x){ // <- x = mindiff~maxdiff
  return max(min(int((mindiff-x) * 400.0 / (maxdiff-mindiff) + 450.0),450),50);
}

boolean ButtonOver(int __x, int __y, int __w, int __h) {return mouseX>=__x && mouseX<=__x+__w && mouseY>=__y && mouseY<=__y+__h;}

// Buttons
final int runpauseButtonX = 100, runpauseButtonY = 460, runpauseButtonSizeX = 80, runpauseButtonSizeY = 30;
final int leftturnButtonX = 190, leftturnButtonY = 460, leftturnButtonSizeX = 80, leftturnButtonSizeY = 30;
final int rightturnButtonX = 280, rightturnButtonY = 460, rightturnButtonSizeX = 80, rightturnButtonSizeY = 30;
final int resetButtonX = 370, resetButtonY = 460, resetButtonSizeX = 80, resetButtonSizeY = 30;
final int checkcurButtonX = 510, checkcurButtonY = 465, checkcurButtonSizeX = 20, checkcurButtonSizeY = 20;
final int resetmaxminsumButtonX = 540, resetmaxminsumButtonY = 460, resetmaxminsumButtonSizeX = 80, resetmaxminsumButtonSizeY = 30;
final int sumplotcheckButtonX = 690, sumplotcheckButtonY = 465, sumplotcheckButtonSizeX = 20, sumplotcheckButtonSizeY = 20;
final int getstandardButtonX = 720, getstandardButtonY = 460, getstandardButtonSizeX = 80, getstandardButtonSizeY = 30;
final int checkstandardButtonX = 875, checkstandardButtonY = 465, checkstandardButtonSizeX = 20, checkstandardButtonSizeY = 20;