#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <xgpio.h>
#include "xparameters.h"
#include "sleep.h"
#include <math.h>

int main()
    {
        XGpio input, output;
        int button_data = 0;        // To store the value which indicates the button pressed
        int switch_data = 0;        // To store the value which indicates the switch toggled

        XGpio_Initialize(&input, XPAR_AXI_GPIO_0_DEVICE_ID);    // Initialize and store the GPIO Device ID for input
        XGpio_Initialize(&output, XPAR_AXI_GPIO_0_DEVICE_ID);   // Initialize and store the GPIO Device ID for output

        XGpio_SetDataDirection(&input, 1, 0xF);     // Set Input device as Input (0xF = Input)
        XGpio_SetDataDirection(&input, 2, 0xF);     // Set Output device as Output (0xF = Input)

        XGpio_SetDataDirection(&output, 1, 0x0);    // Set Output device as Output (0x0 = Output)

        init_platform();    // Initialze the FPGA platform for I/O

        srand(987654321);   // Initialize a random number generator
        float f1 = 5.0;
        float alpha_init, beta_init, alpha, beta, root;

        xil_printf("\n\n\r---------Generation of Qubit on the Press of a Button on Zedboard---------\n\n\r");

        while(1)
            {
                switch_data = XGpio_DiscreteRead(&input, 2);        // Read Switch Data

                XGpio_DiscreteWrite(&output, 1, switch_data);       // Write Switch Data

                button_data = XGpio_DiscreteRead(&input, 1);        // Read Button data - each button is assigned different values

                if(button_data == 0b00000)
                    {
                        // Do Nothing
                    }
                else if(button_data == 0b00001)
                    {
                        // When Buttun 0 is pressed

                        xil_printf("-----------------------------------------------------------------------------------\n\rButton 0 pressed\n\n\r");

                        xil_printf("Qubit Generation started...\n\r");

                        // Code for Generating the Qubit
                        alpha_init = (float)rand()/(float)(RAND_MAX/f1);        // Random Number for the probability amplitude of |0>
                        beta_init = (float)rand()/(float)(RAND_MAX/f1);         // Random Number for the probability amplitude of |1>

                        float insideroot = alpha_init*alpha_init + beta_init*beta_init;     // Sum of squares of random numbers
                        root = sqrtf(insideroot);                                           // Square root of the above value

                        alpha = alpha_init/root;       // Probability Amplitude of |0>
                        beta = beta_init/root;         // Probability Amplitude of |1>

                        // Code for printing the generated Qubit
                        printf("\nThe generated qubit is: \n");
                        printf("(%lf)|0> and (%lf)|1> \n\n", alpha, beta);

                        xil_printf("The generated Qubit in matrix notation is:\n\r");
                        printf("[%lf]\n", alpha);
                        printf("[%lf]\n", beta);

                        printf("The probability of the Qubit collapsing into state |0> is (%lf)\n\n", alpha*alpha);
                        printf("The probability of the Qubit collapsing into state |1> is (%lf)\n\n", beta*beta);
                        xil_printf("-----------------------------------------------------------------------------------\n\r");
                    }
                else if(button_data == 0b00010)
                    {
                        // When Button 1 is pressed

                        xil_printf("-----------------------------------------------------------------------------------\n\rButton 1 pressed\n\n\r");

                        printf("Measuring Qubit...\n\r");

                        // Code for printing the state of the measured Qubit
                        if(alpha < beta)
                            {
                                // When probability of the qubit collapsing into state |1> is higher than state |0>
                                xil_printf("\nThe Qubit collapsed into the state |1>\n\n\r");
                                xil_printf("|1> = [0]\n\r");
                                xil_printf("      [1]\n\r");
                                xil_printf("-----------------------------------------------------------------------------------\n\n\r");
                            }
                        else if(beta < alpha)
                            {
                                // When probability of the qubit collapsing into state |0> is higher than state |1>
                                xil_printf("\nThe Qubit collapsed into the state |0>\n\n\r");
                                xil_printf("|0> = [1]\n\r");
                                xil_printf("      [0]\n\r");
                                xil_printf("-----------------------------------------------------------------------------------\n\n\r");
                            }
                    }

                usleep(200000);
            }
        cleanup_platform();
        return 0;
    }
