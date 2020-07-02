import controlP5.*;

//define area
final float border_spacing_ratio = 0.025;
final float rMin = 0.1, cMin = 1e-5, zMin = 1e-8, rangeRatio = 1e11;
final int done_btn_x1=330,done_btn_x2=400,done_btn_y1=405,done_btn_y2=430;

//declare area
boolean calculate_eqv,draw_ready;
ControlP5 cp5;
String eqn, Z_tmp_name, lasteqn;
String[] Z_name = new String[20];
boolean[] is_N = new boolean[20];
int parentheses, eqn_n, Z_n = 0;
float border_space;
float sqr_w;
float width_border;
int axis_n;

int freqn = 200;
Complex[] Z = new Complex[1010];
float[] freq = new float[1010];
float min_abs,max_abs,min_re,max_re,min_img,max_img,min_p,max_p;

boolean[] H_activate = new boolean[20];
HScrollbar[] hs = new HScrollbar[20];

CheckBox log_bode_check;
boolean log_bode;

//main code
void setup() {
  // Interface Definition
  background(color(200));
  size(1250, 700);
  border_space = height*border_spacing_ratio;
  sqr_w = width/3.7;
  width_border = width/3-sqr_w-border_space;
  
  // Equation input text area and Plot borders
  calculate_eqv = draw_ready = false;
  cp5 = new ControlP5(this);
  plot_borders();
  addTextfield("eqn_text", width_border, 5.5*border_space + sqr_w, "");
  addTextfield("min_freq_text", width_border + sqr_w*0.25, 7.6*border_space + sqr_w, "1");
  cp5.get(Textfield.class, "min_freq_text").setSize(110,20).setFont(createFont("Times new roman", 18));
  addTextfield("max_freq_text", width_border + sqr_w*0.65, 7.6*border_space + sqr_w, "1000000");
  cp5.get(Textfield.class, "max_freq_text").setSize(110,20).setFont(createFont("Times new roman", 18));
  
  log_bode_check = cp5.addCheckBox("checkBox")
                .setPosition(190, 407)
                .setColorForeground(color(120))
                .setColorBackground(color(255))
                .setColorActive(color(0))
                .setSize(20, 20)
                .addItem("logz", 0)
                .hideLabels()
                ;
  log_bode = false;
  
  /*  cp5.addTextfield(Z_name) 
    .setColorBackground(color(255))
    .setColor(color(0))
    .setSize(343, 33)
    .setFont(createFont("Times new roman", 26))
    .setAutoClear(false)
    .setColorCaptionLabel(color(0))
    .setColorCursor(color(0))
    .setCaptionLabel("")
    .setPosition(x, y);
  cp5.get(Textfield.class, Z_name).setText(setText);*/
  
  eqn_in_field();
  cp5.get(Textfield.class, "eqn_text").setText("(Rs)((Qdl)((Rct)(Zw)))");
    
  axis_n = 6; // axis n
  
  // initialize scrollbars
  for(int i=0;i<20;i++) H_activate[i] = false;
}

void draw() {  
  check_equation();
  
  if(draw_ready)
    draw_three_plots();
  
  //for(int i=0;i<20;i++)
  //  print("(" + i + ")" + (H_activate[i]?"T":"F") +" ");
  
  //print("\n");
  for(int i=0;i<20;i++)
    if(H_activate[i]){
      hs[i].update();
      hs[i].display();
    }
}

void mousePressed() {
  if(mouseX>=done_btn_x1&&mouseX<=done_btn_x2&&mouseY>=done_btn_y1&&mouseY<=done_btn_y2)
    calculate_eqv = true;
}

float log10(float x){return log(x)/log(10);}
float lt(float xl,float xh, float yl, float yh, float x){ return (x-xl)*(yh-yl)/(xh-xl)+yl; }
float logt(float xl,float xh, float yl, float yh, float x){ return pow( 10 , lt( log10(xl), log10(xh), log10(yl), log10(yh), log10(x) ) ); }

