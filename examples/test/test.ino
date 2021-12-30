// Define Ovoo Libraries
#include "Telit_xE910.h"

void setup() {

    GSM.Power_ON(false, false, true);

}

void loop() {

    GSM_AT.ATE(0);
  
}
