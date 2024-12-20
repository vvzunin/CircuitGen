# **Единая системы генерации, хранения и обработки датасета цифровых схем** [CircuitGen]

![License: MIT](https://img.shields.io/github/license/vvzunin/CircuitGen)
![GitHub forks](https://img.shields.io/github/forks/vvzunin/CircuitGen)
![GitHub Repo stars](https://img.shields.io/github/stars/vvzunin/CircuitGen)
![GitHub watchers](https://img.shields.io/github/watchers/vvzunin/CircuitGen)

![GitHub CI](https://github.com/vvzunin/CircuitGen/actions/workflows/ci.yml/badge.svg)
[![codecov](https://codecov.io/gh/vvzunin/CircuitGen/graph/badge.svg?token=U88U82QFX8)](https://codecov.io/gh/vvzunin/CircuitGen)
![GitHub Release](https://img.shields.io/github/v/release/vvzunin/CircuitGen)
![GitHub Release Date](https://img.shields.io/github/release-date/vvzunin/CircuitGen)

![GitHub commit activity](https://img.shields.io/github/commit-activity/m/vvzunin/CircuitGen)
![GitHub commits since latest release](https://img.shields.io/github/commits-since/vvzunin/CircuitGen/latest)
![GitHub last commit](https://img.shields.io/github/last-commit/vvzunin/CircuitGen)

![GitHub Issues or Pull Requests](https://img.shields.io/github/issues/vvzunin/CircuitGen)
![GitHub Issues or Pull Requests](https://img.shields.io/github/issues-pr/vvzunin/CircuitGen)

![Alt](https://repobeats.axiom.co/api/embed/393cee194e0d9302500fb5dc6551a537a5e49759.svg "Repobeats analytics image")

<font size="5">**Содержание**:</font>
<a name="content_rus"></a> 
- [Состав программы](#modules_rus)
- [Сборка и запуск компонентов](#build_rus)
- [Датасет](#dataset_rus)
- [Планы на будущее](#plans_rus)
    
# Состав программы
<a name="modules_rus"></a> 
Программа состоит из нескольких основных модулей:
- [Graph](https://github.com/vvzunin/CircuitGen_Graph) - графовое представление цифровых схем;
- [Generator](https://github.com/vvzunin/CircuitGen_Generator) - генератор цифровых схем;
- [Parameters](https://github.com/vvzunin/CircuitGen_Parameters) - расчет различных параметров цифровых схем;
- [AI](https://github.com/vvzunin/CircuitGen_AI) - набор методов машинного обучения для оценки параметров цифровых схем.

[&#8593; Contents](#content_rus)

# ВНИМАНИЕ
В данный момент (06.05.2024) в версии abc для yosys в репозитории Ubuntu 22.04 обнаружена проблема, которая не позволяет полноценно использовать сам abc. 

# Сборка и запуск компонентов
<a name="build_rus"></a> 

Сборка и запуск производятся из операционной системы Linux.

Программа запускалась на Ubuntu 22.04 с использованием WSL (Windows Subsystem for Linux).

## Установка утилит
В первую очередь треубется установить все требуемые утилиты. Для этого выполните следующие команды: 
```
chmod +x installUtils.sh
sudo ./installUtils.sh
```

Далее потребуется установить пакеты, требуемые для работы с данным проектом:

### Debian-like os
```
sudo apt install cmake g++ npm graphviz python3 python3-pip doxygen
```

## Сборка проекта
Для сборки требуется наличие cmake версии не ниже 3.14. Для сборки требуется выполнит следующие команды:
```
chmod +x buildGenerator.sh
./buildGenerator.sh
```
После начнется установка дополнительных пакетов, требующихся для функционирования данной утилиты.

## Сборка базы данных
Для сборки необходимо выполнить следующие команды:
```
chmod +x install_pgadmin.sh
./install_pgadmin.sh
```

## Backend + frontend
<a name="backend_frontend_rus"></a> 
### Установка
<a name="install_rus"></a> 
Предварительно требуется создать .env файл следующего содержщания (укажите свои значения для переменных):
```
NAS_USER="USER_NAME"
NAS_PASS="YOUR_PASSWORD"
NAS_IP="NAS_DOMEN_NAME"
NAS_PORT="NAS_PORT"
DSM_VERSION="DSM_VERSION"

DB_USER="postgres"
DB_PASSWORD="PASSWORD"
DB_HOST="localhost"
DB_PORT="5432"
DB_NAME="CircuitGen"

ER_SCRIPT_PATH=CircuitGen.sql

PG_PORT="5050"
```

После требуется запустить следующее:
```
pip install -r requirements.txt

python server/manage.py migrate --run-syncdb
cd frontend
npm i
npm run build
cd ..
```

## Запуск
<a name="run_rus"></a> 
```
python server/manage.py runserver
```

## Запуск генератора с помощью командной строки
По окончании сборки исполняемый файл будет находиться в папке Generator/build и будет иметь имя `CicruitGen`. Для ознакомления с доступными командами вы можете использовать флаг -h или --help.

```
./Generator/build/CircuitGen --help
```

## Пример запуска генератора 
Далее будет дан пример запуска генератора с последующим созданием bench файла, а также файлов, оптимизированных с помощью алгоритма resyn2.

```
./Generator/build/CircuitGen -j json_examples/sampleTruthTable.json -r -B
```

# Текущий датасет
<a name="dataset_rus"></a>

Текущая версия датасета доступна по [ссылке](https://vvzunin.me:10003/d/s/tVFkjEa5dJVgkpNCMirx37WFS3vxKPgU/tWINRKjvi7TCinaI8i5arDSuCxhKzd-o-X7RAk_qacAo).

[&#8593; Contents](#content_rus)

# Планы на будущее
<a name="plans_rus"></a>
Программ в данный момент перерабатывается и в дальнейшем планируется выполнения ряда задач:
- Разработать подпрограмму визуализации схем;
- Реализовать кросс-платформенность.

[&#8593; Contents](#content_rus)
