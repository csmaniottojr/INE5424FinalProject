# Cadastrador de objetos inteligentes

### Instalação

##### Configuração do banco de dados

```sh
$ mysql -u <username> -p
create database sodb_novo;
exit
$ mysql -u <username> -p sodb_novo < dump_model.sql
```

###### Na aplicação, no arquivo config_bd.py:

```python
DB_USER = ''
DB_PASSWORD = ''
HOST = ''
DB_NAME = 'sodb_novo'
```

##### Instalação das dependências

```sh
$ sudo apt-get install python3
$ sudo apt install python3-pip
$ sudo pip3 install virtualenv
$ virtualenv -p python3 venv
$ pip3 install -r requirements.txt
```

##### Execução

```sh
$ cd src
$ python3 main.py
```