class Complex {
  float real;   // the real part
  float img;   // the imaginary part
  public Complex(Complex Z) {
    this.real = Z.real;
    this.img = Z.img;
  }
  public Complex(float real, float img) {
    this.real = real;
    this.img = img;
  }
  public float phase() {
    return atan(this.img/this.real)*180/PI;
  }
  public float absolute() {
    return pow(this.real*this.real + this.img*this.img, 0.5);
  }
  public Complex reciprocal() {
    float real = this.real / (this.real * this.real + this.img * this.img);
    float img = -this.img / (this.real * this.real + this.img * this.img);
    return new Complex(real, img);
  }
  public Complex multi(Complex b) {
    float real = this.real * b.real - this.img * b.img;
    float img = this.real * b.img + this.img * b.real;
    return new Complex(real, img);
  }
  public Complex plus(Complex b) {
    float real = this.real + b.real;
    float img = this.img + b.img;
    return new Complex(real, img);
  }
}

class HScrollbar {
  int swidth, sheight;    // width and height of bar
  float xpos, ypos;       // x and y position of bar
  float spos, newspos;    // x position of slider
  float sposMin, sposMax; // max and min values of slider
  int loose;              // how loose/heavy
  boolean over;           // is the mouse over the slider?
  boolean locked;
  float ratio;

  HScrollbar (float xp, float yp, int sw, int sh, int l) {
    swidth = sw;
    sheight = sh;
    int widthtoheight = sw - sh;
    ratio = (float)sw / (float)widthtoheight;
    xpos = xp;
    ypos = yp-sheight/2;
    spos = xpos + swidth/2 - sheight/2;
    newspos = spos;
    sposMin = xpos;
    sposMax = xpos + swidth - sheight;
    loose = l;
  }

  void update() {
    if (overEvent()) {
      over = true;
    } else {
      over = false;
    }
    if (mousePressed && over) {
      locked = true;
    }
    if (!mousePressed) {
      locked = false;
    }
    if (locked) {
      newspos = constrain(mouseX-sheight/2, sposMin, sposMax);
    }
    if (abs(newspos - spos) > 1) {
      int id = ((width/3>sposMax)?0:(width/3*2>sposMax?1:2))*3 + ((500>ypos)?0:(600>ypos?1:2)) - 1, idminus=0;
      float value = (spos-sposMin)/(sposMax-sposMin);
      boolean is_N_tmp = is_N[id];
      spos = spos + (newspos-spos)/loose;
      for(int i=0;i<=id;i++)
        idminus+=is_N[i]?1:0;
      id-=idminus;
      print( id + " Moved to : " + value + "\n");
      switch(Z_name[id].charAt(0)){
        case 'R':
          cp5.get(Textfield.class, Z_name[id]).setText(str(rMin*pow(rangeRatio, value)));
          break;
        case 'Q':
          if(is_N_tmp){
            cp5.get(Textfield.class, Z_name[id]+"n").setText(str(value));
            break;
          }
        case 'C':
          cp5.get(Textfield.class, Z_name[id]).setText(str(cMin*pow(rangeRatio, value)));
          break;
        case 'Z':
          cp5.get(Textfield.class, Z_name[id]).setText(str(zMin*pow(rangeRatio, value)));
          break;
      }
      draw_ready = true;
    }
  }

  float constrain(float val, float minv, float maxv) {
    return min(max(val, minv), maxv);
  }

  boolean overEvent() {
    if (mouseX > xpos && mouseX < xpos+swidth &&
       mouseY > ypos && mouseY < ypos+sheight) {
      return true;
    } else {
      return false;
    }
  }

  void display() {
    noStroke();
    fill(color(150,150,255));
    rect(xpos, ypos, swidth, sheight);
    if (over || locked) {
      fill(0, 0, 0);
    } else {
      fill(100, 100, 100);
    }
    rect(spos, ypos, sheight, sheight);
  }

  float getPos() {
    // Convert spos to be values between
    // 0 and the total width of the scrollbar
    return spos * ratio;
  }
}

