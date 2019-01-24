Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does a single LED draw when the output drive is set to "Strong" with the original code?**

**Ans-**
* Current Consumption for both LED and CPU (observing the LED on time in Energy profiler) : **5.01mA**

* Current Consumption for just the CPU i.e LED off (observing the LED off time in Energy Profiler) : **4.51 mA**
      
* Therefore, the current consumed by the LED + Resistor is (5.01 - 4.51 )mA = **0.50 mA**
      

**2. After commenting out the standard output drive and uncommenting "Weak" drive, how much current does a single LED draw?**

**Ans-**
* Current Consumption for both LED and CPU (observing the LED on time in Energy profiler) : **5.00mA**

* Current Consumption for just the CPU i.e LED off (observing the LED off time in Energy Profiler) : **4.50 mA**
  
* Therefore, the current consumed by the LED + Resistor is (5.01 - 4.51 )mA = **0.50 mA**

**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, 
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

**Ans**- 
No, there is no meaningful difference between the answers for question 1 and 2. Infact, in my case both of the values are exactly samei.e *0.50mA* . 
* From the schematic , we observe that 2 LED's are at 2 different GPIO pins of Port F.

* Current shown by energy profiler is the combination of current drawn by both LED and resistor.

* According to the file *em_gpio.h* the current supplied by the gpio in weak drive is 1mA and in strong drive is 10mA. In both the cases the current required by the LED to glow is satisfied and hence we donot see any changes comparing both these cases.

**4. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with only LED1 turning on in the main loop?**

**Ans**-
* Average current with only LED1 turning on : 4.84 mA

* Average energy with only LED1 turning on : 12.03 micro Wh

**5. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with both LED1 and LED0 turning on in the main loop?**

**Ans**-
* Average current with both LED's turning on : 5 mA

* Average energy with both LED's turning on : 12.52 micro Wh
