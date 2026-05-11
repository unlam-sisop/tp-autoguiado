# Trabajo práctico autoguiado de Sistemas Operativos
**Fecha de publicación:** 2026.05.11

## Objetivo

El objetivo de esta actividad es ver en funcionamiento las diferentes herramientas del sistema operativo para la sincronización y comunicación de procesos pesados y livianos (threads), así como la creación de dichos procesos.

La actividad será autoguiada y al finalizar cada una de las secciones se presentarán una serie de preguntas que deberán ser contestadas y entregadas al finalizar la actividad.

## Requisitos

Para poder desarrollar esta actividad se necesitará una computadora con:

* Sistema operativo Linux o Windows con WSL versión 2 instalado y funcionando. Toda la actividad se desarrollará sobre Linux/WSL2. No se ejecutarán procesos sobre Windows.
* Editor de texto. Se recomienda Visual Studio Code.
* Herramientas para la compilación de C++ instaladas sobre Linux. Se pueden instalar con el comando:

```bash
sudo apt install build-essential
```

## Aclaraciones

* Para simplificar la terminología dentro de esta guía:
  * Cuando se menciona la palabra “proceso” siempre será un proceso pesado.
  * Cuando se menciona la palabra “thread” siempre será un hilo o proceso liviano.
* Todos los comandos que se indican en la guía son para ejecutarlos en una terminal que utiliza bash.
* Todos los puntos arrancan con uno o más archivos de código que se pueden descargar desde el repositorio de GitHub `unlam-sisop/tp-autoguiado`.

## Punto 1: Creación de procesos y competencia por recursos

### Objetivo

Comprender el mecanismo de bifurcación de procesos mediante la llamada al sistema `fork()`, analizando la herencia de descriptores de archivos y la competencia por el acceso a recursos compartidos (Race Conditions).

### Pasos

1. Descargamos el archivo `1-fork.c` del repositorio.
2. Compilamos el programa con:

```bash
make fork
```

3. Ejecutamos el programa con:

```bash
./1-fork
```

4. En otra terminal, ejecutamos los siguientes comandos y analizamos los resultados:

```bash
ps -fu $USER
pstree -p $USER
ps x -fu $USER --forest
```

### Para debatir

* ¿Cuántos procesos se generan y en qué estados se encuentran?
* ¿Qué proceso es el padre del proceso del ejercicio que pusieron a ejecutar?
* Se identifica un proceso como demonio, ¿cuál es la diferencia que tiene con los otros procesos?
* Ejecutar de nuevo varias veces, ¿la salida es siempre la misma?

## Punto 2: Creación de threads

### Objetivo

Crear un proceso que cree threads que muestren por pantalla un número aleatorio.

### Pasos

1. Compilamos el programa con:
```bash
make threads
```

2. Ejecutamos el programa con:
```bash
./2-threads
```

3. En otra terminal, ejecutamos:
```bash
ps -fLC 2-threads
```

4. Analizar:
    * ¿Qué datos se pueden ver en la salida del comando?
    * ¿A qué tipo de thread se parecen?
    * ¿Por qué todos tienen el mismo PID?

5. Terminamos la ejecución del proceso con `Ctrl+C`.
6. Modificamos el código para que cada thread realice 30 impresiones por pantalla, reemplazando el `while` de `trabajoThread` por un `for`.
7. Volvemos a compilar y ejecutar el programa como se hizo en los pasos anteriores.
8. Ejecutar el programa varias veces (no hace falta recompilar). Analizar:
   * ¿En qué orden se imprimen los textos en la terminal?
   * ¿Es siempre el mismo?
   * ¿Quedan todas las líneas con el texto correcto o se mezclan los textos?
   * ¿Por qué pasa esto?
   * ¿Qué propuestas tienen para solucionar este problema?

9. Agregamos un mutex para sincronizar la salida por pantalla, que en este caso es nuestro recurso compartido. Analizar:
    * ¿Cómo se relacionan los métodos `lock` y `unlock` de `mutex` con las primitivas vistas en la teoría de la materia?

10. Recompilamos y volvemos a ejecutar el programa.
    * ¿Se solucionaron los problemas de salida por pantalla?
    * ¿Cómo identificar que el proceso tiene threads y cuántos son?
    * ¿Hay alguna jerarquía en los threads como con los procesos?

## Punto 3: Uso de semáforos para sincronizar procesos y threads

### Objetivo

Entender el motivo de necesitar sincronizar procesos o threads con semáforos. Visualizar los recursos utilizados.