Complex Z_eqn(int parentheses_z_eqn, String str, float f) {
  //print("In: " + str + "\n");
  boolean last = true;
  Complex Z_tmp = new Complex(0, 0);
  Complex Z_total = new Complex(0, 0);
  int strn = str.length(), par = 0;
  String tmpstr = "";
  for (int i=0; i<strn; i++) {
    switch(str.charAt(i)) {
    case '(':
      last = false;
      if (par>0)
        tmpstr += str.charAt(i);
      par++;
      break;
    case ')':
      last = false;
      par--;
      if (par>0)
        tmpstr += str.charAt(i);
      else {
        Z_tmp = Z_eqn(parentheses_z_eqn+1, tmpstr, f);
        //print("Return to " + str + ", " + Z_tmp.real + " | " + Z_tmp.img + "\n");
        if (parentheses_z_eqn%2==0) { //even
          Z_total = Z_total.plus(Z_tmp);
          //print("tran " + Z_tmp.real + ", " + Z_tmp.img + "\n");
          //print("tran " + Z_total.real + ", " + Z_total.img + "\n");
        } else { //odd
          if(Z_total.real==0&&Z_total.img==0)
            Z_total = Z_tmp;
          else
            Z_total = Z_total.reciprocal().plus(Z_tmp.reciprocal()).reciprocal();
          //print("bing " + Z_tmp.real + ", " + Z_tmp.img + "\n");
          //print("bing " + Z_total.real + ", " + Z_total.img + "\n");
        }
        tmpstr="";
      }
      break;
    default:
      tmpstr += str.charAt(i);
      break;
    }
  }
  //print("Return to 2 " + str + ", " + Z_total.real + " | " + Z_total.img + "\n");
  if (last) {
    switch(tmpstr.charAt(0)) {
    case 'R':
      Z_total.real = Float.valueOf(cp5.get(Textfield.class, tmpstr).getText());
      Z_total.img = 0;
      break;
    case 'Q':
      float n = Float.valueOf(cp5.get(Textfield.class, tmpstr+"n").getText());
      float A0 = Float.valueOf(cp5.get(Textfield.class, tmpstr).getText())/1e9;
      //print("Q phase!!" + f + " " + A0 + " " + n + " cos(PI*n/2) = " + cos(PI*n/2) + " sin(PI*n/2) = " + sin(PI*n/2) + "\n");
      Z_total.real = cos(PI*n/2)/A0/pow(f, n);
      Z_total.img = -sin(PI*n/2)/A0/pow(f, n);
      //print("R:" + cos(PI*n/2)/A0/pow(f, n) + " I:" + -sin(PI*n/2)/A0/pow(f, n) + "\n");
      break;
    case 'Z':
      Z_total.real = 1/Float.valueOf(cp5.get(Textfield.class, tmpstr).getText())/pow(2*f, 0.5);
      Z_total.img = -Z_total.real;
      break;
    case 'C':
      //print("Capa!!" + f + " " + tmpstr + " " + -1/f/Float.valueOf(cp5.get(Textfield.class, tmpstr).getText())*1e9 + "\n");
      Z_total.real = 0;
      Z_total.img = -1/f/Float.valueOf(cp5.get(Textfield.class, tmpstr).getText())*1e9;
      break;
    }
  }
  return Z_total;
}

void addTextfield(String Z_name, float x, float y, String setText){
  cp5.addTextfield(Z_name) 
    .setColorBackground(color(255))
    .setColor(color(0))
    .setSize(343, 33)
    .setFont(createFont("Times new roman", 26))
    .setAutoClear(false)
    .setColorCaptionLabel(color(0))
    .setColorCursor(color(0))
    .setCaptionLabel("")
    .setPosition(x, y);
  cp5.get(Textfield.class, Z_name).setText(setText);
}

void eqn_in_field(){
  textFont(createFont("Times New Roman", 30));
  stroke(color(200));
  fill(color(200));
  rect(width_border, 1.1*border_space + sqr_w, width, height);
  stroke(color(0));
  rect(189,406,21,21);
  fill(color(255));
  rect(done_btn_x1, done_btn_y1, done_btn_x2-done_btn_x1, done_btn_y2-done_btn_y1);
  fill(color(0));
  text("Equation:", width_border, 5.5*border_space + sqr_w - 7);
  textFont(createFont("Times New Roman", 20));
  text("DONE", done_btn_x1+8, done_btn_y2-5);
  text("Freq(Hz):", width_border, 8.5*border_space + sqr_w);
  text("~", width_border+sqr_w*0.6, 8.5*border_space + sqr_w);
  text("Log Bode", 220, 423);
}

