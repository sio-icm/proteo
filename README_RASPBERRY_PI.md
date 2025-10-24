# 🌊 PRESENS O₂ Monitor - Raspberry Pi Setup

Complete setup guide to create an autonomous WiFi access point with the PRESENS sensor monitoring web application.

## 📋 Prerequisites

- Raspberry Pi 4 (recommended) or Pi 3B+
- MicroSD card (16GB+)
- PRESENS sensor connected via USB
- Fresh Raspberry Pi OS installation

## 🚀 Quick Setup

### 1. Copy project to Raspberry Pi
```bash
# Copy the entire 'proteo' folder to: /home/pi/Desktop/proteo
scp -r proteo/ pi@raspberry-ip:/home/pi/Desktop/
```

### 2. Install dependencies
```bash
cd /home/pi/Desktop/proteo
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

### 3. Test the application
```bash
# Test sensor connection first
python test_presens_simple.py

# Test web application
python app.py
# Open browser: http://raspberry-ip:5000
```

### 4. Setup Access Point (ONE COMMAND!)
```bash
sudo ./setup_ap.sh
```

### 5. Reboot
```bash
sudo reboot
```

## 📱 Usage After Setup

### Connecting to the Monitor:
1. **Find WiFi network**: `PRESENS-Monitor`
2. **Password**: `presens123`
3. **Access**: Open any web browser → automatic redirect to sensor app
4. **Direct URLs**: 
   - http://presens.local
   - http://monitor.local
   - http://192.168.4.1:5000

### Using the Application:
- **Start**: Begin automatic measurements every 5 seconds
- **Stop**: End measurement session
- **One Measurement**: Single reading
- **CSV Button**: Download measurement data
- **Clear**: Remove history from display

## 🔧 Advanced Configuration

### Change WiFi Settings:
Edit `/etc/hostapd/hostapd.conf`:
```bash
sudo nano /etc/hostapd/hostapd.conf
# Change: ssid=YOUR_NETWORK_NAME
# Change: wpa_passphrase=YOUR_PASSWORD
sudo systemctl restart hostapd
```

### Change IP Range:
Edit `/etc/dnsmasq.conf`:
```bash
sudo nano /etc/dnsmasq.conf
# Change: dhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h
sudo systemctl restart dnsmasq
```

### Application Autostart:
```bash
# Check status
sudo systemctl status presens-monitor

# View logs
sudo journalctl -u presens-monitor -f

# Restart app
sudo systemctl restart presens-monitor
```

## 🛠️ Troubleshooting

### WiFi Access Point not visible:
```bash
sudo systemctl status hostapd
sudo systemctl restart hostapd
ip addr show wlan0  # Should show 192.168.4.1
```

### Sensor not detected:
```bash
ls /dev/ttyUSB*  # Should show /dev/ttyUSB0
sudo chmod 666 /dev/ttyUSB0  # If permission issues
```

### Application not accessible:
```bash
sudo systemctl status presens-monitor
curl http://localhost:5000/api/status  # Test locally
```

### Reset to normal WiFi:
```bash
sudo ./reset_wifi.sh
sudo reboot
```

## 📂 File Structure

```
proteo/
├── app.py                    # Main Flask application
├── sensor.py                 # PRESENS sensor interface
├── requirements.txt          # Python dependencies
├── templates/
│   └── index.html           # Web interface
├── static/
│   ├── style.css           # Responsive styling
│   └── script.js           # Frontend logic
├── data/                    # CSV measurement files
├── setup_ap.sh             # Access Point setup script
├── reset_wifi.sh           # WiFi reset script
└── README_RASPBERRY_PI.md  # This file
```

## 🌐 Network Architecture

```
Internet ❌ (Standalone)
    │
┌───▼────────────────────────┐
│    Raspberry Pi            │
│  ┌─────────────────────┐   │
│  │   PRESENS Monitor   │   │
│  │     Web App         │   │
│  │   (Port 5000)       │   │
│  └─────────────────────┘   │
│           │                │
│  ┌─────────────────────┐   │
│  │   WiFi Access Point │   │
│  │   PRESENS-Monitor   │   │
│  │   192.168.4.1       │   │
│  └─────────────────────┘   │
└────────────────────────────┘
    │
    │ WiFi Connection
    │
┌───▼────────────────────────┐
│      Mobile Devices        │
│    192.168.4.2-20         │
│                            │
│  📱 Phone/Tablet          │
│  💻 Laptop                │
│  🖥️ Computer              │
└────────────────────────────┘
```

## 🔒 Security Notes

- **Default password**: Change `presens123` for production use
- **Network isolation**: No internet access by design
- **Local only**: All data stays on the Raspberry Pi
- **USB sensor**: Direct serial connection for reliability

## 📊 Data Management

- **Real-time**: Live measurements every 5 seconds
- **Storage**: CSV files in `data/` folder
- **Sessions**: New CSV file per measurement session
- **Download**: Direct download to mobile devices
- **Backup**: Copy CSV files via USB or SSH

## 🔄 Updates

To update the application:
```bash
cd /home/pi/Desktop/proteo
git pull  # If using git
sudo systemctl restart presens-monitor
```

---

**🎯 Perfect for field work, boat monitoring, laboratory setups, or any location where internet is not available!**