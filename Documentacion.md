# Proyecto 1 - B.A.3 Parqueo Automatizado para el T3

**Curso:** Organización Computacional  
**Facultad:** Ingeniería - Escuela de Ciencias y Sistemas  
**Universidad:** Universidad de San Carlos de Guatemala  
**Catedrático:** Ing. Fernando Paz  
**Auxiliar:** Jorge De León  
**Fecha límite de entrega:** 27 de junio 2025, antes de las 23:59  

## Objetivo General

Aplicar los conocimientos teóricos aprendidos en clase magistral y laboratorio para la construcción de circuitos combinacionales y secuenciales.

## Objetivos Específicos

1. Construir un sistema que combine lógica combinacional y secuencial.
2. Aplicar conocimientos de lógica combinacional y Mapas de Karnaugh.
3. Aprender el funcionamiento de elementos electromecánicos.
4. Iniciarse en el uso de microcontroladores.
5. Diseñar un sistema óptimo utilizando la menor cantidad de dispositivos.
6. Resolver problemas mediante electrónica digital.
7. Explorar distintos usos de la lógica secuencial.

## Descripción del Proyecto

Se debe desarrollar una **maqueta funcional de un sistema automatizado de parqueo** simulando el parqueo del edificio T3 de la Facultad de Ingeniería. Se simulará un solo nivel con **4 espacios disponibles**. El sistema debe incluir:

- **Garita de entrada automatizada** con extracción de ticket y palanca motorizada (servomotor).
- **Conteo automático** de espacios disponibles y ocupados mediante sensores.
- **Control de tiempo** para ingreso y salida de vehículos (20s entrada, 30s salida).
- **Modo de seguridad** con botón de pánico y activación de alarma.
- **Frontend** con panel de estado, panel de estadísticas, gráfica en tiempo real y código de seguridad.

## Requerimientos Técnicos

### Entrada del Parqueo

- Sensor de proximidad activa el Arduino.
- Servomotor eleva la barrera por 20 segundos.

### Contador de Parqueos

- Uso de **flip-flops exclusivamente** (prohibido Arduino).
- Contadores ascendentes y descendentes controlan el número de espacios.

### Salida del Parqueo

- Sensor de proximidad y servomotor controlados por Arduino.
- Barra permanece elevada por 30 segundos.

### Seguridad

- Botón de pánico activa alarma.
- Solo se desactiva con código ingresado en el sistema principal.

## Frontend

- **Panel de estado:** muestra gráficamente los espacios libres y ocupados.
- **Botón de pánico:** activa alarma y cierra parqueo.
- **Sistema de desactivación:** ingreso de código (iniciales del equipo).
- **Estadísticas:** promedio de autos por hora y gráfica en tiempo real.

## Modos del Sistema

1. **Modo Normal:** entrada y salida habilitada.
2. **Modo Pánico:** acceso bloqueado, suena alarma.
3. **Modo Mantenimiento:** solo salida habilitada.
4. **Modo Nocturno:** sin entrada ni salida, sin alarma.
5. **Modo Evacuación:** ambas talanqueras abiertas y suena la alarma.

> Cambio entre modos especiales solo mediante retorno a modo normal.

## Metodología de Trabajo

- Trabajo en grupos asignados.
- Simulación completa en **un solo archivo .pdsprj de Proteus**.
- Dos placas físicas (contadores).
- Libre elección de lenguaje para backend y frontend.
- El proyecto debe funcionar en forma **física y encapsulada**.
- Solo se debe visualizar: controles de mando, entradas y salidas.

## Componentes Permitidos

| Componente         | Modelo       |
|--------------------|--------------|
| AND                | 7408         |
| OR                 | 7432         |
| NOT                | 7404         |
| XOR                | 7486         |
| Comparador         | 7485 / 74285 |
| Decoder            | 7447 / 7448  |
| Sumador            | 7483 / 74283 |
| Multiplexores      | 74157        |
| Demultiplexores    | 74138        |
| Flip-Flops         | Cualquiera   |
| Transistores NPN   | 2N2222       |
| Arduino            | Cualquiera   |
| Sensor de Proximidad | Cualquiera |



## Implementacion fisica contador ascendente
### 🔧 Expresiones Booleanas

### 1. D0 (entrada de FF0)
Dado que D0 = NOT(Q0):


### 2. D1 (entrada de FF1)
Dado que D1 = Q0 XOR Q1:

### Tabla de verdad del contador (de 0 a 4)

Vamos a construir la tabla con las siguientes columnas:

| (FF2) | D1 (FF1) | D0 (FF0) | Estado Siguiente (Q2 Q1 Q0) |
|-------|----------|----------|-----------------------------|
| 1     | 1        | 1        | 001                         |
| 0     | 0        | 0        | 010                         |
| 1     | 1        | 1        | 011                         |
| 0     | 0        | 0        | 100                         |
| 1     | 1        | 1        | 101 → RESET                |
| X     | X        | X        | 000                         |

