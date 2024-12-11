import logging
import logging.handlers
import os
import time

LOG_FILE = "server_logs.log"
LOG_SIZE = 10 * 1024 * 1024  # 10MB
LOG_BACKUP_COUNT = 3

def setup_logging():
    #Настраивает логирование с ротацией.

    logger = logging.getLogger(__name__)
    logger.setLevel(logging.INFO)

    # Создаем обработчик для записи в файл с ротацией
    handler = logging.handlers.RotatingFileHandler(LOG_FILE, maxBytes=LOG_SIZE, backupCount=LOG_BACKUP_COUNT)
    formatter = logging.Formatter("%(asctime)s - %(levelname)s - %(filename)s:%(lineno)d - %(funcName)s - %(message)s")
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    return logger


def check_log_file(LOG_FILE="server_logs.log"):
    log_dir = os.path.dirname(LOG_FILE)
    if log_dir:
        for filename in os.listdir(log_dir):
            if filename.startswith("server_logs.log.") and filename.endswith(".1"):
                filepath = os.path.join(log_dir, filename)
                # Удаление старых лог файлов через 5 дней
                if time.time() - os.path.getmtime(filepath) > 5 * 24 * 3600:
                    os.remove(filepath)
                    logger.warning(f"The old log file has been deleted: {filename}")