void plot_borders() {
  stroke(color(200));
  fill(color(200));
  rect(0, 0, width, 3*border_space + sqr_w);
  stroke(0);
  fill(color(255));
  rect(width_border, border_space*2, sqr_w, sqr_w); // sqr_w = width/3.5 = 1200/3.5 = 343
  rect(width/3+width_border, border_space*2, sqr_w, sqr_w);
  rect(width/3*2+width_border, border_space*2, sqr_w, sqr_w);
  fill(color(0));
  textFont(createFont("Times New Roman", 20));
  text("Nyquist plot ~ Re(Z) vs Im(Z) (Ω)", width_border+28, border_space+10);
  if(log_bode) text("Bode plot ~ log|Z|(Ω) vs log(Freq)(Hz)", sqr_w+2*width_border+60, border_space+10);
  else text("Bode plot ~ |Z|(Ω) vs Freq(Hz)", sqr_w+2*width_border+60, border_space+10);
  if(log_bode) text("Bode plot ~ Phase(°) vs log(Freq)(Hz)", 2*sqr_w+3*width_border+70, border_space+10);
  else text("Bode plot ~ Phase(°) vs Freq(Hz)", 2*sqr_w+3*width_border+70, border_space+10);
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isFrom(log_bode_check)) {
       draw_three_plots();
  }
}

