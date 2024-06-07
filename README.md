# **Единая системы генерации, хранения и обработки датасета цифровых схем** [CircuitGen]

<font size="5">**Содержание**:</font>
<a name="content_rus"></a> 
- [Состав программы](#modules_rus)
- [Сборка и запуск компонентов](#build_rus)
- [Датасет](#dataset_rus)
- [Планы на будущее](#plans_rus)
    
# Состав программы
<a name="modules_rus"></a> 
Программа состоит из нескольких основных модулей:
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

## Backend + frontend
<a name="backend_frontend_rus"></a> 
### Установка
<a name="install_rus"></a> 
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
