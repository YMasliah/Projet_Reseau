sudo ip link set tun0 up
sudo ip addr add 172.16.2.1/28 dev tun0

sudo ip route add 172.16.2.160/28 via 172.16.2.1
sudo ip route add 172.16.2.176/28 via 172.16.2.1
sudo sysctl -w net.ipv4.ip_forward=1
