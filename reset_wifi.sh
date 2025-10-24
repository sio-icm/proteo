#!/bin/bash

echo "🔄 PRESENS Monitor - WiFi Reset Script"
echo "====================================="

# Verificar que se ejecuta como root
if [ "$EUID" -ne 0 ]; then
    echo "❌ Please run as root: sudo ./reset_wifi.sh"
    exit 1
fi

echo "⚠️  This will restore normal WiFi functionality"
echo "⚠️  The PRESENS Access Point will be disabled"
read -p "Continue? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Cancelled."
    exit 1
fi

echo "🛑 Stopping services..."
systemctl stop hostapd
systemctl stop dnsmasq
systemctl stop presens-monitor

echo "🚫 Disabling services..."
systemctl disable hostapd
systemctl disable dnsmasq
systemctl disable presens-monitor

echo "🔧 Restoring network configuration..."
# Remove the static IP configuration we added
sed -i '/# PRESENS Monitor Access Point/,$d' /etc/dhcpcd.conf

echo "🔥 Clearing iptables rules..."
iptables -t nat -F
iptables -t nat -X
netfilter-persistent save

echo "🔄 Restarting networking..."
systemctl restart dhcpcd
systemctl restart networking

echo "✅ WiFi reset completed!"
echo "📱 Normal WiFi functionality restored"
echo "🔄 Please reboot to ensure all changes take effect: sudo reboot"