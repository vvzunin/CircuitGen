# **Единая системы генерации, хранения и обработки датасета цифровых схем** [CircuitGen]

<font size="5">**Содержание**:</font>
<a name="content_rus"></a> 
- [Состав программы](#modules_rus)
- [Сборка и запуск компонентов](#build_rus)
- [Датасет](#dataset_rus)
- [Планы на будущее](#plans_rus)
    
# Состав программы
<a name="modules_rus"></a> 
Программа состоит из нескольких основных модулей.

[&#8593; Contents](#content_rus)

# ВНИМАНИЕ
В данный момент (06.05.2024) в версии abc для yosys в репозитории Ubuntu 22.04 обнаружена проблема, которая не позволяет полноценно использовать сам abc. 

# Сборка и запуск компонентов
<a name="build_rus"></a> 

Сборка и запуск производятся из операционной системы Linux.

Программа запускалась на Ubuntu 22.04 с использованием WSL (Windows Subsystem for Linux).

[&#8593; Contents](#content_rus)

## Сборка проекта
Для сборки требуется наличие cmake версии не ниже 3.14. Для сборки требуется выполнит следующие команды:
```
chmod +x buildGenerator.sh
./buildGenerator.sh
```
После начнется установка дополнительных пакетов, требующихся для функционирования данной утилиты.
## Запуск
По окончании сборки исполняемый файл будет находиться в папке build и будет иметь имя `CicruitGen`. Для ознакомления с доступными командами вы можете использовать флаг -h или --help.

```
./build/CicruitGen --help
```

## Пример запуска генератора 
Далее будет дан пример запуска генератора с последующим созданием bench файла, а также файлов, оптимизированных с помощью алгоритма resyn2.

```
./build/CicruitGen -j "json_examples/sampleTruthTable.json" -r -B
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