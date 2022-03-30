



# SISTEMA HETEROGÉNEO  EN SOC ZYNQ
----


## Tabla de contenidos
- [# SISTEMA HETEROGÉNEO  EN SOC ZYNQ](#-SISTEMA-HETEROGÉNEO-EN-SOC-ZYNQ)
- [Tabla de contenidos](#tabla-de-contenidos)
  - [Descripción](#descripción)
  - [Requisitos](#requisitos)
  - [Instalación y utilización](#instalación-y-utilización)
      - [Vitis HLS](#Vitis-HLS)
      - [Vivado](#Vivado)
      - [Xilinx Vitis](#Xilinx-Vitis)
  - [Contribución](#contribución)
  - [Selección de pragmas](#selección-de-pragmas)
	  - [Resultados obtenidos](#resultados-obtenidos)
	  - [Contribución](#contribución-1)
	  - [Informacion de contacto](#informacion-de-contacto)

### Descripción

Este repositorio es un tutorial para  la implementación de un co-procesador en la plataforma de desarrollo Zynq^{TM} - 7000 de la Tarea 4, asignatura IPD432 (Diseño avazanzado de sistemas digitales).	Se diseña en  FPGA el cálculo de la distancia Euclidiana entre dos vectores con High-Level-Synthesis (HLS). El procesador ARM de la Zynq-7000  cumple el rol de host que solicita el servicio
de procesamiento de vectores a un co-procesador especializado implementado en la FPGA (Programmable Logic o PL). El PS y el PL se comunican mediante un bus AXI4-Lite. El host  envía los datos al PL,  y recibe el resultado. Todas las tareas de procesamiento se implementan en el SoC, mientras que el computador externo se utiliza para configurar, depurar, y monitorear el PSa través de el protocolo de momunicación UART. La  configuración emplaeada para el PS es bare-metal.
Se presentan dos versiones del cálculo de la distancia Euclidiana. Se diseña un co-procesador paran números enteros  de 32 bits, y se modifica el diseño para otra versión de co-procesador para operar con números flotantes de precisión simple. Además,  se muestra la latencia obtenida de cada versión  desde el momento en que el procesador comienza a enviar los datos hasta que tiene el resultado final.

### Requisitos

Para reproducir los resultados obtenidos se requiere instalar los siguientes programas:
- Vitis HLS: Herramienta de síntesis de alto nivel que permite mediante funciones en C y C++ desarrollar bloques IP en RTL  para diseños de hardware en Vivado. 

- Vivado: Paquete de software  para la síntesis,  análisis  e implementación de diseños de lenguaje de descripción de hardware (HDL).

- Xilinx Vitis: Permite la programación del procesador y la importación del hardware diseñado para la lógica programable desde Vivado. En el caso de interés para la plataforma de desarrollo  Zynq^{TM} - 7000.

La versión de Xilinx Vitis, Vitis HLS y Vivado empleadas en este trabajo son del 2021.1.

IMPORTANTE: Es posible que hasta la fecha Vitis HLS tenga problemas para exportar a Bloque IP debido a un problema denominado "New Year's bug". Para solucionarlo, se deben seguir los pasos estipulados [aquí](https://support.xilinx.com/s/article/76960?language=en_US).

### Instalación y utilización

 Clonar el repositorio a un directorio. Se recomienda que la dirección del directorio  sea corta  para evitar conflictos con los programas mencionados

```
git clone https://github.com/ReyPowerLab/T4_SoC-Zynq
```


### Vitis HLS 

Crear un projecto en Vitis:

Se inicia Vitis HLS y se selecciona la opción Create Project, se le asigna un nombre  y el directorio de localización del proyecto

<center><img src="Figuras/Create_Proyect_Vitis.PNG" width="50%"></center>
      <center>This is an image</center>

Luego, en la ventana siguiente se agregan los códigos fuentes en C disponibles en repositorio: T4_SoC_int/Vitis HLS Codes/:

- EucHW.cpp
- EucHW.h
- specs

Luego se especifica el código principal y dar siguiente. En la ventana a continuación se agregan los códigos de simulación:

- EucSW.cpp
- EucSW.h
- EucTB.cpp

A continuación, se selecciona el período de 14.28 ns y la plataforma de desarrollo:


<center><img src="Figuras/Vitis_HLS_board.PNG" width="50%"></center>
      <center></center>

Con los pasos previos se tiene el proyecto creado. Luego, en el fichero specs,  definir el tipo de variables T y T2 como int para indicar que el diseño opera con números enteros. Además, indicar el valor de M con 1024. Los siguientes pasos son de simulación, síntesis y exportación a RTL. Estas opciones se encuentran en el ménú desplegable en el ícono verde de la barra de herramientas que se muestra en la figura. Además, se muestran cinco resultados de simulación. Si se desea verificar un número mayor de veces los resultados de simulación, en el archivo EucTB.cpp se puede introducir a la variable test el número de simulación deseado.


<center><img src="Figuras/Sim_Vitis_int.PNG" width="90%"></center>
      <center></center>

A continuación se muestran el resultado de timing estimado. 

<center><img src="Figuras/Timing_int_1024.PNG" width="90%"></center>
      <center></center>  

Además, se muestran  los recursos estimados, la entrada y la salida del IP diseñado.          

<center><img src="Figuras/Reporte_Vitis_HLS_int.PNG" width="70%"></center>
      <center></center> 

Finalmente, se exporta a RTL y en el directorio  del proyecto, en \solution1\impl se encuentra la carpeta comprimida export. Esta carpeta debe ser descomprimida en un directorio especificado para posteriormente emplear en Vivado.


### Vivado 

1. Crear un projecto en Vivado: 
Se inicia Vivado y se selecciona la opción Create Project, las opciones de configuración se dejan por defecto hasta llegar a la ventana para seleccionar la plataforma, en este caso se selecciona en la ventana de Boards la xc7z010clg400-1 como se muestra en la Figura:

<center><img src="Figuras/Create_Proyect_Vivado.PNG" width="70%"></center>
      <center></center>

A continuación le debe quedar un resumen de proyecto como se muestra a continuación:

<center><img src="Figuras/New Proyect Summary.PNG" width="70%"></center>
      <center></center>

2. El diseño de hardware se realiza mediante los bloques IP disponibles en Vivado y el bloque IP de la distancia euclidiana importada desde Vitis HLS. Para crear el diseño en la ventana de navegación se importa el IP personalizado seleccionando la carpeta donde se encuentra el archivo descomprimido del bloque IP diseñado en Vitis HLS. Para añadir el IP se entra en el catálogo de IP en la ventana de navegación de Vivado y mediante clic derecho se selecciona la opción add repository para seleccionar la carpeta. 

<center><img src="Figuras/add_repository.PNG" width="70%"></center>
      <center></center>

3. En Create Block Design se crea el proyecto y en la ventana de Diagram se seleccionan mediante el signo + (Add IP) los bloques:

- processing_system7_0
- 3 axi_gpio
- EucHW
- concat


Luego, se debe configurar la interface de la plataforma: 

- Clic derecho en  axi_gpio_0 y en Board Interface, en GPIO, seleccionar btns 4bits. Además, en el menú Block Properties asignar el nombre del bloque GPIO_btns.
- Clic derecho en  axi_gpio_1 y en Board Interface, en GPIO, seleccionar leds 4bits. Además, en el menú Block Properties asignar el nombre del bloque GPIO_leds.
- Clic derecho en  axi_gpio_2 y en Board Interface, en GPIO, seleccionar Custom. Además, dentro del mismo menú, en ventana IP Configuration seleccionar All Outputs y en GPIO Width asignar 2.

Nota: Los puertos con propositos personalizados se encuentran en deshuso para esta aplicación. Sin embargo, deben incluirse en el diseño pues se contemplan en la programación del procesador. La exlusión de este bloque da error para el código en Vitis. Por otro lado, se incluyeron los bloques AXI para los botones y los leds para verificar la programación del procesador.

- Luego, para configurar la frecuencia del reloj del procesador (PS) y el PL clic derecho en el bloque Zynq7 Processing System. En Clock Configuration, en Input Frecuency configurar con 50 Mhz para la frecuencia del procesador, y en PL Fabric Clocks configurar para 70 MHz para el PL.

4. Luego, mediante la asistencia de diseño seleccionar run Block Automation y Run Connection Automation. En Run Connection Automation seleccionar todos bloques. En este punto se agregan dos bloques de manera automática para ls interconexión de los perifericos y el reset del procesador. Además, se requiere verificar los nombres de los puertos del diseño. Asignar al puerto de salida axi_gpio_2 el nombre gpio_rtl, y verificar haciendo clic derecho que GPIO está en la opción Custom. Además, en IP Configuration los puertos deben estar en All Outputs, con dimensión 2. De igual manera,  verificar el bloque  GPIO_btns con la configuración del paso 3.

5. Se requiere habilitar las interrupciones para los botones y habilitar la entrada de las interrupciones en el procesador. Clic derecho en GPIO_btns y seleccionar Enable Interrupt. Luego Clic derecho en Zynq7 Processing system, en Interrupts, seleccionar Fabric Interrupts, en PL-PS Interrupts Ports seleccionar IRQ_F2P. El diseño le debe quedar como se muestra en la Figura:

<center><img src="Figuras/Block_design.PNG" width="70%"></center>
      <center></center>
Notar que el bloque concat se usa para conectar todas las interrupciones con el procesador.

6. Se valida el diseño mediante Tools/ Validate  Design. La primera es posible que vez Vivado informe peligros en el diseño. Sin embargo, puede ser omitible y desaparecen si se valida por segunda vez. 

7. Crear HDL Wrapper en la ventana de sources haciendo clic derecho en design_1 y seleccionando la opción Let Vivado manage wrapper and auto-update

8. En RTL ANALYSIS, en el Schematic, seleccionar el puerto gpio_rtl definido, luego en I/O Port Bus Properties, en I/O Ports   seleccionar en Package Pin A20 y B19 para cada puerto. Además, en I/O Std seleccionar LVCMOS33.  Luego guardar y cerrar el RTL ANALYSIS. Al guardar seleccionar un nombre para el fichero XDC.   

9. Generar el Bitstream. En este paso se debe verificar que no hayan problemas de timing. En caso de que la herrmienta informe algún problema con el timing verificar que FCLK_CLK0 es de 70 MHz en el processing_system7_0. En los experimentos realizados se aprecia que la configuración de hardware se modifica automáticamente ante algún cambio en el diseño. Por lo que a veces es necesario verificar toda la configuración realizada anteriormente.


10. Exportar el hardware con el archivo Bitstream en File/Export -> Export Hardware. Por defecto, el hardware se exporta dentro de la carpeta del proyecto con extención .xsa. Este archivo se emplea para Vitis a continuación.




1. Añadir todos los archivos fuentes al proyecto de Vivado, los cuales se encuentra dentro del directorio "IP_block_test" del mismo repositorio. Project Manager->Add Sources->Add or Create design sources. Repetir lo mismo para el archivo de constrain, seleccionando "Add or create contraints".

2. En la pestaña de Flow Navigator, dar clic a Generate Bitstream. Esto realiza la síntesis y la implementación del código previamente, para generar el archivo de configuración.

3. Generado el bitstream, en la pestaña saliente, seleccione la opción Open Hardware Manager. Establezca la conexión con la FPGA con la opcion Auto Connect, luego Program Device. El codigo se encuentra compilado en la tarjeta.

4. Abrir MATLAB y correr el código de nombre "coprocessorTesting_Oficial.m". Recuerde asignar la ubicacion de todos los archivos .m en el PATH del programa. MATLAB avisa al usuario la primera vez que se ejecuta un script. En la pestaña saliente seleccione Add to Path.	

Cabe destacar que el bloque IP ya viene en el repositorio, por lo que no se requiere utilizar el Vitis HLS. En caso de querer comprobar resultados, se deben realizar los siguentes pasos:

1. Crear un nuevo proyecto de Vitis HLS. Dentro de la pestaña al iniciar el programa, dar clic en "Create Project"
2. Se entrará a una pestaña para agregar los Design Files. Dar clic en Add Files, y añadir todo archivo .cpp y .h, con excepcion del archivo "EucTB.cpp" ya que es un archivo de simulación
3. En la siguiente pestaña para agregar testbenches, agregar el archivo faltante (EucTB.cpp)
4. En la siguiente pestaña, se mantienen la gran mayoría de opciones por defecto, con la excepción del Part Selection, la cual debe coincidir con la usada en Vivado. Dar clic en finalizar

### Xilinx Vitis 


### Contribución
Si buscas contribuir al repositorio o para realizar consultas del codigo

### Selección de pragmas
- ARRAY PARTITION: Este pragma permite particionar vectores en secciones más pequeñas. EL desarrollo de la tarea 3 considera una memoria de caracter SIPO (Singular input Parallel Output), por lo que, al querer evitar realizar cambios en la arquitectura se decide particionar los 2 vectores de forma completa, y asi lograr una implementación idéntica.

- PIPELINE: Este pragma permite segmentar las operacion realizada en el código de alto nivel. Se utiliza este pragma para lograr una implementacion con la menor cantidad de recursos posible, a cambio de una mayor latencia. Se realizan pruebas preliminares con el pragma UNROLL, que reduce la cantidad de comparaciones, y en contraste, una mejora considerable en la latencia. No obstante, la FPGA usada no posee los recursos necesarios para implementar dicha estrategia. Se utilizan los parámetros por defecto.
### Resultados obtenidos

1. Vitis HLS

Para validar el diseño realizado en vitis HLS, se debe seguir el siguiente flujo de verificación.

C Simulation -> Synthesis -> Export RTL

La cosimulacion no es considerada para el número de datos solicitado, debido a que el programa entregaba falla a pesar de estar funcionando correctamente en la FPGA, tanto para un menor número de datos. Se desconoce la razón de este falso positivo.

- Resultados simulación

Esta etapa realiza la ejecución del código presente en EUCSW.cpp y EucHW.cpp y los compara, repitiendo la prueba una determinada cantidad de veces. Un valor de retorno 0 indica simulación exitosa. Al dar clic en C simulation, con las opciones por defecto. Se obtiene el siguiente resultado en la terminal.
```

Number of errors: 0
INFO: [SIM 211-1] CSim done with 0 errors.
INFO: [SIM 211-3] *************** CSIM finish ***************
INFO: [HLS 200-111] Finished Command csim_design CPU user time: 6 seconds. CPU system time: 0 seconds. Elapsed time: 6.53 seconds; current allocated memory: 117.766 MB.
Finished C simulation.
```
No se presenta errores en las operaciones, al recibir el valor 0 como retorno de la simulación. Además se detalla el tiempo y recursos de la CPU para realizar la simulación.

- Resultado síntesis

Esto lleva a cabo la simulación para determinar la interfaz (puertos de entrada salida, forma de implementación, entre otros) y reinterpretar la funcionalidad del código C en uno de hardware. La terminal entrega la siguiente información, con respecto al proceso de sintetización:
```
INFO: [VHDL 208-304] Generating VHDL RTL for EucHW.
INFO: [VLOG 209-307] Generating Verilog RTL for EucHW.
INFO: [HLS 200-790] **** Loop Constraint Status: All loop constraints were satisfied.
INFO: [HLS 200-789] **** Estimated Fmax: 140.95 MHz
INFO: [HLS 200-111] Finished Command csynth_design CPU user time: 872 seconds. CPU system time: 6 seconds. Elapsed time: 1084.14 seconds; current allocated memory: 607.312 MB.
INFO: [HLS 200-112] Total CPU user time: 874 seconds. Total CPU system time: 7 seconds. Total elapsed time: 1086.05 seconds; peak allocated memory: 1008.943 MB.
Finished C synthesis.
```
La sintesis determina que el bucle presente cumple con los requisitos de diseño. Además se reporta los recursos que la CPU utilizó en total para realizar la operación, la frecuencia máxima de reloj en que el bloque puede teóricamente funcionar.

Tambies existe otro reporte que el programa entrega, referente a las estimaciones de rendimiento (latencia) y recursos al usar e implementar el bloque IP a generar. La información de importancia se presenta a continuación:

| Módulo && Bucles | Latencia (Ciclos) | Latencia (ns) | Pipelined | DSP | FF  | LUT  |
|------------------|-------------------|---------------|-----------|-----|-----|------|
| EucHW            | 1093              | 10930         | no        | 3   | 569 | 6965 |
| loop             | 1027              | 10270         | yes       | -   | -   | -    |


- Resultado Exportación bloque IP

Haciendo clic en Export RTL, permite generar el IP a utilizar en el bloque vivado. Debe asegurarse de tener el formato de exportación como Vivado IP. El nombre no es algo necesario de mantener, pero evita problemas con duplicados. La pestaña previa debe quedar de forma similar a la imagen:


Una vez terminada, la terminal entrega lo siguiente:
```
INFO: [Common 17-206] Exiting Vivado at Tue Mar 22 15:39:15 2022...
INFO: [HLS 200-802] Generated output file Euc_res.zip
INFO: [HLS 200-111] Finished Command export_design CPU user time: 345 seconds. CPU system time: 5 seconds. Elapsed time: 881.63 seconds; current allocated memory: 211.783 MB.
Finished Export RTL/Implementation.
```

Similar a los puntos anteriores, se reporta el tiempo requerido y los recursos usado por la CPU para realizar la operación.

2. Vivado

Al realizar la integración del bloque euclidiano con el resto del Processing Core, la implementación entrega el siguiente reporte de recursos:

| Recurso | N° en Uso | \%Uso |
|------------------------------|-------|-------|
| Slice LUTs                   | 53467 | 84.33 |
| LUT as Logic                 | 53411 | 84.24 |
| LUT as Memory                | 56    | 0.29  |
| Slice Registers              | 30895 | 24.37 |
| Register as Flip Flop        | 30895 | 24.37 |
| F7 Muxes                     | 4232  | 13.35 |
| F8 Muxes                     | 2048  | 12.92 |
| DSP                          | 3     | 1.25  |

Este reporte considera todos los módulos del Processing core en total. Para el caso particular del bloque IP, se tiene el siguiente reporte de recursos:

| Recurso    | N° en Uso |
|------------|-----------|
| Slice LUTs | 36990     |
| Flip Flops | 3978      |
| DSP        | 3         |


Con respecto al tiempo requerido para realizar proceso de síntesis e implementación, el programa tarda 09:22 y 25:16, respectivamente en cada una. Esto se observa en la pestaña de Design Runs, la cual se accede por defecto, o mediante el buscador Quick Access. Estos tiempos difieren si se tiene un computador de alta gama y si no tiene muchos procesos abiertos.


### Contribución
Si buscas contribuir al repositorio o para realizar consultas del codigo


### Informacion de contacto
[Github Profile](https://github.com/Joptis)
           jose.cayo.14@sansano.usm.cl