void draw_three_plots(){
  int i;
  float min_freq = float(cp5.get(Textfield.class, "min_freq_text").getText()), max_freq = float(cp5.get(Textfield.class, "max_freq_text").getText());
  float max_min_ratio = max_freq / min_freq, max_min_diff = max_freq-min_freq;
  log_bode = log_bode_check.getState(0);
  // Define frequency range
  if(log_bode){
    for (i=0; i<=freqn; i++)
      freq[i] = min_freq*pow(max_min_ratio, float(i)/freqn);
  }else{
    for (i=0; i<=freqn; i++)
      freq[i] = min_freq+i*max_min_diff/freqn;
  }
    
  //calculate Z(freq) and min max ranges
  min_img = min_re = min_abs = min_p = 1e20; max_abs = max_img = max_re = max_p = -1e20;
  for (i=0; i<=freqn; i++) {
    Z[i] = Z_eqn(0, eqn, freq[i]);
    min_abs = min(min_abs, Z[i].absolute()); max_abs = max(max_abs, Z[i].absolute());
    min_p = min(min_p, Z[i].phase()); max_p = max(max_p, Z[i].phase());
    min_re = min(min_re, Z[i].real); max_re = max(max_re, Z[i].real);
    min_img = min(min_img, Z[i].img); max_img = max(max_img, Z[i].img);
  }
  if(abs(min_abs-max_abs)<1){
    min_abs -= 10;
    max_abs += 10;
  }
  if(abs(min_re-max_re)<1){
    min_re -= 10;
    max_re += 10;
  }
  if(abs(min_img-max_img)<1){
    min_img -= 10;
    max_img += 10;
  }
  if(abs(min_p-max_p)<1){
    min_p -= 10;
    max_p += 10;
  }
  
  // Clear Plot graph    
  plot_borders();
  
  // Ticks
  for(i=0;i<=axis_n;i++){
    line( width_border+sqr_w*i/axis_n , border_space*2+sqr_w , width_border+sqr_w*i/axis_n , border_space*2+sqr_w - (2-(i%2))*5 );
    line( width_border , border_space*2+sqr_w*(axis_n-i)/axis_n , width_border + (2-(i%2))*5 , border_space*2+sqr_w*(axis_n-i)/axis_n );
    line( width/3+width_border+sqr_w*i/axis_n , border_space*2+sqr_w , width/3+width_border+sqr_w*i/axis_n , border_space*2+sqr_w - (2-(i%2))*5 );
    line( width/3+width_border , border_space*2+sqr_w*(axis_n-i)/axis_n , width/3+width_border + (2-(i%2))*5 , border_space*2+sqr_w*(axis_n-i)/axis_n );
    line( width/3*2+width_border+sqr_w*i/axis_n , border_space*2+sqr_w , width/3*2+width_border+sqr_w*i/axis_n , border_space*2+sqr_w - (2-(i%2))*5 );
  }
  for(i=0;i<=9;i++){
    line( width/3*2+width_border , border_space*2+(sqr_w-1)*(9-i)/9 , width/3*2+width_border + (2-(i%2))*5 , border_space*2+(sqr_w-1)*(9-i)/9 );
  }

  fill(color(0));
  textFont(createFont("Times New Roman", 15));
  float tmpf, tmpx, tmpy;
  int dec;
  
  // Nyquist
  for(i=0;i<=axis_n;i++){
    // X axis
    tmpf = (min_re*(axis_n-i)+max_re*i)/axis_n; tmpx = width_border+sqr_w/axis_n*i - 22; tmpy = border_space*2+sqr_w+17;
    if(abs(tmpf)<0.01) text( "0.00" , tmpx, tmpy );
    else{
      dec = floor(log(tmpf)/log(10));
      text( nf(tmpf/pow(10.0, dec), 1, 2)+"E"+dec , tmpx, tmpy );
    }
    // Y axis
    tmpf = (max_img*(axis_n-i)+min_img*i)/axis_n; tmpx = 3; tmpy = border_space*2+sqr_w*(axis_n-i)/axis_n;
    //print("tmpf = " + tmpf + "\n");
    if(abs(tmpf)<0.01) text( "0.00" , tmpx, tmpy );
    else{
      dec = floor(log(abs(tmpf))/log(10));
      text( nf(tmpf/pow(10.0, dec), 1, 2)+"E"+dec , tmpx, tmpy );
    }
  }
  for(i=0;i<=freqn;i++)
    rect( lt(min_re, max_re, width_border+1, width_border+sqr_w-1, Z[i].real)-1 , lt(min_img, max_img, border_space*2+1, border_space*2+sqr_w-1, Z[i].img)-1 , 2 , 2 );
  
  // |Z| bode plot
  for(i=0;i<=axis_n;i++){
    // X axis
    if(log_bode) tmpf = logt( 1 , pow(10,axis_n) , min_freq , max_freq , pow(10,i) );
    else tmpf = (freq[0]*(axis_n-i)+freq[freqn]*i)/axis_n;
    tmpx = width/3+width_border+sqr_w/axis_n*i - 22; tmpy = border_space*2+sqr_w+17;
    if(abs(tmpf)<0.01) text( "0.00" , tmpx, tmpy );
    else{
      dec = floor(log(tmpf)/log(10));
      text( nf(tmpf/pow(10.0, dec), 1, 2)+"E"+dec , tmpx, tmpy );
    }
    // Y axis
    if(log_bode) tmpf = logt( 1 , pow(10,axis_n) , min_abs , max_abs , pow(10,i) );
    else tmpf = (min_abs*(axis_n-i)+max_abs*i)/axis_n;
    tmpx = width/3+3; tmpy = border_space*2+sqr_w*(axis_n-i)/axis_n;
    //print("tmpf = " + tmpf + "\n");
    if(abs(tmpf)<0.01) text( "0.00" , tmpx, tmpy );
    else{
      dec = floor(log(abs(tmpf))/log(10));
      text( nf(tmpf/pow(10.0, dec), 1, 2)+"E"+dec , tmpx, tmpy );
    }
  }
  for(i=0;i<=freqn;i++){
    if(log_bode){
      rect( lt(log10(freq[0]), log10(freq[freqn]), width/3+width_border+1, width/3+width_border+sqr_w-1, log10(freq[i]))-1 , 
      lt(log10(min_abs), log10(max_abs), border_space*2+sqr_w-1, border_space*2+1, log10(Z[i].absolute()))-1 , 2 , 2 );
    }else{
      rect( lt(freq[0], freq[freqn], width/3+width_border+1, width/3+width_border+sqr_w-1, freq[i])-1 , 
      lt(min_abs, max_abs, border_space*2+sqr_w-1, border_space*2+1, Z[i].absolute())-1 , 2 , 2 );
    }
  }
  
  // phase bode plot
  for(i=0;i<=axis_n;i++){
    // X axis
    if(log_bode) tmpf = logt( 1 , pow(10,axis_n) , min_freq , max_freq , pow(10,i) );
    else tmpf = (freq[0]*(axis_n-i)+freq[freqn]*i)/axis_n;
    tmpx = width/3*2+width_border+sqr_w/axis_n*i - 32; tmpy = border_space*2+sqr_w+17;
    if(abs(tmpf)<0.01) text( "0.00" , tmpx, tmpy );
    else{
      dec = floor(log(tmpf)/log(10));
      text( nf(tmpf/pow(10.0, dec), 1, 2)+"E"+dec , tmpx-(i==axis_n?10:0), tmpy );
    }
  }
  for(i=0;i<=9;i++){
    // Y axis
    tmpf = -10*i; tmpx = width/3*2+27; tmpy = border_space*2+4+(sqr_w-1)*(9-i)/9;
    text( nfc(tmpf,0) , tmpx, tmpy );
  }
  
  for(i=0;i<=freqn;i++){
    if(log_bode){
      rect( lt(log10(freq[0]), log10(freq[freqn]), width/3*2+width_border+1, width/3*2+width_border+sqr_w-1, log10(freq[i]))-1 , 
      lt(-90, 0, border_space*2+1, border_space*2+sqr_w-1, Z[i].phase())-1 , 2 , 2 );
    }else{
      rect( lt(freq[0], freq[freqn], width/3*2+width_border+1, width/3*2+width_border+sqr_w-1, freq[i])-1 , 
      lt(-90, 0, border_space*2+1, border_space*2+sqr_w-1, Z[i].phase())-1 , 2 , 2 );
    }
  }
  draw_ready = false;
}

