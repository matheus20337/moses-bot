/*
                                            MosesBot Mk.I
                               Hardstone Industries, Anno Domni MMXXVI
                                        @@@@@@@@@@@@@@@@@@@@                                        
                                 @@@@@@......................@@@@@@                                 
                             @@@@................@@..@@............@@@@                             
                         @@@.............@@.....@.......................@@@                         
                      @@@...........@.....@.....@.......@@@@...............@@@                      
                   @@@........@..@........................@..@...@.@..........@@@                   
                 @@............@..@....@@@@@@@@@@@@@@@@@@@@@@...@................@@                 
               @@........@....@...@@@@@#.....:--------:.....#@@@@@..@>....@........@@               
             @@.........@.@@..@@@@#..:------------------------:..#@@@@...@...........@@             
            @@........@.@..@@@{..:--------------------------------:..{@@@.@@..........@@            
          @@.......@.....@@}..----------------------------------------..}@@.............@@          
         @@......@.....@@{.:-......:------------------------------........{@@.@..@@......@@         
        @............@@{.::..*@@@@@...:------------------------:...@@@@@....{@@.@..@.......@@       
      @@...........@@}.:-:.@@@@@<@@@@@.:----------------------:.@@@@@^@@@@@...}@@...@@......@@      
     @@...........@}.:--:.@@@.......@@@.----------------------.@@@.......@@@.-:.}@..@........@@     
    @@...........@#:----.@@@.........@@.---------------------:>@@.........@@.---:#@...........@@    
    @...........@#:-----.@@@.........@@.---------------------::@@.........@@.----:#@...........@    
   @@..........@#:------:.@@@.......@@@.----------------------.@@@.......@@@.-----:#@..........@@   
  @@..........@#:--------:.@@@@@@@@@@..:----------------------:.@@@@@@@@@@..:------:#@..........@@  
  @..........@}:----------:...@@@@...:--:......:---------------:...]@@@...:---------:}@..........@  
 @@.........@@.--------------:....:----.{@@@@@@.:-----------------:....:-------------.@@.........@@ 
 @..........@.:-----------------------.)@.....@@.------------------------------------:.@..........@ 
 @.........~@.------------------------.@@.....@@....:---------------------------------.@@.........@ 
@@.........@::------------------------:.@@...@@]@@@@....:-----------------------------:.@.........@@
@@.........@.--------------------------:.]@@@#......@@@@+....:-------------------------.@.........@@
@...@......@.--------------------------------------:....{@@@@....:---------------------.@..........@
@@@@...@@@@@.------------------------------------------:.....@@@@-...:-----------------.@@@@...@@@@@
@...@......@.-----------------------------------------------.....@@@@=-----------------.@..........@
@@.........@.------------------------------------------:....-@@@@....:-----------------.@.........@@
@@.........@::-------------------------------------:....@@@@@....:--------------------:.@.........@@
 @.........+@.-------------------------:.#@@@#......@@@@.....:------------------------.@@.........@ 
 @..........@.:-----------------------:.@@...@@.@@@@....:----------------------------:.@..........@ 
 @@.........@@.-----------:..:--------.@@.....@@....:-----------------:..:-----------.@@.........@@ 
  @..........@}:-------:.]@@@{.:------.=@.....@@.-------------------:.{@@@].:-------:}@..........@  
  @@..........@#:------.@@...@@@:------.{@@@@@@.:------------------:@@@...@@.------:#@..........@@  
   @@..........@#:-----.@......@.-------:......:-------------------.@......@.-----:#@..........@@   
    @......@@...@#:----.@.....@@.----------------------------------.@@.....@.----:#@...........@    
    @@..@........@#:---:}@@@@@@{..:--------------------------------..@@@@@@{:---:#@...........@@    
     @@..@.....@..@}.:--:......=@@..:----------------------------:..@%.....:--:.}@...........@@     
      @@.....@.....@@}.:-------:.@%@..............................@@..:-----:.}@@....@......@@      
        @......@@....@@{.:------:..@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@).:-----:.{@@......@..@..@@       
         @@.@@..@@.....@@{.:------:...............................:-----:.{@@......@.....@@         
          @@@.@..@.@.....@@}..----------------------------------------..}@@..........@..@@          
            @@..@..@...@...@@@{..:--------------------------------:..{@@@..........@..@@            
             @@......@........@@@@#..:------------------------:..#@@@@.......@.@.@...@@             
               @@.......@@........@@@@@#.....:--------:.....#@@@@@...........@..@..@@               
                 @@...@....@@..........@@@@@@@@@@@@@@@@@@@@@@............@.......@@                 
                   @@@......@@@........................................@..@@@.@@@                   
                      @@@...@@.@.@@.@...........................@.@.....@..@@@                      
                         @@@....@...@.@.@.@@@.........@...@...@..@.@.@..@@@                         
                             @@@@.......@.....@@......@......@.@...@@@@                             
                                 @@@@@@....@@.@.@............@@@@@@                                 
                                        @@@@@@@@@@@@@@@@@@@@                                        
*/

