#!/bin/bash

# Обновляем список пакетов и обновляем установленные пакеты
sudo apt-get update -y
sudo apt-get upgrade -y

# Устанавливаем PostgreSQL
sudo apt-get install -y postgresql

# Добавляем ключ для pgAdmin4
curl https://www.pgadmin.org/static/packages_pgadmin_org.pub | sudo apt-key add -

# Добавляем репозиторий для pgAdmin4
sudo sh -c 'echo "deb https://ftp.postgresql.org/pub/pgadmin/pgadmin4/apt/$(lsb_release -cs) pgadmin4 main" > /etc/apt/sources.list.d/pgadmin4.list'

# Обновляем список пакетов после добавления нового репозитория
sudo apt-get update -y

# Устанавливаем pgAdmin4 (веб-версия)
sudo apt-get install -y pgadmin4-web

# Запускаем настройку pgAdmin4
echo "Запуск настройки pgAdmin4. Пожалуйста, следуйте инструкциям для создания учетной записи администратора."
sudo /usr/pgadmin4/bin/setup-web.sh

# Убедимся, что Apache запущен
echo "Проверка и запуск Apache..."

# Проверяем наличие команды systemctl
if command -v systemctl >/dev/null; then
    # Если systemctl доступен, используем его
    sudo systemctl enable apache2
    sudo systemctl start apache2
else
    # Если systemctl не найден, проверяем наличие команды service
    if command -v service >/dev/null; then
        # Используем команду service для систем SysVinit или Upstart
        sudo service apache2 start
        sudo service apache2 restart
    else
        echo "Не найдены ни systemctl, ни service. Невозможно управлять Apache."
    fi
fi
