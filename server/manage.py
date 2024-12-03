#!/usr/bin/env python
"""Django's command-line utility for administrative tasks."""
import os
import sys

from dotenv import load_dotenv


def main():
    """Run administrative tasks."""
    os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'django_project.settings')
    load_dotenv()
    try:
        from django.core.management import execute_from_command_line
        import django
        django.setup()  # Инициализация приложений Django
        from make_dataset.views import create_db_for_dataset  # Импорт после настройки Django
    except ImportError as exc:
        raise ImportError(
            "Couldn't import Django. Are you sure it's installed and "
            "available on your PYTHONPATH environment variable? Did you "
            "forget to activate a virtual environment?"
        ) from exc
    
    # Вызов вашей функции здесь, если это необходимо
    create_db_for_dataset()

    execute_from_command_line(sys.argv)


if __name__ == '__main__':
    main()