/* Código baseado no exemplo de controle da biblioteca Bluepad32 */

#include <Arduino.h>
#include <Bluepad32.h>
#include <uni.h>


/* === Funções de movimento === */
static const uint8_t IN1 = 13;
static const uint8_t IN2 = 25;
static const uint8_t IN3 = 33;
static const uint8_t IN4 = 27;
/* As entradas IN1 - IN4 são
 * acionadas de acordo com as
 * conexões físicas do motor com
 * a ponte H.
 */
void go_forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void go_left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void go_right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void go_backward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void halt_movement() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void emergency_stop() {
  halt_movement();
}

/* ========================================================================================== */
/* MAC do controle é hardcodado porque não se espera que mudemos o controle. */
static const char *controller_mac = "03:21:05:26:0F:C3";
ControllerPtr controller = nullptr;
static const char *VERSION = "0.0.2";

/* Este callback é chamado sempre que um novo controle conectar.*/
void onConnectedController(ControllerPtr ctl) {
  /* Como filtramos para escanear somente o 
   * MAC do controle no setup(), aceitamos 
   * qualquer controle aqui.
   */
   if (controller == nullptr) {
    ControllerProperties properties = ctl->getProperties();
    Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(),
      properties.vendor_id, properties.product_id);
    controller = ctl;
   }
}

/* Este callback é chamado sempre que o controle desconectar. */
void onDisconnectedController(ControllerPtr ctl) {
  Serial.printf("Controller disconnected.");
  controller = nullptr;
  /* Importante: Se paramos os motores se o robô perder conexão com o controle. */
  emergency_stop();
}

/* Função de debug.
 * Atualmente inutilizada.
 */
void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}

/* Esta função é chamada quando recebemos algo do controle */
/* O código é auto explicativo */
void processGamepad(ControllerPtr ctl) {
  switch(ctl->dpad()) {
    case DPAD_UP:
      go_forward();
    break;
    case DPAD_DOWN:
      go_backward();
    break;
    case DPAD_LEFT:
      go_left();
    break;
    case DPAD_RIGHT:
      go_right();
    break;
    default:
      halt_movement();
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {
  /* Configuração de pinos */
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  /* Começamos com o robô parado */
  emergency_stop();
  
  /* Configura a biblioteca de comunicação para
   * utilizar nossos callbacks. */
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  //BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);

  /* Definimos um filtro para procurar somente o nosso controle. */
  bd_addr_t controller_addr;
  sscanf_bd_addr(controller_mac, controller_addr);
  uni_bt_allowlist_add_addr(controller_addr);
  uni_bt_allowlist_set_enabled(true);


  /* Mensagem de debug */
  Serial.println(" __  __                       __  __ _      ___ ");
  Serial.println("|  \\/  | ___  ___  ___  ___  |  \\/  | | __ |_ _|");
  Serial.println("| |\\/| |/ _ \\/ __|/ _ \\/ __| | |\\/| | |/ /  | | ");
  Serial.println("| |  | | (_) \\__ \\  __/\\__ \\ | |  | |   < _ | | ");
  Serial.println("|_|  |_|\\___/|___/\\___||___/ |_|  |_|_|\\_(_)___|");
  Serial.printf("Firmware: %s\n", VERSION);
  Serial.printf("Bluepad32 Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
  Serial.println("Initialization complete.");
  Serial.println("Unreleased development build. Unauthorized distribution will trigger the self-destruct mechanism of the robot.");
  Serial.println("Have a very safe day.");
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    /* Chama nossa função de controle se recebermos
     * dados novos do controle. */
    bool dataUpdated = BP32.update();
    if (dataUpdated && controller != nullptr)
        processGamepad(controller);

    delay(150);
}
