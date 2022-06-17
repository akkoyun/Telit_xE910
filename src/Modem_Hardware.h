#ifndef __Modem_Hardware__
#define __Modem_Hardware__

// Define Arduino Library
#ifndef __Arduino__
#include <Arduino.h>
#endif

// Modem Hardware Class
class Modem_Hardware {

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

			// Enable Communication 
			if (_State) {

				// Set Port				
				PORTJ &= 0b11101111;

			}

			// Disable Communication
			if (!_State) {

				// Set Port
				PORTJ |= 0b00010000;

			} 

		}

		/**
		 * @brief On or Off Modem.
		 * @param _Time On/Off Time
		 */
		void OnOff(const uint16_t _Time) {

			// Set On/Off Signal HIGH [PJ6]
			PORTJ |= 0b01000000;

			// Command Delay
			for (uint8_t i = 0; i < 100; i++) {

				// Calculate Delay (2000)
				uint8_t _Delay = _Time / 100;

				// Terminal Bar
				#ifdef GSM_Debug
					Terminal.Text(Debug_Boot_X, Debug_Boot_Y + i, WHITE, F("▒"));
				#endif

				// Wait
				delay(_Delay); 

			}

			// Set On/Off Signal LOW [PJ6]
			PORTJ &= 0b10111111;

			// Clear Bar
			#ifdef GSM_Debug
				for (uint8_t i = 0; i < 100; i++) Terminal.Text(Debug_Boot_X, Debug_Boot_Y + i, WHITE, F(" "));
			#endif

		}

		/**
		 * @brief ShutDown Modem
		 * @param _Time ShutDown Time
		 */
		void ShutDown(const uint16_t _Time) {

			// Set Shut Down Signal HIGH [PJ5]
			PORTJ |= 0b00100000;

			// Command Delay
			delay(_Time);

			// Set Shut Down Signal LOW [PJ5]
			PORTJ &= 0b11011111;

		}

		/**
		 * @brief Power Switch
		 * @param _State Switch State
		 */
		void Power_Switch(const bool _State) {

			// Set GSM Power Enable
			if (_State) {
				
				// Set Port				
				PORTH |= 0b00000100;

			}

			// Set GSM Power Disable
			if (!_State) {

				// Set Port
				PORTH &= 0b11111011;
			
			}
		
		}

		/**
		 * @brief LED Power
		 * @param _State LED State
		 */
		void LED(const bool _State) {

			// Set GSM LED Power Enable
			if (_State) {
				
				// Set Port
				PORTH &= 0b11101111;
				
			}

			// Set GSM LED Power Disable
			if (!_State) {

				// Set Port
				PORTH |= 0b00010000;
				
			}

		}

	public:

		/**
		 * @brief Get Power Monitor
		 * @return true Modem Powered
		 * @return false Modem is not Powered
		 */
		bool PowerMonitor(void) {

			// Control for PWMon (PJ3)
			if ((PINJ & (1 << PINJ3)) == (1 << PINJ3)) {

				// Response Delay
				delay(10);

				// Power Monitor 3V3 HIGH
				return (true);

			} else {

				// Response Delay
				delay(10);

				// Power Monitor 3V3 LOW
				return (false);

			}

		}

		/**
		 * @brief Power on Sequence of Modem
		 * @param _Power_Switch Power Switch State
		 * @param _LED_Switch  LED Stata
		 * @param _Communication_Switch Communication State
		 * @return true Modem is ON
		 * @return false Modem is OFF
		 */
		bool ON(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

			// Send Shut Down Signal
			this->ShutDown(500);

			// Enable GSM Modem Power Switch
			if (_Power_Switch) this->Power_Switch(true);  
			
			// Enable GSM Modem LED Feed
			if (_LED_Switch) this->LED(true);

			// Set Communication Signal LOW
			if (_Communication_Switch) this->Communication(true);
			
			// Turn On Modem
			if (this->PowerMonitor()) {

				// End Function
				return (true);

			} else {

				// Send On Off Signal
				this->OnOff(5000);

				// Control for PWMon (PH7)
				if (this->PowerMonitor()) {

					// End Function
					return(true);

				} else {

					// Send Shut Down Signal
					this->ShutDown(200);

				}

			}

			// End Function
			return (false);

		}

		/**
		 * @brief Power on Sequence of Modem
		 * @param _Power_Switch Power Switch State
		 * @param _LED_Switch  LED Stata
		 * @param _Communication_Switch Communication State
		 * @return true Modem is OFF
		 * @return false Modem is ON
		 */
		bool OFF(const bool _Power_Switch, const bool _LED_Switch, const bool _Communication_Switch) {

			// Turn Off Modem
			if (this->PowerMonitor()) this->ShutDown(200);

			// Disable GSM LED Power
			if (_LED_Switch) this->LED(false);

			// Disable GSM Modem Voltage Translator
			if (_Communication_Switch) this->Communication(false);

			// Disable GSM Modem Main Power Switch
			if (_Power_Switch) this->Power_Switch(false);

			// Command Delay
			delay(1000);

			// End Function
			return (true);

		}

};

#endif /* defined(__Modem_Hardware__) */