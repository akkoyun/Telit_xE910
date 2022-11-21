// Define Arduino Library
#ifndef __Arduino__
	#include <Arduino.h>
#endif

// Define Hardware Configuration
#include "Hardware_Config.h"

// Modem Hardware Class
class Hardware {

	private:

		/**
		 * @brief Enable Communication Buffer.
		 * @param _Function 
		 * _GET_ (0) : Get Buffer State.
		 * _SET_ (1) : Set Buffer State.
		 * @param _State 
		 * true : Enable Buffer.
		 * false : Disable Buffer.
		 * @return true Buffer is Enabled.
		 * @return false Buffer is Disabled.
		 */
		void Communication(const bool _State) {

			// Set Communication
			if (_State) {

				// Set Communication Signal
				PORTJ &= 0b11101111;

				// Set Variable
				this->Status.Communication = true;
	
			} else {

				// Set Communication Signal
				PORTJ |= 0b00010000;

				// Set Variable
				this->Status.Communication = false;

			}

		}

		/**
		 * @brief Power Switch
		 * @param _State Switch State
		 */
		void Power_Switch(const bool _State) {

			// Set Power Switch
			if (_State) {

				// Set GSM Power Switch Signal
				PORTH |= 0b00000100;

				// Set Variable
				this->Status.Power_Switch = true;

			} else {

				// Set GSM Power Switch Signal
				PORTH &= 0b11111011;

				// Set Variable
				this->Status.Power_Switch = false;

			}
		
		}

		/**
		 * @brief LED Power
		 * @param _State LED State
		 */
		void LED(const bool _State) {

			// Set LED Status
			if (_State) {

				// Set GSM LED Signal
				PORTH &= 0b11101111;

				// Set Variable
				this->Status.LED = true;

			} else {

				// Set GSM LED Signal
				PORTH |= 0b00010000;

				// Set Variable
				this->Status.LED = false;

			}

		}

		/**
		 * @brief On or Off Modem.
		 * @param _Time On/Off Time
		 */
		void OnOff(void) {

			// Set On/Off Signal HIGH [PJ6]
			PORTJ |= 0b01000000;

			// Command Delay
			for (uint8_t i = 0; i < 100; i++) {

				// Calculate Delay (2000)
				uint8_t _Delay = GSM_Modem_On_Time / 100;

				// Terminal Bar
				#ifdef GSM_Debug
					Terminal_GSM.Text(GSM_Boot_Bar_X, GSM_Boot_Bar_Y + i, WHITE, F("▒"));
				#endif

				// Wait
				delay(_Delay); 

			}

			// Set On/Off Signal LOW [PJ6]
			PORTJ &= 0b10111111;

			// Clear Bar
			#ifdef GSM_Debug
				for (uint8_t i = 0; i < 100; i++) Terminal_GSM.Text(GSM_Boot_Bar_X, GSM_Boot_Bar_Y + i, WHITE, F(" "));
			#endif

		}

		/**
		 * @brief ShutDown Modem
		 * @param _Time ShutDown Time
		 */
		void ShutDown(void) {

			// Set Shut Down Signal HIGH [PJ5]
			PORTJ |= 0b00100000;

			// Command Delay
			delay(GSM_Modem_SDown_Time);

			// Set Shut Down Signal LOW [PJ5]
			PORTJ &= 0b11011111;

		}

	public:

		// Define Modem Hardware Status
		struct Struct_Status {
			bool Communication = false;
			bool Power_Switch = false;
			bool LED = false;
			bool Power = false;
			bool Power_Monitor = false;
		} Status;

		/**
		 * @brief Get Power Monitor
		 * @return true Modem Powered
		 * @return false Modem is not Powered
		 */
		bool PowerMonitor(void) {

			// Control for PWMon (PJ3)
			if ((PINJ & (1 << PINJ3)) == (1 << PINJ3)) {

				// Response Delay
				delay(5);

				// Set Variable
				this->Status.Power_Monitor = true;

				// Power Monitor 3V3 HIGH
				return (true);

			} else {

				// Response Delay
				delay(5);

				// Set Variable
				this->Status.Power_Monitor = false;

				// Power Monitor 3V3 LOW
				return (false);

			}

		}

		/**
		 * @brief Power on Sequence of Modem
		 * @param _Power_Switch Power Switch State
		 * @param _LED_Switch  LED Stat
		 * @param _Communication_Switch Communication State
		 * @return true Modem is ON
		 * @return false Modem is OFF
		 */
		bool ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

			// Send Shut Down Signal
			this->ShutDown();

			// Enable GSM Modem Power Switch
			if (_Power_Switch) this->Power_Switch(Enable);  
			
			// Enable GSM Modem LED Feed
			if (_LED_Switch) this->LED(Enable);

			// Set Communication Signal LOW
			if (_Communication_Switch) this->Communication(Enable);
			
			// Turn On Modem
			if (this->PowerMonitor()) {

				// End Function
				return (true);

			} else {

				// Send On Off Signal
				this->OnOff();

				// Control for PWMon (PH7)
				if (this->PowerMonitor()) {

					// End Function
					return(true);

				} else {

					// Send Shut Down Signal
					this->ShutDown();

				}

			}

			// End Function
			return (false);

		}

		/**
		 * @brief Power on Sequence of Modem
		 * @param _Power_Switch Power Switch State
		 * @param _LED_Switch  LED Stat
		 * @param _Communication_Switch Communication State
		 * @return true Modem is OFF
		 * @return false Modem is ON
		 */
		bool OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

			// Turn Off Modem
			if (this->PowerMonitor()) this->ShutDown();

			// Disable GSM LED Power
			if (_LED_Switch) this->LED(Disable);

			// Disable GSM Modem Voltage Translator
			if (_Communication_Switch) this->Communication(Disable);

			// Disable GSM Modem Main Power Switch
			if (_Power_Switch) this->Power_Switch(Disable);

			// Command Delay
			delay(1000);

			// End Function
			return (true);

		}

};