void check_equation(){
  int i;
  // Get equation text update
  eqn = cp5.get(Textfield.class, "eqn_text").getText();
  eqn_n = eqn.length();
  lasteqn = eqn;
  if (calculate_eqv) {
    // Remove previous Textfields and initialize
    calculate_eqv = false;
    for(i=0;i<20;i++) is_N[i] = false;
    for (i=0; i<Z_n; i++) {
      cp5.remove(Z_name[i]);
      if (Z_name[i].charAt(0)=='Q')
        cp5.remove(Z_name[i]+"n");
    }
    for(i=0;i<20;i++) H_activate[i] = false;

    // Analyze equation text -> names and number
    Z_tmp_name = "";
    parentheses = Z_n = 0;
    for (i=0; i<eqn_n; i++) {
      if (eqn.charAt(i) == ')' && parentheses==0) break; // syntax error
      if (eqn.charAt(i) == '(') parentheses++;
      else if (eqn.charAt(i) == ')') {
        if (Z_tmp_name.length()>0) {
          if( Z_tmp_name.charAt(0)!='R' && Z_tmp_name.charAt(0)!='C' && Z_tmp_name.charAt(0)!='Z' && Z_tmp_name.charAt(0)!='Q' ) break; // syntax error
          Z_name[Z_n++] = Z_tmp_name;
          Z_tmp_name = "";
        }
        parentheses--;
      }else Z_tmp_name += eqn.charAt(i);
    }

    // Create Textfields for parameter input
    if ( parentheses==0 && i==eqn_n && i>0 && Z_n>0 ) { // Check if eqn is valid
      eqn_in_field(); // Clear Textfields
      
      textFont(createFont("Times New Roman", 30));
      int parameters_n = 0;
      float tmpleft,tmpright;
      for (i=0; i<Z_n; i++) {
        if (cp5.get(Textfield.class, Z_name[i]) == null) {
          fill(color(0));
          tmpleft = width_border + width/3*((parameters_n+1)/3);
          tmpright = 5.5*border_space + sqr_w + 5.5*border_space*((parameters_n+1)%3);
          switch(Z_name[i].charAt(0)) {
          case 'R':
            text(Z_name[i] + " (Ohm)", tmpleft, tmpright - 7);
            addTextfield(Z_name[i], tmpleft, tmpright, "100");
            break;
          case 'Z':          
            text(Z_name[i], tmpleft, tmpright - 7);
            addTextfield(Z_name[i], tmpleft, tmpright, "0.001");
            break;
          case 'Q':
            text(Z_name[i] + " (nF)", tmpleft, tmpright - 7);
            addTextfield(Z_name[i], tmpleft, tmpright, "100");
            hs[parameters_n] = new HScrollbar(tmpleft, tmpright + 45, int(sqr_w), 16, 8);
            H_activate[parameters_n] = true;
            parameters_n = parameters_n+1;
            tmpleft = width_border + width/3*((parameters_n+1)/3);
            tmpright = 5.5*border_space + sqr_w + 5.5*border_space*((parameters_n+1)%3);
            text("n(" + Z_name[i] + ")", tmpleft, tmpright - 7);
            addTextfield(Z_name[i]+"n", tmpleft, tmpright, "1");
            is_N[parameters_n] = true;
            break;
          case 'C':
            text(Z_name[i] + " (nF)", tmpleft, tmpright - 7);
            addTextfield(Z_name[i], tmpleft, tmpright, "100");
            break;
          }
          hs[parameters_n] = new HScrollbar(tmpleft, tmpright + 45, int(sqr_w), 16, 8);
          H_activate[parameters_n] = true;
          parameters_n = parameters_n+1;
        }
      }
      /*for(i=0;i<parameters_n;i++)
        print(i + ":" + (is_N[i]?'T':'F') + " ");
      print("\n");*/
      draw_ready = true;
    }
  }
}