El proceso lanza unidades de procesamiento que hacen una iteración de 100.000 veces sumando 1.

### Pasos

1. Compilamos el programa con:

```bash
make semaforos
```

2. Ejecutamos el programa con:

```bash
./3-semaforos
```

3. Elegir una opción del menú:
   * Procesos sin sincronización
   * Procesos con semáforo POSIX
   * Threads sin sincronización
   * Threads con mutex

4. Ejecutar varias veces cada una de las variantes, prestando atención al valor esperado y el valor real obtenido en la ejecución.
5. En el caso de utilizar semáforos POSIX, antes de finalizar el programa ejecutar en otra terminal:

```bash
ls -l /dev/shm
```

### Para debatir

* ¿Se mantiene el valor entre las diferentes ejecuciones del proceso?
* ¿Y el orden en que van ejecutando los procesos o threads?
* ¿Qué problemas puede traer no eliminar los recursos utilizados por el proceso (semáforos en este caso)?
* ¿Cómo puedo identificar que un proceso está utilizando semáforos?

Investigar los comandos:

```bash
ipcs
ls -l /dev/shm
lsof
fuser
```

## Punto 4: Comunicación entre procesos con memoria compartida

### Objetivo

Trabajar con procesos que se comunican por memoria compartida (`shm`) y comprender su funcionamiento. Visualizar los recursos utilizados.

Es un clásico modelo de productor-consumidor, con un productor y dos consumidores.

### Pasos

1. Ejecutar para realizar la compilación de los programas productor y consumidores.
```bash
make shm
```

2. Cada uno en una terminal separada, ejecutar:
    * Una instancia del productor:
    ```bash
    ./4-shm-productor
    ```
    * Una instancia de cada consumidor:

```bash
./4-shm-consumidor1
./4-shm-consumidor2
```

3. Volver a ejecutar los consumidores.

4. Ejecutar dos veces el mismo consumidor.

5. Detener el proceso productor con `Ctrl+C`.

6. Ejecutar cada uno de los consumidores.

7. Ejecutar dos instancias del productor en distintas terminales, y luego los consumidores.

### Para debatir

* ¿Cómo puedo identificar que un proceso está utilizando memoria compartida?

Investigar los comandos:

```bash
ipcs
ls -l /dev/shm
lsof
fuser
ipcrm
```

* ¿Cuántos segmentos de memoria compartida se están creando y de qué tipos?
* ¿Qué pasa con las memorias compartidas si matamos el proceso productor?
* ¿Se pueden eliminar los segmentos? ¿Cómo debería comportarse el proceso para que esto no suceda?
* Si los procesos no están emparentados, ¿cómo pueden identificar el mismo segmento de memoria para poder vincularse?
* ¿Estos procesos están sincronizados o puede darse una condición de carrera?
* ¿Qué sucede si se ejecuta dos veces el Consumidor1 o el Consumidor2?

## Punto 5: Comunicación entre procesos utilizando sockets

### Objetivo

Trabajar con procesos que se comunican por sockets (red) y comprender su funcionamiento. Visualizar los recursos utilizados.

Los programas funcionan como una sala de chat pública, en donde cada cliente escribe un mensaje y todos los demás clientes conectados pueden verlo. El servidor es el encargado de reenviar los mensajes que recibe de cada uno de los clientes.

### Pasos

1. para realizar la compilación de los programas servidor y cliente, ejecutar:
```bash
make sockets
```

2. Cada uno en una terminal separada, ejecutar:
   * Una instancia del servidor:
   ```bash
   ./5-servidor
   ```
   * Tres instancias del cliente:
   ```bash
   ./5-cliente
    ```

3. En una quinta terminal, ejecutar:
    ```bash
    ps -fLC 5-cliente,5-servidor
    ```

4. Analizar:
   * ¿Están emparentados los procesos?
   * ¿Cuántos hilos tiene cada proceso?

5. Interactuar con los procesos para ver su funcionamiento.
6. Analizar:
    * ¿Qué pasa si matamos el proceso servidor?

### Para debatir

* ¿Qué ventaja tiene este mecanismo de comunicación por sobre la memoria compartida?
* ¿Se podría resolver el servidor sin utilizar threads?
* ¿Cómo puedo identificar que un proceso está utilizando sockets?
* ¿Pueden identificar en el sistema sockets que utilizan distintos protocolos de comunicación? Mencionar cuáles son.
* ¿Cómo puedo conocer los puertos y procesos servidores que están levantados en la máquina? Investigar los comandos:
    ```bash
    netstat
    ss
    ```
