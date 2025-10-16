# Test de Conexión - Arduino UNO R4 WiFi con PlatformIO

## Código de prueba creado ✅

He creado un código de prueba básico en `src/main.cpp` que:
- ✅ Parpadea el LED integrado (pin 13)
- ✅ Envía mensajes por Serial cada 5 segundos
- ✅ Lee lo que escribas y lo devuelve en MAYÚSCULAS
- ✅ Muestra un contador para verificar que funciona

**IMPORTANTE**: He guardado tu código completo en `src/main_full.cpp.bak` para restaurarlo después.

---

## Pasos para probar la conexión

### 1. Instalar PlatformIO

#### Opción A: VS Code + Extensión (Recomendado)

1. **Abrir VS Code**
   ```bash
   code .
   ```

2. **Instalar extensión PlatformIO**
   - Ir a Extensions (Ctrl+Shift+X)
   - Buscar "PlatformIO IDE"
   - Clic en "Install"
   - Esperar 3-5 minutos
   - Reiniciar VS Code

3. **Verificar instalación**
   - Debería aparecer un icono de "hormiga alien" en la barra lateral izquierda
   - O presionar Ctrl+Shift+P y buscar "PlatformIO"

#### Opción B: CLI (Línea de comandos)

```bash
# Instalar Python si no está
sudo apt update
sudo apt install python3 python3-pip -y

# Instalar PlatformIO CLI
pip install -U platformio

# Añadir al PATH (si es necesario)
export PATH=$PATH:~/.local/bin

# Verificar
pio --version
```

---

### 2. Conectar el Arduino

1. **Conectar Arduino UNO R4 WiFi** al puerto USB del ordenador
2. **Verificar que se detecta**:
   ```bash
   # En Linux
   ls -l /dev/ttyACM*
   # Deberías ver algo como /dev/ttyACM0

   # O con PlatformIO
   pio device list
   ```

3. **Dar permisos** (Linux, solo primera vez):
   ```bash
   sudo usermod -a -G dialout $USER
   # Luego cerrar sesión y volver a entrar
   ```

---

### 3. Compilar el código

```bash
cd /home/raul/code/proteo

# Compilar
pio run
```

**Esperado**: Debería compilar sin errores y mostrar:
```
SUCCESS
```

---

### 4. Cargar al Arduino

```bash
# Cargar al Arduino
pio run --target upload
```

**Esperado**:
- El LED del Arduino debería parpadear durante la carga
- Al final debe mostrar "SUCCESS"

---

### 5. Abrir el Monitor Serial

```bash
# Abrir monitor serial
pio device monitor
```

**Esperado**: Deberías ver algo como:

```
========================================
  PROTEO - Test de Conexión
  Arduino UNO R4 WiFi
========================================

Sistema iniciado correctamente!
El LED debería estar parpadeando...

Escribe algo en el monitor serial
y te lo devolveré en MAYÚSCULAS
========================================

Arduino funcionando OK - Contador: 1
Arduino funcionando OK - Contador: 2
Arduino funcionando OK - Contador: 3
```

---

### 6. Probar comunicación

**Mientras el monitor está abierto:**

1. **Escribe algo** (ej: "hola proteo") y presiona Enter
2. **Deberías ver**:
   ```
   >>> Recibido: HOLA PROTEO
   >>> Longitud: 11
   ```

3. **El LED integrado** (cerca del pin 13) debería estar parpadeando cada 500ms

---

## Verificaciones

### ✅ Test exitoso si:
- [ ] El código compila sin errores
- [ ] Se carga al Arduino sin problemas
- [ ] El LED parpadea cada 500ms
- [ ] Aparecen mensajes en el monitor serial cada 5 segundos
- [ ] Puedes escribir algo y te lo devuelve en MAYÚSCULAS

### ❌ Si hay problemas:

#### Error: "pio: command not found"
```bash
# Instalar PlatformIO CLI
pip install -U platformio

# Añadir al PATH
echo 'export PATH=$PATH:~/.local/bin' >> ~/.bashrc
source ~/.bashrc
```

#### Error: "Board uno_r4_wifi not found"
```bash
# Instalar plataforma Renesas RA
pio platform install renesas-ra
```

#### Error: "Could not open port"
```bash
# Cerrar cualquier monitor serial abierto
# Verificar permisos
sudo usermod -a -G dialout $USER
# Cerrar sesión y volver a entrar
```

#### No aparece el puerto /dev/ttyACM0
```bash
# Verificar que el Arduino está conectado
lsusb | grep -i arduino

# Ver logs del kernel
dmesg | grep -i usb | tail -20
```

---

## Comandos útiles durante el test

```bash
# Compilar
pio run

# Compilar y cargar
pio run --target upload

# Monitor serial
pio device monitor

# Todo en uno (compilar, cargar, monitor)
pio run --target upload && pio device monitor

# Limpiar build
pio run --target clean

# Ver puertos disponibles
pio device list

# Salir del monitor serial
Ctrl+C
```

---

## Después del test

### Si todo funciona correctamente:

1. **Salir del monitor serial**: Presiona `Ctrl+C`

2. **Restaurar el código completo**:
   ```bash
   cp src/main_full.cpp.bak src/main.cpp
   ```

3. **Continuar con el desarrollo** del proyecto Proteo

---

## Desde VS Code (GUI)

Si instalaste la extensión PlatformIO en VS Code:

1. **Abrir el proyecto**
   - Abrir la carpeta `/home/raul/code/proteo` en VS Code

2. **Compilar**
   - Clic en el botón ✓ (check) en la barra inferior
   - O: Ver → Command Palette → "PlatformIO: Build"

3. **Cargar**
   - Clic en el botón → (flecha derecha) en la barra inferior
   - O: Ver → Command Palette → "PlatformIO: Upload"

4. **Monitor Serial**
   - Clic en el botón 🔌 (enchufe) en la barra inferior
   - O: Ver → Command Palette → "PlatformIO: Serial Monitor"

---

## Siguiente paso

Una vez que confirmes que todo funciona, avísame y restauramos el código completo del proyecto para empezar a trabajar con el sensor PreSens y la válvula.

---

**Backup creado**: `src/main_full.cpp.bak` (tu código completo está ahí guardado)
