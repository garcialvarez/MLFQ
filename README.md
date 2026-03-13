# Simulador de planificador MLFQ (Multi-Level Feedback Queue)

Este proyecto implementa un simulador de planificador de procesos utilizando la política **Multi-Level Feedback Queue (MLFQ)** en lenguaje C. 

## Estructura del Proyecto

```text
mlfq_scheduler/
├── src/
│   ├── main.c                  # Inicialización y ejecución principal
│   ├── scheduler/
│   │   └── scheduler.c         # Lógica central del planificador y sus colas
│   └── utils/
│       └── utils.c             # Funciones para generar la tabla de métricas y exportar CSV
├── include/
│   ├── scheduler/
│   │   └── scheduler.h         # Definición de la estructura de procesos y constantes
│   └── utils/
│       └── utils.h             # Firmas de funciones de utilidad
├── assets/                     # Carpeta para recursos adicionales
├── Makefile                    # Script de compilación
└── README.md                   # Documentación principal
```

## Cómo compilar y ejecutar
Para la correcta compilación y prueba de todos los componentes, puedes usar las reglas predefinidas en el Makefile en la raíz del proyecto.

1. **Compilar** el proyecto entero (Usa C11 y flags de warning estrictos):
```bash
make
```
2. **Ejecutar** la simulación (Compilará y lanzará la prueba por defecto):
```bash
make run
```
3. **Limpiar** los objetos compilados, binarios y .csv generado en los tests:
```bash
make clean
```

## Reglas MLFQ Implementadas
1. **Selección por Prioridad**: El sistema cuenta con tres colas `Q0`, `Q1` y `Q2`. El proceso elegido será SIEMPRE el que se encuentre en la primera cola no vacía en orden de prioridad de mayor a menor.
2. **Time Slice & Round Robin**: Los procesos ejecutan un máximo del `quantum` predefinido para esta cola (Q0=2, Q1=4, Q2=8 turnos). Entre cada nivel operan usando un sistema Round Robin interno.
3. **Democión**: Si el proceso que se encuentra en ejecución agota COMPLETAMENTE su `quantum` concedido, es degradado a la siguiente estructura interna, exceptuando claro que si se encuentra en `Q2` debe permanecer ahí y volver a integrarse a su mismo Round Robin.
4. **Interrupción sin democión**: Si un proceso de alta prioridad llega al transcurso de la ejecución de uno de menor prioridad actual en CPU, este último será interrumpido, devolverá a la CPU pero re-encolado desde el inicio de SU misma cola SIN ser degradado injustamente y priorizando que el nuevo de alta prioridad entre inminentemente.
5. **Priority Boost**: Cada vez que el reloj discreto cuente (S = 20 ciclos enteros continuados) absolverá a TODOS los procesos atrapados sin completarse en cualquier cola para reordenarlos directamente a la cola origen `Q0`, garantizando nuevamente el máximo peso para todos como rebalance.

## Métricas Calculadas
La simulación almacena métricas por ejecución interna individual del ciclo, finalmente exporta sus propiedades para comprobar sus índices usando la fórmula:
* **Response Time**: `first_response_time - arrival_time`
* **Turnaround Time**: `finish_time - arrival_time`
* **Waiting Time**: `turnaround_time - burst_time`

## Ejemplo y Validación (Consola y .CSV)
Se generarán impresiones continuas al terminal ciclo a ciclo como:
```text
[Ciclo   0] -> Llega Proceso P1. Encolado en Q0.
[Ciclo   0] Ejecutando P1 en Q0 (Tiempo Restante: 8)
[Ciclo   1] -> Llega Proceso P2. Encolado en Q0.
[Ciclo   1] Ejecutando P1 en Q0 (Tiempo Restante: 7)
...
[Ciclo  20] *** PRIORITY BOOST ***: Todos los procesos se mueven a Q0
...
```
También producirá un output por CLI y el CSV de validación (`results.csv`) conteniendo el desglose general:
```csv
PID,Arrival,Burst,Start,Finish,Response,Turnaround,Waiting
1,0,8,0,23,0,23,15
2,1,4,1,14,1,13,9
3,2,9,2,26,2,24,15
4,3,5,3,21,3,18,13
```

## Análisis Académico / Preguntas 

**1. ¿Qué ocurre si el Boost es muy frecuente?**
Si el *Priority Boost* es demasiado frecuente la política pierde todo el sentido adaptativo y el balance que aporta tener niveles de prioridad. Todos los procesos regresarían a ejecutarse constantemente como si fuese una sola super-cola tipo *Round-Robin* o FIFO (anulando el castigo sobre el uso de la CPU entre interactivos y computacionales pesados).

**2. ¿Qué ocurre si no existe Boost?**
El sistema se vuelve inmediatamente injusto perdiendo total interactividad y causando que cualquier proceso pesado de CPU termine eventualmente atrapado sin recurso constante en la última cola, dando pie a sufrir un estado agudo de **Inanición o Starvation** debido a las interrupciones constantes por la llegada de nuevos perfiles de I/O que robarán la CPU desde Q0.

**3. ¿Cómo afecta un quantum pequeño en la cola de mayor prioridad?**
Obliga al ordenador a rotar frecuentemente la asignación del primer plano; y es óptimo para la capa inicial pues prioriza altamante y hace el sistema mucho más rápido visual y lógicamente para el usuario entregando atención en paralelo a tareas rápidas y penalizando prontamente perfiles acaparadores sin que saturen las colas inferiores o bloqueen las colas por largos turnos al iniciar.

**4. ¿Puede haber inanición (starvation) en MLFQ?**
En una política MLFQ clásica básica SI existe una real predisposición. Sin embargo gracias a contar con el mecanismo mitigante del *Priority Boost*, todo el esquema subsana los fallos estructurales para resolver el Starvation en nuestra variante final.
