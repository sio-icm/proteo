# Guía de Instalación - PlatformIO para Proteo

## ¿Qué es PlatformIO?

PlatformIO es una plataforma de desarrollo profesional para sistemas embebidos que:
- ✅ Gestiona dependencias y librerías automáticamente
- ✅ Soporta múltiples placas y frameworks
- ✅ Proporciona debugging avanzado
- ✅ Se integra con VS Code
- ✅ Mejor que el Arduino IDE para proyectos complejos

---

## Instalación de PlatformIO

### Opción 1: VS Code + Extensión PlatformIO (Recomendado)

#### Paso 1: Instalar VS Code
```bash
# En Ubuntu/Debian
sudo snap install --classic code

# O descargar desde: https://code.visualstudio.com/
```

#### Paso 2: Instalar la extensión PlatformIO
1. Abrir VS Code
2. Ir a Extensions (Ctrl+Shift+X)
3. Buscar "PlatformIO IDE"
4. Clic en "Install"
5. Esperar a que termine la instalación (~5 minutos)
6. Reiniciar VS Code

#### Paso 3: Verificar instalación
1. Debería aparecer un icono de hormiga en la barra lateral
2. O ir a View → Command Palette → "PlatformIO: Home"

### Opción 2: CLI (Línea de comandos)

```bash
# Instalar Python si no está instalado
sudo apt install python3 python3-pip

# Instalar PlatformIO CLI
pip install -U platformio

# Verificar instalación
pio --version
```

---

## Configurar el Proyecto Proteo

### 1. Clonar el repositorio

```bash
cd ~/code
git clone https://github.com/sio-icm/proteo.git
cd proteo
```

### 2. Abrir en VS Code

```bash
code .
```

### 3. Estructura del proyecto

```
proteo/
├── platformio.ini      # Configuración del proyecto
├── src/
│   └── main.cpp       # Código principal
├── lib/               # Librerías locales
├── include/           # Headers
├── test/              # Tests
└── .pio/              # (generado) Build files
```

---

## Configuración para Arduino UNO R4 WiFi

El archivo `platformio.ini` ya está configurado:

```ini
[env:uno_r4_wifi]
platform = renesas-ra
board = uno_r4_wifi
framework = arduino
monitor_speed = 9600
```

---

## Compilar y Cargar el Código

### Desde VS Code (GUI)

1. **Conectar el Arduino UNO R4 WiFi** al puerto USB
2. Abrir el archivo `src/main.cpp`
3. En la barra inferior, clic en:
   - ✓ (check): Compilar
   - → (flecha): Cargar al Arduino
   - 🔌 (enchufe): Abrir monitor serial

### Desde terminal

```bash
# Compilar el proyecto
pio run

# Cargar al Arduino (detecta puerto automáticamente)
pio run --target upload

# Abrir monitor serial
pio device monitor

# Todo en uno: compilar, cargar y monitor
pio run --target upload && pio device monitor
```

---

## Configurar el Puerto Serial

### Linux

El puerto suele ser `/dev/ttyACM0` o `/dev/ttyUSB0`

```bash
# Listar puertos disponibles
pio device list

# Dar permisos al usuario (si es necesario)
sudo usermod -a -G dialout $USER
# Luego cerrar sesión y volver a entrar
```

Si quieres especificar un puerto manualmente, edita `platformio.ini`:

```ini
upload_port = /dev/ttyACM0
monitor_port = /dev/ttyACM0
```

### Windows

El puerto suele ser `COM3`, `COM4`, etc.

```ini
upload_port = COM3
monitor_port = COM3
```

---

## Usar el Monitor Serial

### Abrir el monitor

```bash
# Desde terminal
pio device monitor

# O desde VS Code: clic en el icono 🔌 en la barra inferior
```

### Comandos disponibles en Proteo

Una vez conectado, puedes escribir:

```
HELP         - Muestra todos los comandos
START        - Inicia el sistema automático
STOP         - Detiene el sistema
STATUS       - Muestra estado actual
VALVE OPEN   - Abre válvula manualmente
VALVE CLOSE  - Cierra válvula manualmente
READ         - Solicita lectura del sensor
```

### Salir del monitor

- **Linux/Mac**: `Ctrl+C`
- **Windows**: `Ctrl+C`

---

## Añadir Librerías

Si necesitas añadir librerías externas:

### Método 1: Desde VS Code
1. Abrir PlatformIO Home
2. Libraries → Search
3. Instalar la librería deseada

### Método 2: Editar platformio.ini

```ini
lib_deps =
    nombre_libreria@^1.0.0
    usuario/repositorio
    https://github.com/usuario/repo.git
```

### Método 3: CLI

```bash
pio lib search "nombre libreria"
pio lib install "nombre_libreria"
```

---

## Problemas Comunes

### Error: "Board uno_r4_wifi not found"

```bash
# Instalar plataforma Renesas RA
pio platform install renesas-ra
```

### Error: Puerto serial ocupado

```bash
# Cerrar cualquier monitor serial abierto
# Verificar que no haya otra instancia del Arduino IDE abierta
```

### Error: Permiso denegado en Linux

```bash
# Añadir usuario al grupo dialout
sudo usermod -a -G dialout $USER

# Cerrar sesión y volver a entrar
```

### El Arduino no se detecta

```bash
# Verificar que el cable USB transmite datos (no solo carga)
# Probar otro puerto USB
# Verificar drivers

# En Linux, ver logs:
dmesg | grep tty
```

---

## Comandos Útiles de PlatformIO

```bash
# Ver información del proyecto
pio project config

# Limpiar build
pio run --target clean

# Ver información de la placa
pio boards uno_r4_wifi

# Actualizar plataformas y herramientas
pio upgrade
pio platform update

# Ver información de librerías instaladas
pio lib list

# Tests unitarios
pio test
```

---

## Configuración Avanzada

### Compilación en paralelo (más rápido)

En `platformio.ini`:

```ini
build_flags =
    -j 4  ; Usar 4 cores
```

### Habilitar warnings

```ini
build_flags =
    -Wall
    -Wextra
```

### Definir constantes de compilación

```ini
build_flags =
    -D DEBUG_MODE=1
    -D VERSION="1.0.0"
```

---

## Recursos Adicionales

- [Documentación oficial PlatformIO](https://docs.platformio.org/)
- [Arduino UNO R4 WiFi - PlatformIO](https://docs.platformio.org/en/latest/boards/renesas-ra/uno_r4_wifi.html)
- [Guía de conexión hardware](./CONEXION_HARDWARE.md)
- [Repositorio del proyecto](https://github.com/sio-icm/proteo)

---

## Siguiente Paso

Una vez instalado PlatformIO y compilado el código, continúa con la [Guía de Conexión del Hardware](./CONEXION_HARDWARE.md) para conectar físicamente el sensor PreSens y la válvula.

---

*Última actualización: 2025-10-16*
