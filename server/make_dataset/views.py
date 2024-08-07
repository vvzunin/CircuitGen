import pathlib
import random
import subprocess

from dotenv import load_dotenv

from django.http import JsonResponse
from django.http import HttpResponse
from django.http import HttpRequest
from add_parameter.views import *
import json

from rest_framework import viewsets

from .models import Dataset
from .serializers import DatasetSerializer

from pathlib import Path
import glob

import io
import os
import shutil

from make_dataset.synology_drive_api.drive import SynologyDrive


class DatasetList(viewsets.ModelViewSet):
    queryset = Dataset.objects.all()
    serializer_class = DatasetSerializer


def add_dataset(request: HttpRequest):
    print("add_dataset is running")

    values_for_db = json.loads(request.body.decode('utf-8'))
    flags = values_for_db["flags"]
    ids = values_for_db["id"]

    # добавить датасет в базу данных
    dataset_id, parameters_of_generation, threads_num = add_dataset_to_database(
        ids)
    print("add_dataset_to_database is finished")

    # запуск генератора
    run_generator(parameters_of_generation, dataset_id, flags, threads_num)
    print("run_generator is finished")

    # запуск Yosys
    # make_image_from_verilog(dataset_id)
    print("make_image_from_verilog is finished")

    # загрузка Synology Drive
    upload_to_synology(dataset_id)
    print("upload_to_synology is finished")

    # удалить локальную папку с датасетом
    delete_folders(dataset_id)
    print("delete_folders is finished")

    print("add_dataset is finished")
    return HttpResponse("Ok")


def run_generator(parameters_of_generation, dataset_id, flags: dict, threads_num: int):
    flags_str = ''
    for key, value in flags.items():
        if (value):
            if key == "resyn2":
                flags_str += "-r "
            elif key == "balance":
                flags_str += "-b "
            elif key == "bench":
                flags_str += "-B "
            elif key == "firrtl":
                flags_str += "-F"
            else:
                print(f"Unknown key: {key}")

    # print(parameters_of_generation)
    for obj in parameters_of_generation:
        obj['dataset_id'] = dataset_id

    if not os.path.exists("jsons_for_generator/"):
        os.mkdir("jsons_for_generator/")

    with open(f'jsons_for_generator/data_{dataset_id}.json', 'w', encoding='utf-8') as f:
        json.dump(parameters_of_generation, f, ensure_ascii=False, indent=4)
    subprocess.Popen(f"./Generator/build/CircuitGen -j ./jsons_for_generator/data_{dataset_id}.json -t {threads_num} {flags_str}",
                     shell=True).wait()
    obj = Dataset.objects.get(id=dataset_id)
    obj.ready = True
    obj.save()


def make_image_from_verilog(dataset_id):
    dataset_id = int(dataset_id)
    path = 'export PATH="/Users/kudr.max/PycharmProjects/1290_project/source/data/Yosys/bin:$PATH"'
    base_folder_path = os.path.dirname(
        os.path.dirname(os.path.abspath(__file__)))
    directory = f"./dataset/{dataset_id}/"
    for verilog_path in glob.iglob(f'{directory}/**/*.v', recursive=True):
        image_path = pathlib.Path(verilog_path).parent
        full_name = os.path.basename(verilog_path)
        file_name = os.path.splitext(full_name)
        image_path = './' + str(image_path) + '/' + file_name[0]
        yo = "yosys -p'read_verilog " + verilog_path + \
            "; clean; show -format png -prefix " + image_path + "'"
        os.system(path + ";" + yo)


def progress_of_datasets(request):
    datasets = list(Dataset.objects.all().values())
    progress_list = {}
    for obj in datasets:
        ready = ready_verilogs(obj["id"])
        in_total = in_total_function(obj)
        if obj['ready'] is False:
            progress_dict = {
                "ready": ready,
                "in_total": in_total
            }
        else:
            progress_dict = {
                "ready": in_total,
                "in_total": in_total
            }
        progress_list[obj["id"]] = progress_dict
    return JsonResponse(progress_list)


def ready_verilogs(dataset_id):
    directory = Path(f"./dataset/{dataset_id}/")
    num = len(list(directory.rglob("*.v")))
    return num


