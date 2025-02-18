/* *****************************************************************************
 * P.H.2024: TODO
 */

#include <nrf.h>
#include <board_nrf52840dk.h>

void hal_gpio_iniciar(){
  // Configure the GPIO pin for LED 1 on the nRF52840 dev kit
  // as output with standard drive strength.
  NRF_GPIO->PIN_CNF[LED_1] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

  NRF_GPIO->PIN_CNF[LED_2] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

  NRF_GPIO->PIN_CNF[LED_3] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
																	 
  NRF_GPIO->PIN_CNF[LED_4] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                                (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                                (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
}

/**
 * Los bits indicados se configuran como
 * entrada o salida según la dirección.
 */
void hal_gpio_sentido(uint32_t num_bits, uint32_t direccion){
	if (direccion == GPIO_PIN_CNF_DIR_Input) {
		NRF_GPIO->PIN_CNF[num_bits] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
	}
	else if (direccion == GPIO_PIN_CNF_DIR_Output) {
		NRF_GPIO->PIN_CNF[num_bits] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
	}								
}	


/**
 * Escribe en el gpio el valor
 */
void hal_gpio_escribir(uint32_t gpio, uint32_t valor){
	uint32_t masc = (1UL << gpio);
	
	if ((valor & 0x01) == 0) {
		NRF_GPIO->OUTCLR = masc;
	}
	else {
		NRF_GPIO->OUTSET = masc;
	}
}


/**
 * La función devuelve un entero (bool) con el valor de los bits indicados.
 */
uint32_t hal_gpio_leer(uint32_t gpio){
	uint32_t masc = (1UL << gpio);
	return ((NRF_GPIO->IN & masc)!=0);
}


/* **********************************************************************************
* MONITORESSS ***********************************************************************
* ***********************************************************************************
*/

void hal_gpio_iniciar_monitor(){
}

void hal_gpio_iniciar_monitor_unico(uint32_t id){
	NRF_GPIO->OUTSET = (1UL << id);  // Activa el monitor (procesador despierto)
}

void hal_gpio_monitor_marcar(uint32_t id){
	NRF_GPIO->OUTSET = (1UL << id);  // Activa el monitor (procesador despierto)
}

void hal_gpio_monitor_desmarcar(uint32_t id){
	NRF_GPIO->OUTCLR = (1UL << id);  // Desactiva el monitor (procesador wfi o idle)
}

