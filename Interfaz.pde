import processing.serial.*;

Serial myPort;  // Create object from Serial class

Button on_button;  // Lampara 1
Button on_button2;  // Lampara 2
Button on_button3;  // Lampara 3
Button on_button4;  // Panico
Button on_button5;  // Panico
Button on_button6;  // Panico

int lectura_on = 0;



int clk = 0;       // number of times the button is clicked
int clk2 = 0;
int clk3 = 0;
int clk4 = 0;
int clk6 = 0;


int estado_app = 0;

String card;
String message;

void setup() {
  size (600, 480);
  smooth();
  // create the button object
  on_button = new Button("Led_1", 20, 20, 100, 50);
  on_button2 = new Button("Led_2", 20, 90, 100, 50);
  on_button3 = new Button("Led_3", 20, 160, 100, 50);
  on_button4 = new Button("¡Panico!", 20, 230, 100, 50);
  
  on_button5 = new Button("Activar Modulo de Tarjetas", 400, 20, 170, 50);
  on_button6 = new Button("Lectura Tarjeta", 400, 90, 120, 50);

  myPort = new Serial(this, "COM8", 9600);

}

void draw() {
  // draw a square if the mouse curser is over the button
  if (on_button.MouseIsOver()) {
  }
  else {
    // hide the square if the mouse cursor is not over the button
    background(255);
  }
  // draw the button in the window
    text("Operaciones con Leds", 75, 5);
  on_button.Draw();
  on_button2.Draw();
  on_button3.Draw();
  on_button4.Draw();
  on_button5.Draw();
  
  if(lectura_on == 1)
  {
      on_button6.Draw();
  }

  
}

// mouse button clicked
void mousePressed()
{
  if (on_button.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    print("Estado Led 1: ");
    if(clk4 != 1){
    clk++;
    clk = clk > 1 ? 0:clk;
    char envio = clk == 1 ? 'A':'B';
    myPort.write(envio);         //send a 1
    }
    println(clk);


  }
  
  if (on_button2.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    print("Estado Led 2: ");
    if(clk4 != 1){
      clk2++;
      clk2 = clk2 > 1 ? 0:clk2;
      char envio = clk2 == 1 ? 'C':'D';
      myPort.write(envio);         //send a 1
    }
    println(clk2);
    
  }
  
  if (on_button3.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
     print("Estado Led 3: ");
    if(clk4 != 1){
    clk3++;
    clk3 = clk3 > 1 ? 0:clk3;
    char envio = clk3 == 1 ? 'E':'F';
    myPort.write(envio);         //send a 1
    }
    println(clk3);
  }
  
   if (on_button4.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    print("Estado Panico: ");
    clk4++;
    clk4 = clk4 > 1 ? 0:clk4;
    println(clk4);
    char envio = clk4 == 1 ? 'G':'H';
    myPort.write(envio);         //send a 1
  }
  
   if (on_button5.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
    println("Activar Modulo Tarjeta");
    estado_app++;
    estado_app = estado_app > 1 ? 0 : estado_app;
    println(estado_app);
    char envio = estado_app == 1 ? 'I':'J';
    myPort.write(envio);         //send a 1
    lectura_on = estado_app == 1 ? 1:0;
  }
  
   if (on_button6.MouseIsOver()) {
    // print some text to the console pane if the button is clicked
     print("Lectura Tarjeta: ");
    clk6++;
    clk6 = clk6 > 1 ? 0:clk6;
    char envio = clk6 == 1 ? 'A':'B';
    myPort.write(envio);         //send a 1
    println(clk6);
  }
}

// the Button class
class Button {
  String label; // button label
  float x;      // top left corner x position
  float y;      // top left corner y position
  float w;      // width of button
  float h;      // height of button
  
  // constructor
  Button(String labelB, float xpos, float ypos, float widthB, float heightB) {
    label = labelB;
    x = xpos;
    y = ypos;
    w = widthB;
    h = heightB;
  }
  
  void Draw() {
    fill(218);
    stroke(141);
    rect(x, y, w, h, 10);
    textAlign(CENTER, CENTER);
    fill(0);
    text(label, x + (w / 2), y + (h / 2));
  }
  
  boolean MouseIsOver() {
    if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) {
      return true;
    }
    return false;
  }
}

void serialEvent(Serial p) {
  // get message till line break (ASCII > 13)
  message = myPort.readStringUntil(13);
  if(message != null){
    println(message);
  }
}
