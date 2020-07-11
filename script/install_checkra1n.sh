# winra1n checkra1n installer script

echo Installing checkra1n...
echo "deb https://assets.checkra.in/debian /" | sudo tee -a /etc/apt/sources.list
sudo apt-key adv --fetch-keys https://assets.checkra.in/debian/archive.key
sudo apt update -y
sudo apt install checkra1n -y

echo Done