### Tabla de verdad del contador (de 0 a 4)

Vamos a construir la tabla con las siguientes columnas:

| Estado Actual (Q2 Q1 Q0) | D2 (FF2) | D1 (FF1) | D0 (FF0) |
|--------------------------|----------|----------|----------|
| 000                      | 0        | 1        | 1        |
| 001                      | 0        | 0        | 0        |
| 010                      | 1        | 1        | 1        |
| 011                      | 1        | 0        | 0        |
| 100                      | 1        | 1        | 1        |
| 101                      | X        | X        | X        |

### Estados y tabla de verdad (Contador descendente de 4 a 0)

Vamos a definir los estados del contador descendente de 4 a 0:

| Estado actual (Q2 Q1 Q0) | Estado siguiente (Q2+ Q1+ Q0+) |
|--------------------------|-------------------------------|
| 100 (4)                  | 011 (3)                       |
| 011 (3)                  | 010 (2)                       |
| 010 (2)                  | 001 (1)                       |
| 001 (1)                  | 000 (0)                       |
| 000 (0)                  | 100 (4) ← reinicio cíclico    |

### Resumen de ecuaciones

- D2 = ¬Q2 ∧ ¬Q1 ∧ ¬Q0  
- D1 = (Q2 ∧ ¬Q1 ∧ ¬Q0) ∨ (¬Q2 ∧ Q1 ∧ Q0)  
- D0 = (Q2 ∧ ¬Q1 ∧ ¬Q0) ∨ (¬Q2 ∧ Q1 ∧ ¬Q0)

### Tabla de excitación para flip-flops tipo D

Para flip-flops tipo D, la entrada D = Q⁺ (el siguiente estado). Así que simplemente tomamos los valores de la columna de estado siguiente y los asignamos como entradas D.

| Q2 Q1 Q0 (actual) | D2 D1 D0 (entradas) |
|-------------------|---------------------|
| 1 0 0             | 0 1 1               |
| 0 1 1             | 0 1 0               |
| 0 1 0             | 0 0 1               |
| 0 0 1             | 0 0 0               |
| 0 0 0             | 1 0 0               |

### Expresiones adicionales

### 3. D2 (entrada de FF2)
Dado que D2 = (Q0 AND Q1) XOR Q2:


### 4. R (reset)
Dado que R = NOT(Q0 AND (NOT Q1) AND Q2):




## 💰 Presupuesto

| Concepto                       | Cantidad | Precio unitario (Q) | Subtotal (Q) |
|-------------------------------|----------|----------------------|--------------|
| Materiales maqueta            | 1        | 250.00               | 250.00       |
| Servomotores                  | 2        | 32.00                | 64.00        |
| Módulos de sensor ultrasónico | 6        | 27.00                | 162.00       |
| Flip-flops tipo D 74LS74      | 3        | 18.75                | 56.25        |
| Buzzer                        | 1        | 4.50                 | 4.50         |
| Pantalla LCD 16 pines         | 1        | 41.50                | 41.50        |
| **Total**                     |          |                      | **578.25**   |

## 👥 Participación del Equipo

| Nombre del integrante     | Carné       | Porcentaje de participación |
|---------------------------|-------------|------------------------------|
| Brandon Eduardo Pablo Garcia  | 202112092     | 100%                         |
| Michael Cristian Itzep Ixcayau  | 201610975     | 100%                         |
| Angely Lucrecia García Martínez  | 202210483    | 100%                         |

Cada integrante aporto en su totalidad a la elaboracion del proyecto final.

> Todos los integrantes contribuyeron de manera equitativa en las fases de diseño, implementación, simulación y documentación del proyecto.

## 📌 Conclusiones

1. La práctica permitió aplicar de forma efectiva los conocimientos adquiridos en clase sobre lógica combinacional y secuencial, mediante la implementación de flip-flops y mapas de Karnaugh.
2. Se comprendió el uso de sensores y microcontroladores para controlar entradas y salidas físicas como barreras y contadores, simulando un sistema automatizado real.
3. Se logró integrar elementos electrónicos, software y lógica digital en una maqueta funcional, lo cual representa un paso importante hacia proyectos de automatización más complejos.
4. El sistema de parqueo implementado no solo cumple los requisitos funcionales, sino que también contempla condiciones de seguridad y estadística, dando un enfoque integral al prototipo.
5. El uso de herramientas como Proteus y Arduino facilitó la simulación y control del sistema, mientras que el desarrollo del frontend ofreció una visualización clara del estado del parqueo.
6. El trabajo en equipo permitió una distribución equitativa de las tareas, garantizando que todos los integrantes comprendieran cada parte del proyecto.


## Documentacion

- Archivo `.pdsprj` (circuito combinacional)
- Código Arduino
- Enlace al repositorio de GitHub: `https://github.com/angelygm03/Proyecto_ORGA_G7.git`


---

