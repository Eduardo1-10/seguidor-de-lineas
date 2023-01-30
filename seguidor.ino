
const int PIN_SENSOR_IZQUIERDA = 10;
const int PIN_SENSOR_DERECHA = 11; 
const int PIN_SENSOR_CENTRO = 12;
const int PINES_MOTOR_IZQUIERDO[2] = {3,5};
const int PINES_MOTOR_DERECHO[2] = {9,6};

class Motor { 
  public: 
     Motor(const int PINES_DE_MOTOR_init[2]){
        PINES_DE_MOTOR[0] = PINES_DE_MOTOR_init[0];
        PINES_DE_MOTOR[1] = PINES_DE_MOTOR_init[1];
        PINES_DE_MOTOR_REVERSA[1] = PINES_DE_MOTOR_init[0];
        PINES_DE_MOTOR_REVERSA[0] = PINES_DE_MOTOR_init[1];
    }
    void motorAdelante(int velocidad){
        mover_motor(PINES_DE_MOTOR, velocidad);
    }
    void motorReversa(int velocidad){
        mover_motor(PINES_DE_MOTOR_REVERSA, velocidad);
    }
    void motorDetener(){
        detener_motor(PINES_DE_MOTOR);
    }
 private:
    int PINES_DE_MOTOR[2];
    int PINES_DE_MOTOR_REVERSA[2];
    
    void mover_motor(int pinesMotor[], int velocidad){
        analogWrite(pinesMotor[0], velocidad);
        analogWrite(pinesMotor[1], 0);
    }
    void detener_motor(int pinesMotor[]){
        analogWrite(pinesMotor[0], 0);
        analogWrite(pinesMotor[1], 0);
    }
};

class Sensor { 
  public: 
     Sensor(const int PIN_DE_SENSOR_init){
        PIN_DE_SENSOR = PIN_DE_SENSOR_init;
    }
    bool estaActivo(){
        return (digitalRead(PIN_DE_SENSOR) == HIGH)?true:false;
    }
 private:
    int PIN_DE_SENSOR;
};

class Seguidor { 
  public: 
     Seguidor():sensorDerecho(PIN_SENSOR_DERECHA),
                sensorIzquierdo(PIN_SENSOR_IZQUIERDA),
                sensorCentro(PIN_SENSOR_CENTRO), 
                motorIzquierdo(PINES_MOTOR_IZQUIERDO),
                motorDerecho(PINES_MOTOR_DERECHO){

                }

    void haciaAdelante(int velocidadFinal = 255){
        motorIzquierdo.motorAdelante(velocidadFinal);
        motorDerecho.motorAdelante(velocidadFinal);
    }

    void haciaAtras(int velocidadFinal = 255){
        motorIzquierdo.motorReversa(velocidadFinal);
        motorDerecho.motorReversa(velocidadFinal);
    }

    void haciaLaDerecha(){

        giro(motorDerecho, motorIzquierdo,sensorIzquierdo);
    }
    void haciaLaIzquierda(){

        giro(motorIzquierdo, motorDerecho, sensorDerecho);
    }
    void detenerMovimiento(){
        motorIzquierdo.motorDetener();
        motorDerecho.motorDetener();
    }
    bool sensorCentroActivo(){
        return sensorCentro.estaActivo();
    }
    bool sensorDerechoActivo(){
        return sensorDerecho.estaActivo();
    }
    bool sensorIzquierdoActivo(){
        return sensorIzquierdo.estaActivo();
    }
 private:
    int PIN_DE_SENSOR;
    float factorDeDisminucionEncurvas = 1.005;
    int velocidadMaxima =255;
    Sensor sensorDerecho; 
    Sensor sensorIzquierdo;
    Sensor sensorCentro; 
    Motor motorIzquierdo;
    Motor motorDerecho;
    void giro(Motor motorInterno, Motor motorExterno, Sensor sensorOpuesto){      
        int tiempo = millis();
        int tiempoIn = tiempo;
        for(int i = velocidadMaxima; i >= 0; i/=factorDeDisminucionEncurvas){
            motorInterno.motorAdelante(i);    
            motorExterno.motorAdelante(velocidadMaxima);
            tiempoIn = millis();
            if((tiempoIn - tiempo > 10) && (sensorCentro.estaActivo() || sensorOpuesto.estaActivo())){
                return;
            }
        }
        return;
    }
};

Seguidor seguidorDeLineas;

void setup() {

       /** Pines de  **** SENSORES INFRAROJOS ****  */
       pinMode(PIN_SENSOR_IZQUIERDA, INPUT); /* Pin 10 (color violeta) sensor IZQUIERDA */
       pinMode(PIN_SENSOR_CENTRO, INPUT); /* Pin 12 (color azul) sensor CENTRO */
       pinMode(PIN_SENSOR_DERECHA, INPUT); /* Pin 11 (color blanco con rayas naranja) sensor DERECHA */

       /** Pines de  **** MOTOR IZQUIERDO **** 
       *  Se polariza el pin 3 (color verde) en high y el pin 5 (color marron) low para ir hacia adelante 
       *  Se polariza el motor al contrario para ir marcha atras  
       * */
      pinMode(PINES_MOTOR_IZQUIERDO[0], OUTPUT); 
      pinMode(PINES_MOTOR_IZQUIERDO[1], OUTPUT);

       /** Pines de  **** MOTOR DERECHO **** 
       *  Se polariza el pin 9 (color naranja) en high y el pin 6 (color amarillo) low para ir hacia adelante 
       *  Se polariza el motor al contrario para ir marcha atras 
       * **/
      pinMode(PINES_MOTOR_DERECHO[0], OUTPUT);
      pinMode(PINES_MOTOR_DERECHO[1], OUTPUT);
}
void loop() {

if( seguidorDeLineas.sensorCentroActivo() &&
    !seguidorDeLineas.sensorIzquierdoActivo()&&
    !seguidorDeLineas.sensorDerechoActivo()){
        seguidorDeLineas.haciaAdelante();
        
    } 

if( seguidorDeLineas.sensorIzquierdoActivo()&&
    !seguidorDeLineas.sensorCentroActivo()&&
    !seguidorDeLineas.sensorDerechoActivo()){
        seguidorDeLineas.haciaLaIzquierda();
    } 
if( seguidorDeLineas.sensorDerechoActivo()&&
    !seguidorDeLineas.sensorCentroActivo()&&
    !seguidorDeLineas.sensorIzquierdoActivo()){
        seguidorDeLineas.haciaLaDerecha();
       
    } 

}