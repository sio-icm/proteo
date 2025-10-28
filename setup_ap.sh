#!/bin/bash

echo "🚀 PRESENS Monitor - Raspberry Pi Access Point Setup"
echo "=================================================="

# Verificar que se ejecuta como root
if [ "$EUID" -ne 0 ]; then
    echo "❌ Please run as root: sudo ./setup_ap.sh"
    exit 1
fi

echo "📦 Installing required packages..."
apt update
apt install -y hostapd dnsmasq iptables-persistent

echo "⚙️ Configuring hostapd..."
cat > /etc/hostapd/hostapd.conf << 'EOF'
interface=wlan0
driver=nl80211
ssid=PRESENS-Monitor
hw_mode=g
channel=7
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
EOF

echo "🌐 Configuring dnsmasq..."
cp /etc/dnsmasq.conf /etc/dnsmasq.conf.backup 2>/dev/null || true
cat > /etc/dnsmasq.conf << 'EOF'
interface=wlan0
dhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h
address=/presens.local/192.168.4.1
address=/monitor.local/192.168.4.1
address=/sensor.local/192.168.4.1
bind-interfaces
EOF

echo "🔧 Configuring dnsmasq service dependencies..."
mkdir -p /etc/systemd/system/dnsmasq.service.d
cat > /etc/systemd/system/dnsmasq.service.d/wait-for-network.conf << 'EOF'
[Unit]
After=dhcpcd.service
Wants=dhcpcd.service
EOF

echo "🔧 Disabling wpa_supplicant for wlan0..."
# Desactivar wpa_supplicant para evitar conexiones automáticas a redes guardadas
systemctl disable wpa_supplicant.service
# Renombrar el archivo de redes guardadas para que no se conecte automáticamente
if [ -f /etc/wpa_supplicant/wpa_supplicant.conf ]; then
    mv /etc/wpa_supplicant/wpa_supplicant.conf /etc/wpa_supplicant/wpa_supplicant.conf.backup
    echo "✓ Redes WiFi guardadas desactivadas (backup guardado)"
fi

echo "🔧 Configuring static IP..."
# Eliminar configuración previa si existe para evitar duplicados
sed -i '/# PRESENS Monitor Access Point/,/nohook wpa_supplicant/d' /etc/dhcpcd.conf
# Añadir configuración
cat >> /etc/dhcpcd.conf << 'EOF'

# PRESENS Monitor Access Point
interface wlan0
    static ip_address=192.168.4.1/24
    nohook wpa_supplicant
EOF

echo "🔥 Configuring iptables (captive portal)..."
iptables -t nat -A PREROUTING -i wlan0 -p tcp --dport 80 -j REDIRECT --to-port 5000
iptables -t nat -A PREROUTING -i wlan0 -p tcp --dport 443 -j REDIRECT --to-port 5000
iptables-save > /etc/iptables/rules.v4

echo "🔄 Enabling services..."
systemctl daemon-reload
systemctl unmask hostapd
systemctl enable hostapd
systemctl enable dnsmasq

echo "🔄 Restarting services in correct order..."
systemctl restart dhcpcd
sleep 2
systemctl restart hostapd
systemctl restart dnsmasq

echo "📱 Creating PRESENS app service..."
cat > /etc/systemd/system/presens-monitor.service << 'EOF'
[Unit]
Description=PRESENS O2 Monitor
After=network.target
Wants=network.target

[Service]
Type=simple
User=pi
WorkingDirectory=/home/pi/Desktop/proteo
ExecStart=/home/pi/Desktop/proteo/.venv/bin/python app.py
Restart=always
RestartSec=10
Environment=PYTHONPATH=/home/pi/Desktop/proteo

[Install]
WantedBy=multi-user.target
EOF

systemctl enable presens-monitor.service

echo "🔧 Setting hostapd daemon path..."
echo 'DAEMON_CONF="/etc/hostapd/hostapd.conf"' >> /etc/default/hostapd

echo "✅ Setup completed!"
echo ""
echo "📋 Next steps:"
echo "1. sudo reboot"
echo "2. After reboot, WiFi network 'PRESENS-Monitor' will be available (OPEN - no password)"
echo "3. Connect and go to: http://presens.local or any website"
echo "4. The sensor app will load automatically!"
echo ""
echo "🔧 Troubleshooting:"
echo "- Check services: sudo systemctl status hostapd dnsmasq presens-monitor"
echo "- View logs: sudo journalctl -u presens-monitor -f"
echo "- Check IP: ip addr show wlan0"