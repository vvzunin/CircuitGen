#!/bin/bash

# Update package lists
sudo apt-get update

# Upgrade installed packages
sudo apt-get upgrade -y

# Install PostgreSQL
sudo apt install -y postgresql

# Switch to the postgres user to create a new superuser
# Use the following SQL command to create a superuser:
# THE FOLLOWING COMMANDS SHOULD BE DONE IN THE POSTGRES
# ''' 
# psql -c "CREATE USER posttest WITH SUPERUSER PASSWORD 'test1234';"
# CREATE DATABASE CircuitGen;
# GRANT ALL PRIVILEGES ON DATABASE CircuitGen to posttest;
# \q
# exit
# '''
#sudo -i -u postgres << EOF
#psql -c "CREATE USER posttest WITH SUPERUSER PASSWORD 'test1234';"
#EOF

# Install curl via snap
sudo snap install curl

# Setup the pgAdmin repository

# Install the public key for the repository
curl -fsS https://www.pgadmin.org/static/packages_pgadmin_org.pub | sudo gpg --dearmor -o /usr/share/keyrings/packages-pgadmin-org.gpg

# Create the repository configuration file and update package lists
sudo sh -c 'echo "deb [signed-by=/usr/share/keyrings/packages-pgadmin-org.gpg] https://ftp.postgresql.org/pub/pgadmin/pgadmin4/apt/$(lsb_release -cs) pgadmin4 main" > /etc/apt/sources.list.d/pgadmin4.list && apt update'

# Install pgAdmin for web mode only
sudo apt install -y pgadmin4-web

# Configure the webserver for pgAdmin
sudo /usr/pgadmin4/bin/setup-web.sh

echo "pgAdmin installation and setup is complete."
echo "Please enter your email address and password when prompted during web setup."