def in_total_function(obj):
    list_of_param = obj["parameters_of_generation"]
    # print(list_of_param)
    in_total = 0
    for param in list_of_param:
        in_total += (param["max_in"] - param["min_in"] + 1) * (
            param["max_out"] - param["min_out"] + 1) * param["repeat_n"]
        if param["type_of_generation"] == "From Random Truth Table" and param["CNFF"] is True and param["CNFT"] is True:
            in_total *= 2
    return in_total


def upload_to_synology(dataset_id):
    load_dotenv()
    
    NAS_USER = 'project1290'
    NAS_PASS = os.getenv("NAS_PASS", "NOT_DEFINED")
    NAS_IP = 'vvzunin.me'
    NAS_PORT = 10003
    dsm_version = '7'

    with SynologyDrive(NAS_USER, NAS_PASS, NAS_IP, NAS_PORT, dsm_version=dsm_version) as synd:
        dataset_id = str(dataset_id)
        dataset_dir = f'./dataset/{dataset_id}'
        extension_lst = ['.v', '.aig', '.json', '.png', '.bench', '.graphml']

        for param in os.listdir(dataset_dir):
            # looking for subdirectiries
            sub_folders = [f.name for f in os.scandir(
                f'{dataset_dir}/{param}') if f.is_dir()]

            if not sub_folders:
                sub_folders = ['']

            for sub_folder in sub_folders:
                sub_path = f'{param}' + \
                    ('/' + sub_folder if sub_folders else '')

                for extension in extension_lst:
                    for file_path in glob.iglob(f'{dataset_dir}/{sub_path}/*{extension}', recursive=True):
                        try:
                            with open(file_path, 'rb') as file:
                                file_name = os.path.basename(file_path)
                                bfile = io.BytesIO(file.read())
                                bfile.name = f'{
                                    dataset_id}/{sub_path}/{file_name}'

                                synd.upload_file(
                                    bfile,
                                    dest_folder_path=f'/team-folders/circuits/datasets/'
                                )
                        except Exception as e:
                            print(e)


def get_link_to_synology(dataset_id, param_id):
    load_dotenv()
    
    NAS_USER = 'project1290'
    NAS_PASS = os.getenv("NAS_PASS", "NOT_DEFINED")
    NAS_IP = 'vvzunin.me'
    NAS_PORT = 10003
    dsm_version = '7'

    with SynologyDrive(NAS_USER, NAS_PASS, NAS_IP, NAS_PORT, dsm_version=dsm_version) as synd:
        synd.create_folder(f'{dataset_id}/{param_id}',
                           f'team-folders/circuits/datasets/')
        return synd.create_link(f'team-folders/circuits/datasets/{dataset_id}/{param_id}/')['data']['url']


def delete_folders(dataset_id):
    try:
        shutil.rmtree(f'./dataset/{dataset_id}/')
        for f in os.listdir('./jsons_for_generator'):
            os.remove(os.path.join('./jsons_for_generator', f))
    except OSError as e:
        print("Error: %s - %s." % (e.filename, e.strerror))


def add_dataset_to_database(id_of_parameters_of_generation: HttpRequest):
    # получаем list параметров генерации, по которым будет делать датасет
    list_of_parameters = []
    for param_id in id_of_parameters_of_generation:
        list_of_parameters.append(AddParameter.objects.filter(id=param_id))

    # добавление пустого датасета в бд датасетов
    list_of_parameters_for_dataset = []
    for param in list_of_parameters:
        link_to_parameter = "empty link"
        obj = list(param.values())[0]
        obj["link_of_parameter"] = link_to_parameter
        obj['swap_type'] = int(obj['swap_type'])
        list_of_parameters_for_dataset.append(obj)
    dataset_id = Dataset.objects.create(
        parameters_of_generation=list_of_parameters_for_dataset, ready=False).id

    threads_num = max(
        map(lambda x: x["multithread"], list_of_parameters_for_dataset))
    # замена ссылок на правильные в бд датаета
    for obj in list_of_parameters_for_dataset:
        obj['link_of_parameter'] = get_link_to_synology(dataset_id, obj['id'])
    obj = Dataset.objects.get(id=dataset_id)
    obj.parameters_of_generation = list_of_parameters_for_dataset
    obj.save()

    # получить параметры генерации
    dataset_id = str(dataset_id)
    return dataset_id, list_of_parameters_for_dataset, threads_num
