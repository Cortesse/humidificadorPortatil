# Humidificador Ultrasónico Portátil

Sistema electrónico para el control de un humidificador ultrasónico portátil basado en un microcontrolador ESP32.
El proyecto integra el diseño de hardware, firmware y documentación técnica necesaria para el funcionamiento del dispositivo.

El sistema permite controlar el funcionamiento del humidificador, monitorear el estado de la batería y gestionar los diferentes modos de operación del equipo.

---

# Descripción del Proyecto

Este proyecto consiste en el desarrollo de la electrónica de control para un humidificador ultrasónico alimentado por batería.

El sistema utiliza un microcontrolador **ESP32** para gestionar el funcionamiento del dispositivo, incluyendo:

* Control del módulo generador de niebla (humidificador)
* Monitoreo del nivel de batería
* Gestión de estados de funcionamiento
* Visualización de información en display
* Optimización del consumo energético

El dispositivo está diseñado para operar con baterías recargables y utiliza un convertidor **DC-DC boost** para elevar el voltaje cuando es necesario para el funcionamiento del sistema.

---

# Funcionalidades

* Control del módulo humidificador
* Medición del nivel de batería
* Indicador de estado del sistema
* Modos de operación (READY / RUN)
* Conversión DC-DC para regulación de voltaje
* Gestión eficiente del consumo de energía

---

# Arquitectura del Sistema

El sistema se compone de los siguientes bloques principales:

**Batería recargable**
Fuente de alimentación principal del dispositivo.

**Convertidor DC-DC Boost**
Eleva el voltaje de la batería cuando es necesario para alimentar el sistema.

**Microcontrolador ESP32**
Encargado del control del sistema, lectura de sensores y gestión de los estados de funcionamiento.

**Módulo Humidificador Ultrasónico**
Genera la niebla mediante vibración ultrasónica.

**Sistema de medición de batería**
Permite monitorear el estado de carga de la batería mediante el microcontrolador.

**Display / interfaz de usuario**
Muestra información sobre el estado del sistema.

---

# Estructura del Repositorio

Los archivos del proyecto se organizan de la siguiente manera:

```
/humidificador
│
├── firmware/        # Código del ESP32
├── hardware/        # Esquemáticos y PCB
├── docs/            # Documentación del proyecto
├── datasheets/      # Hojas de datos de componentes
└── simulations/     # Archivos de simulación
```

---

# Hardware Principal

* Microcontrolador **ESP32**
* Convertidor **DC-DC Boost**
* Batería recargable
* Módulo humidificador ultrasónico
* Display de estado
* Circuito de medición de batería

---

# Herramientas Utilizadas

* **Arduino IDE** para el desarrollo del firmware
* Software de diseño electrónico (KiCad / EasyEDA / similar)
* Git para control de versiones

---

# Objetivo del Proyecto

Desarrollar un sistema electrónico compacto, eficiente y confiable para el control de un humidificador ultrasónico portátil, integrando hardware y firmware en una arquitectura modular y documentada.

---

# Estado del Proyecto

En desarrollo.

---

# Autores

Agostini, Emiliano
Cortesse, Agustín
